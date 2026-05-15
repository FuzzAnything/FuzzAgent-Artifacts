#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <fuzzer/FuzzedDataProvider.h>
#include "png.h"

namespace harness_002 {


// Simple error handling for fuzzing
static void pngtest_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void pngtest_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
struct png_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void png_read_from_buffer(png_structp png_ptr, png_bytep out, png_size_t length) {
    png_read_data* rd = (png_read_data*)png_get_io_ptr(png_ptr);
    if (rd->offset + length > rd->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out, rd->data + rd->offset, length);
    rd->offset += length;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) {
        // Need minimum data for PNG signature and basic parameters
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;
    
    // Create read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngtest_error, pngtest_warning);
    if (png_ptr == NULL) {
        return 0;
    }
    
    // Set up error handling with setjmp - MUST be done before any libpng calls that can longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        if (png_ptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        }
        return 0;
    }
    
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    end_info = png_create_info_struct(png_ptr);
    if (end_info == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up custom read function to read from our buffer
    png_read_data read_data = {data, size, 0};
    png_set_read_fn(png_ptr, &read_data, png_read_from_buffer);
    
    // Tell libpng we haven't read any signature bytes yet
    png_set_sig_bytes(png_ptr, 0);
    
    // Read PNG info (this reads the signature and IHDR)
    png_read_info(png_ptr, info_ptr);
    
    // Get image dimensions and color type
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Use fuzzer input to decide which transformations to apply
    // Consume a byte to determine transformation configuration
    uint8_t transform_config = fdp.ConsumeIntegral<uint8_t>();
    
    // Apply various transformations based on fuzzer input
    // This targets the pngrtran.c module specifically
    
    // 1. Gamma transformations
    if (transform_config & 0x01) {
        double screen_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
        double file_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
        png_set_gamma(png_ptr, screen_gamma, file_gamma);
    }
    
    // 2. Alpha mode transformations
    if (transform_config & 0x02) {
        int alpha_mode = fdp.PickValueInArray({
            PNG_ALPHA_PNG,
            PNG_ALPHA_STANDARD,
            PNG_ALPHA_ASSOCIATED,
            PNG_ALPHA_PREMULTIPLIED,
            PNG_ALPHA_OPTIMIZED,
            PNG_ALPHA_BROKEN
        });
        double output_gamma = fdp.ConsumeFloatingPointInRange<double>(0.1, 5.0);
        png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);
    }
    
    // 3. Expand transformations
    if (transform_config & 0x04) {
        png_set_expand(png_ptr);
        
        if (color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_palette_to_rgb(png_ptr);
        }
        
        if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
            png_set_tRNS_to_alpha(png_ptr);
        }
        
        if (bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
    }
    
    // 4. Expand to 16-bit
    if ((transform_config & 0x08) && bit_depth == 8) {
        png_set_expand_16(png_ptr);
    }
    
    // 5. BGR transformation
    if (transform_config & 0x10) {
        png_set_bgr(png_ptr);
    }
    
    // 6. Invert transformations
    if (transform_config & 0x20) {
        if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
            png_set_invert_mono(png_ptr);
        }
        
        if (color_type & PNG_COLOR_MASK_ALPHA) {
            png_set_invert_alpha(png_ptr);
        }
    }
    
    // 7. Shift transformation
    if (transform_config & 0x40) {
        if (color_type == PNG_COLOR_TYPE_PALETTE || bit_depth < 8) {
            png_color_8 true_bits;
            true_bits.red = fdp.ConsumeIntegralInRange<png_byte>(0, 7);
            true_bits.green = fdp.ConsumeIntegralInRange<png_byte>(0, 7);
            true_bits.blue = fdp.ConsumeIntegralInRange<png_byte>(0, 7);
            true_bits.gray = fdp.ConsumeIntegralInRange<png_byte>(0, 7);
            true_bits.alpha = fdp.ConsumeIntegralInRange<png_byte>(0, 7);
            png_set_shift(png_ptr, &true_bits);
        }
    }
    
    // Additional transformations that require more parameters
    if (fdp.remaining_bytes() > 4) {
        // Filler transformation (add or remove alpha channel)
        if (fdp.ConsumeBool()) {
            int filler = fdp.ConsumeIntegral<uint16_t>();
            int flags = fdp.PickValueInArray({0, PNG_FILLER_BEFORE, PNG_FILLER_AFTER});
            png_set_filler(png_ptr, filler, flags);
        }
        
        // RGB to gray transformation
        if (fdp.ConsumeBool() && (color_type & PNG_COLOR_MASK_COLOR)) {
            int error_action = fdp.PickValueInArray({
                PNG_ERROR_ACTION_NONE,
                PNG_ERROR_ACTION_WARN,
                PNG_ERROR_ACTION_ERROR
            });
            double red_coeff = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
            double green_coeff = fdp.ConsumeFloatingPointInRange<double>(0.0, 1.0);
            png_set_rgb_to_gray(png_ptr, error_action, red_coeff, green_coeff);
        }
        
        // Gray to RGB transformation
        if (fdp.ConsumeBool() && !(color_type & PNG_COLOR_MASK_COLOR)) {
            png_set_gray_to_rgb(png_ptr);
        }
        
        // Swap alpha transformation
        if (fdp.ConsumeBool() && (color_type & PNG_COLOR_MASK_ALPHA)) {
            png_set_swap_alpha(png_ptr);
        }
        
        // Swap bytes transformation (for 16-bit data)
        if (fdp.ConsumeBool() && bit_depth == 16) {
            png_set_swap(png_ptr);
        }
        
        // Packswap transformation
        if (fdp.ConsumeBool() && bit_depth < 8) {
            png_set_packswap(png_ptr);
        }
    }
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Get updated rowbytes after transformations
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Allocate row pointers
    png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, height * sizeof(png_bytep));
    if (row_pointers == NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }
    
    // Allocate memory for each row
    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)png_malloc(png_ptr, rowbytes);
        if (row_pointers[y] == NULL) {
            // Clean up already allocated rows
            for (png_uint_32 y2 = 0; y2 < y; y2++) {
                png_free(png_ptr, row_pointers[y2]);
            }
            png_free(png_ptr, row_pointers);
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            return 0;
        }
    }
    
    // Read the image data (this will apply the transformations)
    png_read_image(png_ptr, row_pointers);
    
    // Read the end info
    png_read_end(png_ptr, end_info);
    
    // Clean up row memory
    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    
    // Clean up PNG structures
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    
    return 0;
}

} // namespace harness_002
