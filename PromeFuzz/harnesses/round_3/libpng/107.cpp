// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that reads from a buffer
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    uint8_t** buffer_ptr = static_cast<uint8_t**>(io_ptr);
    if (*buffer_ptr) {
        size_t to_copy = length;
        if (to_copy > 0) {
            memcpy(data, *buffer_ptr, to_copy);
            *buffer_ptr += to_copy;
        }
    }
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just prevent the default error handler from aborting
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                 dummy_error_fn, dummy_warning_fn);
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
        remove("./dummy_file");
        return 0;
    }
    
    // Set up file reading
    png_init_io(png_ptr, fp);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Test png_set_alpha_mode_fixed with different modes
    static const int alpha_modes[] = {
        PNG_ALPHA_PNG,
        PNG_ALPHA_STANDARD,
        PNG_ALPHA_ASSOCIATED,
        PNG_ALPHA_PREMULTIPLIED,
        PNG_ALPHA_OPTIMIZED
    };
    
    static const png_fixed_point gammas[] = {
        PNG_DEFAULT_sRGB,
        PNG_GAMMA_MAC_18,
        PNG_GAMMA_sRGB,
        PNG_GAMMA_LINEAR,
        100000,  // 1.0
        150000,  // 1.5
        200000   // 2.0
    };
    
    // Try different alpha modes and gammas
    for (size_t i = 0; i < sizeof(alpha_modes)/sizeof(alpha_modes[0]) && i < Size; i++) {
        for (size_t j = 0; j < sizeof(gammas)/sizeof(gammas[0]) && j < Size; j++) {
            png_set_alpha_mode_fixed(png_ptr, alpha_modes[i % 5], gammas[j % 7]);
        }
    }
    
    // Test png_set_interlace_handling
    int passes = png_set_interlace_handling(png_ptr);
    
    // Test png_set_bgr
    png_set_bgr(png_ptr);
    
    // Test png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Get image dimensions
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Allocate row buffer if needed
    png_bytep row = nullptr;
    png_bytep display_row = nullptr;
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    if (rowbytes > 0 && rowbytes < 1000000) {  // Sanity check
        row = (png_bytep)png_malloc(png_ptr, rowbytes);
        display_row = (png_bytep)png_malloc(png_ptr, rowbytes);
    }
    
    // Test png_read_row in a loop
    if (row && display_row && height > 0 && height < 10000) {  // Sanity check
        for (png_uint_32 y = 0; y < height; y++) {
            // Test with both row and display_row provided
            png_read_row(png_ptr, row, display_row);
            
            // Test with NULL display_row
            png_read_row(png_ptr, row, nullptr);
            
            // Test with NULL row (skip processing)
            png_read_row(png_ptr, nullptr, display_row);
            
            // Test with both NULL
            png_read_row(png_ptr, nullptr, nullptr);
        }
    }
    
    // Clean up row buffers
    if (row) png_free(png_ptr, row);
    if (display_row) png_free(png_ptr, display_row);
    
    // Reset file pointer to beginning for png_read_png test
    rewind(fp);
    
    // Test png_read_png with different transforms
    static const int transforms[] = {
        PNG_TRANSFORM_IDENTITY,
        PNG_TRANSFORM_STRIP_16,
        PNG_TRANSFORM_STRIP_ALPHA,
        PNG_TRANSFORM_PACKING,
        PNG_TRANSFORM_PACKSWAP,
        PNG_TRANSFORM_EXPAND,
        PNG_TRANSFORM_GRAY_TO_RGB,
        PNG_TRANSFORM_INVERT_MONO,
        PNG_TRANSFORM_SHIFT,
        PNG_TRANSFORM_BGR,
        PNG_TRANSFORM_SWAP_ALPHA,
        PNG_TRANSFORM_SWAP_ENDIAN,
        PNG_TRANSFORM_INVERT_ALPHA,
        PNG_TRANSFORM_SCALE_16
    };
    
    // Create new structures for png_read_png test
    png_structp png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (png_ptr2) {
        png_infop info_ptr2 = png_create_info_struct(png_ptr2);
        if (info_ptr2) {
            if (!setjmp(png_jmpbuf(png_ptr2))) {
                png_init_io(png_ptr2, fp);
                
                // Try different transforms
                for (size_t i = 0; i < sizeof(transforms)/sizeof(transforms[0]) && i < Size; i++) {
                    png_read_png(png_ptr2, info_ptr2, transforms[i % 14], nullptr);
                }
            }
            png_destroy_read_struct(&png_ptr2, &info_ptr2, nullptr);
        } else {
            png_destroy_read_struct(&png_ptr2, nullptr, nullptr);
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    remove("./dummy_file");
    
    return 0;
}