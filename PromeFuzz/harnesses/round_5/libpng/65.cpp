// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
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
#include <cstdlib>
#include <cstdio>
#include <csetjmp>

#include "code/png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function that returns zeros
    for (png_size_t i = 0; i < length; ++i) {
        data[i] = 0;
    }
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    static const char* dummy_filename = "./dummy_file";
    
    // Write data to dummy file
    FILE* fp = fopen(dummy_filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Open dummy file for reading
    fp = fopen(dummy_filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Get some basic info to make transformations meaningful
    int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    int color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Call target functions based on input data to explore different states
    // Use first byte to determine which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        if (selector & 0x01) {
            png_set_strip_16(png_ptr);
        }
        if (selector & 0x02) {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        if (selector & 0x04) {
            png_set_gray_to_rgb(png_ptr);
        }
        if (selector & 0x08) {
            png_set_expand(png_ptr);
        }
        if (selector & 0x10) {
            png_set_scale_16(png_ptr);
        }
        if (selector & 0x20) {
            png_set_expand_16(png_ptr);
        }
    }
    
    // Update info after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Try to read some image data (allocate row buffer)
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    if (width > 0 && height > 0 && rowbytes > 0) {
        png_bytep row = (png_bytep)png_malloc(png_ptr, rowbytes);
        if (row) {
            for (png_uint_32 y = 0; y < height; y++) {
                png_read_row(png_ptr, row, nullptr);
            }
            png_free(png_ptr, row);
        }
    }
    
    // Clean up
    png_read_end(png_ptr, nullptr);
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}