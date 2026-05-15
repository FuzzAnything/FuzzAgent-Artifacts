#include <fuzzer/FuzzedDataProvider.h>
#include <png.h>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <string>

namespace harness_004 {

// Buffer structure for memory I/O
struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Callback functions for libpng error handling
static void png_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Custom read function for reading from memory buffer
static void png_read_from_memory(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) {
        png_error(png_ptr, "No IO pointer");
        return;
    }
    
    mem_buffer* buffer = (mem_buffer*)io_ptr;
    
    if (buffer->offset + length > buffer->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buffer->data + buffer->offset, length);
    buffer->offset += length;
}

// Status callback for read status notifications
static void png_read_status_callback(png_structp png_ptr, png_uint_32 row, int pass) {
    (void)png_ptr;
    (void)row;
    (void)pass;
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    // Need minimum size for meaningful PNG operations
    if (size < 100) {
        return 0;
    }
    
    FuzzedDataProvider fdp(data, size);
    
    // Prepare memory buffer for custom read function
    struct mem_buffer buffer;
    buffer.data = data;
    buffer.size = size;
    buffer.offset = 0;
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(
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
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling via setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Setup custom read function
    png_set_read_fn(png_ptr, &buffer, png_read_from_memory);
    
    // Set read status callback (Target API from guidance)
    png_set_read_status_fn(png_ptr, png_read_status_callback);
    
    // Consume transform flags from fuzzer input
    int transforms = fdp.ConsumeIntegral<int>();
    
    // Consume number of signature bytes already read
    int sig_bytes = fdp.ConsumeIntegralInRange<int>(0, 8);
    png_set_sig_bytes(png_ptr, sig_bytes);
    
    // Consume parameters for png_set_quantize if supported
#ifdef PNG_READ_QUANTIZE_SUPPORTED
    if (fdp.remaining_bytes() > 100) {
        // Create a simple palette for testing
        png_color palette[256];
        int num_palette = fdp.ConsumeIntegralInRange<int>(1, 256);
        int maximum_colors = fdp.ConsumeIntegralInRange<int>(1, 256);
        
        for (int i = 0; i < num_palette && i < 256; i++) {
            palette[i].red = fdp.ConsumeIntegral<uint8_t>();
            palette[i].green = fdp.ConsumeIntegral<uint8_t>();
            palette[i].blue = fdp.ConsumeIntegral<uint8_t>();
        }
        
        // Create histogram if enough data
        png_uint_16 histogram[256];
        int full_quantize = fdp.ConsumeBool();
        
        if (full_quantize && fdp.remaining_bytes() >= 512) {
            for (int i = 0; i < 256; i++) {
                histogram[i] = fdp.ConsumeIntegral<uint16_t>();
            }
            png_set_quantize(png_ptr, palette, num_palette, maximum_colors, histogram, full_quantize);
        } else {
            png_set_quantize(png_ptr, palette, num_palette, maximum_colors, NULL, 0);
        }
    }
#endif
    
    // Consume filter parameters if supported
#ifdef PNG_READ_FILTER_SUPPORTED
    {
        int filter_method = fdp.ConsumeIntegralInRange<int>(0, 5);
        png_set_filter(png_ptr, PNG_FILTER_TYPE_BASE, filter_method);
    }
#endif
    
    // Apply other transform configurations based on transform flags
    // These mirror what png_read_png does internally
    if ((transforms & PNG_TRANSFORM_SCALE_16) != 0) {
#ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
        png_set_scale_16(png_ptr);
#endif
    }
    
    if ((transforms & PNG_TRANSFORM_STRIP_16) != 0) {
#ifdef PNG_READ_STRIP_16_TO_8_SUPPORTED
        png_set_strip_16(png_ptr);
#endif
    }
    
    if ((transforms & PNG_TRANSFORM_STRIP_ALPHA) != 0) {
#ifdef PNG_READ_STRIP_ALPHA_SUPPORTED
        png_set_strip_alpha(png_ptr);
#endif
    }
    
    if ((transforms & PNG_TRANSFORM_EXPAND) != 0) {
        png_set_expand(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_PACKING) != 0) {
        png_set_packing(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_PACKSWAP) != 0) {
        png_set_packswap(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_INVERT_MONO) != 0) {
        png_set_invert_mono(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_SHIFT) != 0) {
        // Consume shift parameters
        png_color_8 sig_bit;
        if (fdp.remaining_bytes() >= 4) {
            sig_bit.red = fdp.ConsumeIntegral<uint8_t>();
            sig_bit.green = fdp.ConsumeIntegral<uint8_t>();
            sig_bit.blue = fdp.ConsumeIntegral<uint8_t>();
            sig_bit.alpha = fdp.ConsumeIntegral<uint8_t>();
            png_set_shift(png_ptr, &sig_bit);
        }
    }
    
    if ((transforms & PNG_TRANSFORM_BGR) != 0) {
        png_set_bgr(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_SWAP_ALPHA) != 0) {
        png_set_swap_alpha(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_SWAP_ENDIAN) != 0) {
        png_set_swap(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_INVERT_ALPHA) != 0) {
        png_set_invert_alpha(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_GRAY_TO_RGB) != 0) {
        png_set_gray_to_rgb(png_ptr);
    }
    
    if ((transforms & PNG_TRANSFORM_EXPAND_16) != 0) {
#ifdef PNG_READ_EXPAND_16_SUPPORTED
        png_set_expand_16(png_ptr);
#endif
    }
    
    // Now test the core reading operation: png_read_png (Primary target - 273 branches)
    // First try to read info
    png_read_info(png_ptr, info_ptr);
    
    // Test png_read_update_info (Target API)
    png_read_update_info(png_ptr, info_ptr);
    
    // Read the PNG with the specified transforms
    png_read_png(png_ptr, info_ptr, transforms, NULL);
    
    // Alternative: Test png_read_rows for more granular control
    if (fdp.ConsumeBool()) {
        // Get image dimensions first
        png_uint_32 width, height;
        int bit_depth, color_type, interlace_type, compression_type, filter_type;
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                     &interlace_type, &compression_type, &filter_type);
        
        if (height > 0) {
            // Allocate row pointers
            png_bytep* row_pointers = (png_bytep*)png_malloc(png_ptr, 
                height * sizeof(png_bytep));
            
            png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
            
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
            }
            
            // Read rows (Target API - 60 branches)
            png_read_rows(png_ptr, row_pointers, NULL, height);
            
            // Clean up row pointers
            for (png_uint_32 y = 0; y < height; y++) {
                png_free(png_ptr, row_pointers[y]);
            }
            png_free(png_ptr, row_pointers);
        }
    }
    
    // Test metadata retrieval functions from pngget.c (Target module - 2.85% coverage)
    // These functions extract metadata after reading
#ifdef PNG_mDCV_SUPPORTED
    {
        double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        double mastering_display_maximum_luminance, mastering_display_minimum_luminance;
        if (png_get_mDCV(png_ptr, info_ptr, &white_x, &white_y, 
                         &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y,
                         &mastering_display_maximum_luminance, &mastering_display_minimum_luminance)) {
            // Successfully retrieved mDCV data
            (void)white_x; (void)white_y;
            (void)red_x; (void)red_y;
            (void)green_x; (void)green_y;
            (void)blue_x; (void)blue_y;
            (void)mastering_display_maximum_luminance; (void)mastering_display_minimum_luminance;
        }
        
        png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y;
        png_fixed_point int_green_x, int_green_y, int_blue_x, int_blue_y;
        png_uint_32 mastering_display_maximum_luminance_scaled_by_10000;
        png_uint_32 mastering_display_minimum_luminance_scaled_by_10000;
        if (png_get_mDCV_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y,
                              &int_red_x, &int_red_y, &int_green_x, &int_green_y,
                              &int_blue_x, &int_blue_y,
                              &mastering_display_maximum_luminance_scaled_by_10000,
                              &mastering_display_minimum_luminance_scaled_by_10000)) {
            // Successfully retrieved fixed point mDCV data
            (void)int_white_x; (void)int_white_y;
            (void)int_red_x; (void)int_red_y;
            (void)int_green_x; (void)int_green_y;
            (void)int_blue_x; (void)int_blue_y;
            (void)mastering_display_maximum_luminance_scaled_by_10000;
            (void)mastering_display_minimum_luminance_scaled_by_10000;
        }
    }
#endif
    
    // Test png_get_cHRM functions
    {
        double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        if (png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, 
                         &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y)) {
            // Successfully retrieved cHRM data
            (void)white_x; (void)white_y;
            (void)red_x; (void)red_y;
            (void)green_x; (void)green_y;
            (void)blue_x; (void)blue_y;
        }
        
        png_fixed_point white_x_fixed, white_y_fixed, red_x_fixed, red_y_fixed;
        png_fixed_point green_x_fixed, green_y_fixed, blue_x_fixed, blue_y_fixed;
        if (png_get_cHRM_fixed(png_ptr, info_ptr, &white_x_fixed, &white_y_fixed,
                              &red_x_fixed, &red_y_fixed, &green_x_fixed, &green_y_fixed,
                              &blue_x_fixed, &blue_y_fixed)) {
            // Successfully retrieved fixed point cHRM data
            (void)white_x_fixed; (void)white_y_fixed;
            (void)red_x_fixed; (void)red_y_fixed;
            (void)green_x_fixed; (void)green_y_fixed;
            (void)blue_x_fixed; (void)blue_y_fixed;
        }
    }
    
    // Test png_get_cHRM_XYZ functions (if supported)
#ifdef PNG_cHRM_XYZ_SUPPORTED
    {
        double red_X, red_Y, red_Z, green_X, green_Y, green_Z;
        double blue_X, blue_Y, blue_Z;
        if (png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z,
                            &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z)) {
            // Successfully retrieved cHRM XYZ data
            (void)red_X; (void)red_Y; (void)red_Z;
            (void)green_X; (void)green_Y; (void)green_Z;
            (void)blue_X; (void)blue_Y; (void)blue_Z;
        }
        
        png_fixed_point red_X_fixed, red_Y_fixed, red_Z_fixed;
        png_fixed_point green_X_fixed, green_Y_fixed, green_Z_fixed;
        png_fixed_point blue_X_fixed, blue_Y_fixed, blue_Z_fixed;
        if (png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &red_X_fixed, &red_Y_fixed, &red_Z_fixed,
                                  &green_X_fixed, &green_Y_fixed, &green_Z_fixed,
                                  &blue_X_fixed, &blue_Y_fixed, &blue_Z_fixed)) {
            // Successfully retrieved fixed point cHRM XYZ data
            (void)red_X_fixed; (void)red_Y_fixed; (void)red_Z_fixed;
            (void)green_X_fixed; (void)green_Y_fixed; (void)green_Z_fixed;
            (void)blue_X_fixed; (void)blue_Y_fixed; (void)blue_Z_fixed;
        }
    }
#endif
    
    // Test other metadata retrieval functions
    {
        png_uint_32 width, height;
        int bit_depth, color_type, interlace_type, compression_type, filter_type;
        
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                     &interlace_type, &compression_type, &filter_type);
        
        // Get various chunks if present
        png_bytep profile;
        png_uint_32 proflen;
        if (png_get_iCCP(png_ptr, info_ptr, NULL, NULL, &profile, &proflen)) {
            (void)profile; (void)proflen;
        }
        
        png_textp text_ptr;
        int num_text;
        if (png_get_text(png_ptr, info_ptr, &text_ptr, &num_text)) {
            (void)text_ptr; (void)num_text;
        }
        
        png_timep mod_time;
        if (png_get_tIME(png_ptr, info_ptr, &mod_time)) {
            (void)mod_time;
        }
        
        png_colorp palette;
        int num_palette;
        if (png_get_PLTE(png_ptr, info_ptr, &palette, &num_palette)) {
            (void)palette; (void)num_palette;
        }
        
        png_bytep trans_alpha;
        int num_trans;
        png_color_16p trans_color;
        if (png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color)) {
            (void)trans_alpha; (void)num_trans; (void)trans_color;
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}

} // namespace harness_004
