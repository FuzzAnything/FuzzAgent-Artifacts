// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_image at pngread.c:696:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_filler at pngtrans.c:119:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_read_fn(png_structp png_ptr, png_bytep out, png_size_t length) {
    // Dummy read function that returns zeroed data
    memset(out, 0, length);
}

extern "C" int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Extract parameters from fuzz data
    // Use first bytes for various parameters, with fallback defaults
    png_uint_32 filler = 0;
    int flags = 0;
    int mode = 0;
    png_fixed_point output_gamma = 0;
    png_fixed_point red_coeff = 0;
    png_fixed_point green_coeff = 0;
    int error_action = 0;
    int background_gamma_code = 0;
    int need_expand = 0;
    png_fixed_point background_gamma = 0;
    double output_gamma_double = 1.0;
    
    if (Size >= 4) {
        filler = *reinterpret_cast<const png_uint_32*>(Data);
        if (Size >= 8) {
            flags = static_cast<int>(Data[4]);
            mode = static_cast<int>(Data[5]);
            output_gamma = *reinterpret_cast<const png_int_32*>(&Data[6]);
            if (Size >= 16) {
                red_coeff = *reinterpret_cast<const png_int_32*>(&Data[10]);
                green_coeff = *reinterpret_cast<const png_int_32*>(&Data[14]);
                if (Size >= 20) {
                    error_action = static_cast<int>(Data[18]);
                    background_gamma_code = static_cast<int>(Data[19]);
                    if (Size >= 24) {
                        need_expand = static_cast<int>(Data[20]);
                        background_gamma = *reinterpret_cast<const png_int_32*>(&Data[21]);
                        if (Size >= 32) {
                            output_gamma_double = *reinterpret_cast<const double*>(&Data[25]);
                        }
                    }
                }
            }
        }
    }
    
    // Call target API functions
    png_set_filler(png_ptr, filler, flags);
    
    png_set_alpha_mode_fixed(png_ptr, mode, output_gamma);
    
    png_set_add_alpha(png_ptr, filler, flags);
    
    png_color_16 background_color;
    background_color.index = 0;
    background_color.red = 0;
    background_color.green = 0;
    background_color.blue = 0;
    background_color.gray = 0;
    png_set_background_fixed(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma);
    
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red_coeff, green_coeff);
    
    png_set_alpha_mode(png_ptr, mode, output_gamma_double);
    
    // Read image data to trigger processing
    png_read_update_info(png_ptr, info_ptr);
    
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    
    if (height > 0 && width > 0) {
        png_bytep row_pointers[height];
        png_uint_32 rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = static_cast<png_bytep>(png_malloc(png_ptr, rowbytes));
        }
        
        png_read_image(png_ptr, row_pointers);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
    }
    
    // Clean up
    png_read_end(png_ptr, info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}