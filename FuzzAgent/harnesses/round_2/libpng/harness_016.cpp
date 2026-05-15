#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace harness_016 {

// Custom error handler functions
void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just return
}

void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing - just return
}

// Custom read function for libpng that reads from fuzzer input
void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Handle zero-length reads according to libpng's API contract
    if (length == 0) {
        return;  // Nothing to read, nothing to copy
    }
    
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    FuzzedDataProvider* fdp = static_cast<FuzzedDataProvider*>(io_ptr);
    if (fdp->remaining_bytes() < length) {
        png_error(png_ptr, "Insufficient fuzzer data");
        return;
    }
    
    std::vector<uint8_t> bytes = fdp->ConsumeBytes<uint8_t>(length);
    // length > 0 at this point, so bytes.data() is valid (even for empty vector)
    memcpy(data, bytes.data(), length);
}

// Helper function to allocate and fill image rows for testing
png_byte** allocate_image_rows(png_uint_32 width, png_uint_32 height, int channels, 
                               int bit_depth, FuzzedDataProvider& fdp) {
    if (width == 0 || height == 0) return NULL;
    
    // Calculate bytes per pixel
    int bytes_per_pixel;
    if (bit_depth == 16) {
        bytes_per_pixel = channels * 2;  // 2 bytes per channel
    } else if (bit_depth == 8) {
        bytes_per_pixel = channels * 1;  // 1 byte per channel
    } else {
        // For other bit depths, use 8-bit as fallback
        bytes_per_pixel = channels * 1;
    }
    
    // Allocate row pointers
    png_byte** rows = (png_byte**)malloc(height * sizeof(png_byte*));
    if (rows == NULL) return NULL;
    
    // Allocate each row and fill with fuzzer data
    for (png_uint_32 y = 0; y < height; y++) {
        rows[y] = (png_byte*)malloc(width * bytes_per_pixel);
        if (rows[y] == NULL) {
            // Cleanup on allocation failure
            for (png_uint_32 i = 0; i < y; i++) {
                free(rows[i]);
            }
            free(rows);
            return NULL;
        }
        
        // Fill row with fuzzer data
        if (fdp.remaining_bytes() >= width * bytes_per_pixel) {
            std::vector<uint8_t> row_data = fdp.ConsumeBytes<uint8_t>(width * bytes_per_pixel);
            memcpy(rows[y], row_data.data(), width * bytes_per_pixel);
        } else {
            // Not enough data, fill with pattern
            for (int x = 0; x < width * bytes_per_pixel; x++) {
                rows[y][x] = (x + y) % 256;
            }
        }
    }
    
    return rows;
}

// Helper function to free allocated image rows
void free_image_rows(png_byte** rows, png_uint_32 height) {
    if (rows == NULL) return;
    
    for (png_uint_32 y = 0; y < height; y++) {
        if (rows[y] != NULL) {
            free(rows[y]);
        }
    }
    free(rows);
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum data for meaningful testing
    if (size < 100) return 0;
    
    FuzzedDataProvider fdp(data, size);
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 NULL, 
                                                 user_error_fn, 
                                                 user_warning_fn);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Create end info structure
    png_infop end_info_ptr = png_create_info_struct(png_ptr);
    if (end_info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up error handling using setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
        return 0;
    }
    
    // Set up custom read function using fuzzer data
    png_set_read_fn(png_ptr, &fdp, user_read_data);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions and format
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type, compression_type, filter_type;
    
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);
    
    // Consume transformation configuration from fuzzer input
    uint8_t transform_flags = fdp.ConsumeIntegral<uint8_t>();
    double red_coeff = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double green_coeff = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
    double screen_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
    double file_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
    int error_action = fdp.ConsumeIntegralInRange<int>(1, 3); // PNG_ERROR_ACTION_NONE to ERROR
    int alpha_mode = fdp.ConsumeIntegralInRange<int>(0, 3); // PNG_ALPHA_PNG to PNG_ALPHA_BROKEN
    double output_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
    int background_gamma_code = fdp.ConsumeIntegralInRange<int>(0, 3);
    double background_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
    int need_expand = fdp.ConsumeBool() ? 1 : 0;
    
    // Create background color from fuzzer input
    png_color_16 background_color;
    background_color.red = fdp.ConsumeIntegralInRange<png_uint_16>(0, 65535);
    background_color.green = fdp.ConsumeIntegralInRange<png_uint_16>(0, 65535);
    background_color.blue = fdp.ConsumeIntegralInRange<png_uint_16>(0, 65535);
    background_color.gray = fdp.ConsumeIntegralInRange<png_uint_16>(0, 65535);
    background_color.index = fdp.ConsumeIntegral<png_byte>();
    
    // Apply color transformations based on fuzzer input
    
    // 1. Convert RGB to grayscale
    if (transform_flags & 0x01 && color_type & PNG_COLOR_MASK_COLOR) {
        png_set_rgb_to_gray(png_ptr, error_action, red_coeff, green_coeff);
    }
    
    // 2. Apply gamma correction
    if (transform_flags & 0x02) {
        png_set_gamma(png_ptr, screen_gamma, file_gamma);
    }
    
    // 3. Set background color (for alpha blending)
    if (transform_flags & 0x04) {
        png_set_background(png_ptr, &background_color, background_gamma_code, 
                          need_expand, background_gamma);
    }
    
    // 4. Set alpha mode (how to handle alpha channel)
    if (transform_flags & 0x08 && (color_type & PNG_COLOR_MASK_ALPHA)) {
        png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);
    }
    
    // 5. Convert tRNS to alpha (for palette images with transparency)
    if (transform_flags & 0x10 && color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_tRNS_to_alpha(png_ptr);
    }
    
    // Apply other common transformations based on image type
    if (transform_flags & 0x20 && bit_depth == 16) {
        png_set_strip_16(png_ptr);
    }
    
    if (transform_flags & 0x40 && (color_type & PNG_COLOR_MASK_ALPHA)) {
        png_set_strip_alpha(png_ptr);
    }
    
    if (transform_flags & 0x80 && color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png_ptr);
    }
    
    // Update transformation info
    png_read_update_info(png_ptr, info_ptr);
    
    // Get updated image info after transformations
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_type, &compression_type, &filter_type);
    
    // Check RGB to gray status if applicable
    if (transform_flags & 0x01) {
        png_byte rgb_to_gray_status = png_get_rgb_to_gray_status(png_ptr);
        (void)rgb_to_gray_status; // Use status if needed
    }
    
    // Allocate and read image rows to test transformations
    int channels = png_get_channels(png_ptr, info_ptr);
    png_byte** rows = allocate_image_rows(width, height, channels, bit_depth, fdp);
    if (rows != NULL) {
        // Read the image data to apply transformations
        png_read_image(png_ptr, rows);
        
        // Free allocated rows
        free_image_rows(rows, height);
    }
    
    // Read remaining data
    png_read_end(png_ptr, end_info_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
    
    return 0;
}

} // namespace harness_016
