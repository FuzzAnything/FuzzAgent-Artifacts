#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <fuzzer/FuzzedDataProvider.h>
#include <vector>

namespace harness_009 {

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
    // Need minimum data for compression configuration testing
    if (size < 64) {
        return 0;  // Minimum size for meaningful testing
    }

    FuzzedDataProvider fdp(data, size);

    // PHASE 1: INITIALIZATION
    
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

    // PHASE 2: IMAGE SETUP
    
    // Consume image parameters from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 100);
    int bit_depth = fdp.ConsumeIntegralInRange<int>(1, 16);
    int color_type = fdp.ConsumeIntegralInRange<int>(0, 6);
    int interlace_type = fdp.ConsumeIntegralInRange<int>(0, 1);
    
    // Validate and adjust color_type and bit_depth combinations
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && 
                bit_depth != 8 && bit_depth != 16) {
                bit_depth = 8;
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

    // Set image header (IHDR)
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // PHASE 3: COMPRESSION CONFIGURATION - PRIMARY TARGETS
    
    // TARGET: png_set_compression_buffer_size - PRIMARY TARGET (0 hits, 27 undiscovered branches)
    // Test various buffer sizes: small, medium, large, zero
    int buffer_size_choice = fdp.ConsumeIntegralInRange<int>(0, 3);
    size_t buffer_size;
    
    switch (buffer_size_choice) {
        case 0:
            buffer_size = fdp.ConsumeIntegralInRange<size_t>(1, 1024);  // Small
            break;
        case 1:
            buffer_size = fdp.ConsumeIntegralInRange<size_t>(1025, 65536);  // Medium
            break;
        case 2:
            buffer_size = fdp.ConsumeIntegralInRange<size_t>(65537, 1048576);  // Large
            break;
        case 3:
            buffer_size = 0;  // Zero size (edge case)
            break;
        default:
            buffer_size = 8192;  // Default
    }
    
    png_set_compression_buffer_size(png_ptr, buffer_size);
    
    // TARGET: png_set_compression_method (9 hits, 2 undiscovered branches)
    if (fdp.ConsumeBool()) {
        int compression_method = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_compression_method(png_ptr, compression_method);
    }
    
    // TARGET: png_set_compression_level (22 hits, 1 undiscovered branch)
    if (fdp.ConsumeBool()) {
        int compression_level = fdp.ConsumeIntegralInRange<int>(0, 9);
        png_set_compression_level(png_ptr, compression_level);
    }
    
    // TARGET: png_set_compression_strategy (0 hits, 2 undiscovered branches)
    if (fdp.ConsumeBool()) {
        int compression_strategy = fdp.ConsumeIntegralInRange<int>(0, 4);
        png_set_compression_strategy(png_ptr, compression_strategy);
    }
    
    // TARGET: png_set_compression_window_bits (19 hits, 4 undiscovered branches)
    if (fdp.ConsumeBool()) {
        int window_bits = fdp.ConsumeIntegralInRange<int>(8, 15);
        png_set_compression_window_bits(png_ptr, window_bits);
    }
    
    // Also test memory level if there's enough data
    if (fdp.remaining_bytes() > 0 && fdp.ConsumeBool()) {
        int mem_level = fdp.ConsumeIntegralInRange<int>(1, 9);
        png_set_compression_mem_level(png_ptr, mem_level);
    }

    // PHASE 4: VALIDATION
    
    // TARGET: png_get_compression_buffer_size - VALIDATION (0 hits, 4 undiscovered branches)
    size_t retrieved_buffer_size = png_get_compression_buffer_size(png_ptr);
    // Note: The retrieved size may not match the set size due to implementation constraints
    // This is okay - we're just testing that the function can be called

    // PHASE 5: WRITE PNG DATA
    
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
        default: channels = 3; break;
    }
    
    size_t row_size = (width * channels * bit_depth + 7) / 8;
    
    // Generate simple image data using remaining fuzzer input
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_bytep)png_malloc(png_ptr, row_size);
        if (fdp.remaining_bytes() >= row_size) {
            std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(row_size);
            memcpy(row_pointers[y], row_data.data(), row_size);
        } else {
            // Fill with pattern if not enough data
            memset(row_pointers[y], (y * 7) & 0xFF, row_size);
        }
    }
    
    // Write image data
    png_write_image(png_ptr, row_pointers);
    
    // Write end of PNG
    png_write_end(png_ptr, info_ptr);

    // PHASE 6: CLEANUP
    
    // Free row data
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Free write buffer
    free(write_data.buffer);
    
    // Destroy PNG structures
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_009
