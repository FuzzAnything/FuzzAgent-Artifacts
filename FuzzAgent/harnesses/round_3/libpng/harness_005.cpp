#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_005 {

// Structure to hold write callback data for memory-based output
struct png_write_data {
    uint8_t* buffer;
    size_t capacity;
    size_t size;
};

// Write callback function for memory-based output
static void png_write_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_write_data* write_data = (png_write_data*)png_get_io_ptr(png_ptr);
    
    if (write_data->size + length > write_data->capacity) {
        // Reallocate buffer if needed
        size_t new_capacity = write_data->capacity * 2;
        if (new_capacity < write_data->size + length) {
            new_capacity = write_data->size + length;
        }
        uint8_t* new_buffer = (uint8_t*)realloc(write_data->buffer, new_capacity);
        if (new_buffer == NULL) {
            png_error(png_ptr, "Memory allocation failed in write callback");
            return;
        }
        write_data->buffer = new_buffer;
        write_data->capacity = new_capacity;
    }
    
    memcpy(write_data->buffer + write_data->size, data, length);
    write_data->size += length;
}

// Flush callback function (optional)
static void png_flush_callback(png_structp png_ptr) {
    // Nothing to do for memory-based output
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 64) {
        return 0;  // Minimum size for meaningful testing with filter/compression settings
    }

    FuzzedDataProvider fdp(data, size);

    // Consume image dimensions and parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Validate color_type and bit_depth combinations
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && 
                bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;  // Default to 8 bits
            }
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if (bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;
            }
            break;
        case PNG_COLOR_TYPE_PALETTE:
            if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && bit_depth != 8) {
                bit_depth = 8;
            }
            break;
        case PNG_COLOR_TYPE_RGB:
            if (bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;
            }
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            if (bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;
            }
            break;
        default:
            color_type = PNG_COLOR_TYPE_RGB;
            bit_depth = 8;
    }

    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,  // error_ptr
        nullptr,  // error_fn
        nullptr   // warn_fn
    );
    
    if (png_ptr == nullptr) {
        return 0;
    }

    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize write data structure
    png_write_data write_data;
    write_data.buffer = (uint8_t*)malloc(4096);
    write_data.capacity = 4096;
    write_data.size = 0;
    
    if (write_data.buffer == nullptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Set up the write function with our memory-based callback
    png_set_write_fn(png_ptr, &write_data, png_write_callback, png_flush_callback);

    // Set image header (IHDR) - TARGET: png_set_IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // TARGET: png_set_filter - Test different filter types
    int filter_flags = fdp.ConsumeIntegralInRange<int>(0, 255);
    png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, filter_flags);

    // TARGET: png_set_compression_window_bits - Compression settings
    if (fdp.ConsumeBool()) {
        int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        png_set_compression_window_bits(png_ptr, window_bits);
    }

    // TARGET: png_set_text_compression_window_bits - Text compression settings
    if (fdp.ConsumeBool()) {
        int text_window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        png_set_text_compression_window_bits(png_ptr, text_window_bits);
    }

    // TARGET: png_set_compression_method - Compression method
    if (fdp.ConsumeBool()) {
        int compression_method = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_compression_method(png_ptr, compression_method);
    }

    // TARGET: png_set_text_compression_method - Text compression method
    if (fdp.ConsumeBool()) {
        int text_compression_method = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_text_compression_method(png_ptr, text_compression_method);
    }

    // Additional compression settings
    if (fdp.ConsumeBool()) {
        int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_compression_level(png_ptr, compression_level);
    }

    if (fdp.ConsumeBool()) {
        int text_compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_text_compression_level(png_ptr, text_compression_level);
    }

    // Write header
    png_write_info(png_ptr, info_ptr);

    // Allocate and populate image rows
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
    
    // Calculate row size based on color type and bit depth
    int channels;
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY: channels = 1; break;
        case PNG_COLOR_TYPE_GRAY_ALPHA: channels = 2; break;
        case PNG_COLOR_TYPE_PALETTE: channels = 1; break;
        case PNG_COLOR_TYPE_RGB: channels = 3; break;
        case PNG_COLOR_TYPE_RGB_ALPHA: channels = 4; break;
        default: channels = 3;
    }
    
    size_t row_size = (width * channels * bit_depth + 7) / 8;
    
    // Generate some image data
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, row_size);
        
        // Fill row with fuzzer data
        std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_size);
        if (row_data.size() == row_size) {
            memcpy(row_pointers[y], row_data.data(), row_size);
        } else {
            // If not enough fuzzer data, fill with simple pattern
            for (size_t x = 0; x < row_size; x++) {
                row_pointers[y][x] = (png_byte)((x + y) % 256);
            }
        }
    }
    
    // TARGET: png_write_rows - Write multiple rows at once
    png_write_rows(png_ptr, row_pointers, height);

    // Test png_write_row as well (single row writing)
    if (height > 0 && fdp.ConsumeBool()) {
        // Write first row again with png_write_row
        png_write_row(png_ptr, row_pointers[0]);
    }

    // Finish writing
    png_write_end(png_ptr, info_ptr);

    // Clean up row pointers
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

    // Clean up write data buffer
    free(write_data.buffer);

    // Clean up PNG structures
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}

} // namespace harness_005
