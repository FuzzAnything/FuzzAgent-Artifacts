#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace harness_008 {

// Buffer structure for memory I/O
struct mem_buffer {
    uint8_t* data;
    size_t size;
    size_t offset;
};

// Custom write function for writing to memory buffer
static void png_write_to_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        // Reallocate buffer if needed
        size_t new_size = buffer->offset + length;
        uint8_t* new_data = (uint8_t*)realloc(buffer->data, new_size);
        if (new_data == NULL) {
            return;
        }
        buffer->data = new_data;
        buffer->size = new_size;
    }
    
    memcpy(buffer->data + buffer->offset, data, length);
    buffer->offset += length;
}

// Custom flush function
static void png_flush_memory(png_structp png_ptr) {
    (void)png_ptr;
}

// Error callback
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

// Warning callback
static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 100) {
        // Need minimum size for meaningful PNG transformation testing
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        png_error_callback,
        png_warning_callback);
    
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up memory buffer for writing
    mem_buffer buffer;
    buffer.data = (uint8_t*)malloc(4096);
    buffer.size = 4096;
    buffer.offset = 0;
    
    if (buffer.data == NULL) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set write function
    png_set_write_fn(png_ptr, &buffer, png_write_to_memory, png_flush_memory);
    
    // Consume parameters for IHDR from fuzzer input
    png_uint_32 width = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    png_uint_32 height = fdp.ConsumeIntegralInRange<png_uint_32>(1, 256);
    
    // Choose bit depth and color type based on transformation requirements
    uint8_t config_choice = fdp.ConsumeIntegral<uint8_t>() % 8;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_GRAY;
    
    // Different configurations to test various transformations
    switch (config_choice) {
        case 0:
            // Grayscale 8-bit - for packing, shift, invert_mono
            bit_depth = 8;
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
        case 1:
            // Grayscale 16-bit - for swap, strip_16, shift
            bit_depth = 16;
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
        case 2:
            // Grayscale with alpha 8-bit - for swap_alpha, invert_alpha
            bit_depth = 8;
            color_type = PNG_COLOR_TYPE_GRAY_ALPHA;
            break;
        case 3:
            // RGB 8-bit - for bgr, gray_to_rgb
            bit_depth = 8;
            color_type = PNG_COLOR_TYPE_RGB;
            break;
        case 4:
            // RGB 16-bit - for swap, strip_16, shift
            bit_depth = 16;
            color_type = PNG_COLOR_TYPE_RGB;
            break;
        case 5:
            // RGBA 8-bit - for bgr, swap_alpha, invert_alpha
            bit_depth = 8;
            color_type = PNG_COLOR_TYPE_RGB_ALPHA;
            break;
        case 6:
            // Grayscale 1-bit - for packing
            bit_depth = 1;
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
        case 7:
            // Grayscale 2-bit - for packing
            bit_depth = 2;
            color_type = PNG_COLOR_TYPE_GRAY;
            break;
    }
    
    // Set IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Apply transformation APIs based on configuration
    uint8_t transform_mask = fdp.ConsumeIntegral<uint8_t>();
    
    // Apply packing for low bit depths
    if ((transform_mask & 0x01) && (bit_depth == 1 || bit_depth == 2 || bit_depth == 4)) {
        png_set_packing(png_ptr);
    }
    
    // Apply shift transformation
    if (transform_mask & 0x02) {
        png_color_8 true_bits;
        true_bits.red = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
        true_bits.green = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
        true_bits.blue = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
        true_bits.gray = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
        true_bits.alpha = fdp.ConsumeIntegralInRange<uint8_t>(1, bit_depth);
        png_set_shift(png_ptr, &true_bits);
    }
    
    // Apply swap for 16-bit images
    if ((transform_mask & 0x04) && bit_depth == 16) {
        png_set_swap(png_ptr);
    }
    
    // Apply BGR for RGB images
    if ((transform_mask & 0x08) && (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_RGB_ALPHA)) {
        png_set_bgr(png_ptr);
    }
    
    // Apply invert mono for grayscale images
    if ((transform_mask & 0x10) && (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)) {
        png_set_invert_mono(png_ptr);
    }
    
    // Apply swap alpha for images with alpha
    if ((transform_mask & 0x20) && (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_RGB_ALPHA)) {
        png_set_swap_alpha(png_ptr);
    }
    
    // Apply invert alpha for images with alpha
    if ((transform_mask & 0x40) && (color_type == PNG_COLOR_TYPE_GRAY_ALPHA || color_type == PNG_COLOR_TYPE_RGB_ALPHA)) {
        png_set_invert_alpha(png_ptr);
    }
    
    // Apply strip 16 for 16-bit images
    if ((transform_mask & 0x80) && bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }
    
    // Apply gray to rgb for grayscale images
    if ((transform_mask & 0x01) && (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)) {
        png_set_gray_to_rgb(png_ptr);
    }
    
    // Write info
    png_write_info(png_ptr, info_ptr);
    
    // Generate appropriate test image data based on configuration
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    if (row_bytes == 0 || row_bytes > 1024 * 1024) { // Limit memory usage
        free(buffer.data);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Allocate row buffer
    std::vector<png_byte> row_buffer(row_bytes);
    
    // Fill row with fuzzer data
    size_t data_needed = row_bytes * height;
    if (fdp.remaining_bytes() < data_needed) {
        // Not enough data, use what we have
        data_needed = std::min(fdp.remaining_bytes(), data_needed);
    }
    
    // Fill rows with consumed data
    for (png_uint_32 y = 0; y < height; y++) {
        if (fdp.remaining_bytes() > 0) {
            size_t bytes_to_consume = std::min(row_bytes, fdp.remaining_bytes());
            auto row_data = fdp.ConsumeBytes<uint8_t>(bytes_to_consume);
            memcpy(row_buffer.data(), row_data.data(), bytes_to_consume);
            
            // Fill remaining with zeros if needed
            if (bytes_to_consume < row_bytes) {
                memset(row_buffer.data() + bytes_to_consume, 0, row_bytes - bytes_to_consume);
            }
        } else {
            // No more data, fill with pattern
            for (size_t i = 0; i < row_bytes; i++) {
                row_buffer[i] = (png_byte)((y * 31 + i * 7) & 0xFF);
            }
        }
        
        png_write_row(png_ptr, row_buffer.data());
    }
    
    // Write end
    png_write_end(png_ptr, info_ptr);
    
    // Cleanup
    free(buffer.data);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}

} // namespace harness_008
