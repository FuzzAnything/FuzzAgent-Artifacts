// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
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

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    uint8_t** input_data = (uint8_t**)io_ptr;
    if (*input_data) {
        memcpy(data, *input_data, length);
        *input_data += length;
    }
}

static void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler - do nothing for fuzzing
}

static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_343(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Need at least PNG signature
    
    // Write data to file for libpng to read
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) return 0;
    
    // Check PNG signature
    png_byte sig[8];
    if (fread(sig, 1, 8, fp) != 8) {
        fclose(fp);
        return 0;
    }
    
    if (png_sig_cmp(sig, 0, 8) != 0) {
        fclose(fp);
        return 0;
    }
    
    // Create png_struct and png_info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, 
                                                 user_error_fn, 
                                                 user_warning_fn);
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
    png_set_sig_bytes(png_ptr, 8);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Call target API functions
    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    
    png_uint_32 x_pixels_per_meter = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    png_uint_32 y_pixels_per_meter = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    
    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    png_uint_32 x_pixels_per_inch = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    
    // Try fixed point versions if supported
#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    png_fixed_point pixel_aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
#endif
    
    // Try png_get_pHYs_dpi if supported
#ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x, res_y;
    int unit_type;
    png_uint_32 phys_dpi = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
#endif
    
    // Try other related functions
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}