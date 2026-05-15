// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cLLI at pngset.c:194:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "png.h"
#include <cstdint>
#include <cstddef>
#include <cstring>

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy writer
}

static void dummy_flush_data(png_structp png_ptr) {
    // Do nothing - dummy writer
}

static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy reader
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data
    
    // Create dummy PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up dummy I/O functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);
    png_set_read_fn(png_ptr, nullptr, dummy_read_data);
    
    // Extract values from fuzzer input
    size_t offset = 0;
    
    // For png_set_cLLI (doubles)
    double maxCLL = 0.0, maxFALL = 0.0;
    if (offset + 16 <= Size) {
        memcpy(&maxCLL, Data + offset, 8);
        memcpy(&maxFALL, Data + offset + 8, 8);
        offset += 16;
    }
    
    // For png_set_cLLI_fixed (scaled by 10000)
    png_uint_32 maxCLL_fixed = 0, maxFALL_fixed = 0;
    if (offset + 8 <= Size) {
        memcpy(&maxCLL_fixed, Data + offset, 4);
        memcpy(&maxFALL_fixed, Data + offset + 4, 4);
        offset += 8;
    }
    
    // For png_set_mDCV (8 doubles)
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double max_lum = 0.0, min_lum = 0.0;
    
    if (offset + 64 <= Size) {
        memcpy(&white_x, Data + offset, 8);
        memcpy(&white_y, Data + offset + 8, 8);
        memcpy(&red_x, Data + offset + 16, 8);
        memcpy(&red_y, Data + offset + 24, 8);
        memcpy(&green_x, Data + offset + 32, 8);
        memcpy(&green_y, Data + offset + 40, 8);
        memcpy(&blue_x, Data + offset + 48, 8);
        memcpy(&blue_y, Data + offset + 56, 8);
        offset += 64;
    }
    
    if (offset + 16 <= Size) {
        memcpy(&max_lum, Data + offset, 8);
        memcpy(&min_lum, Data + offset + 8, 8);
        offset += 16;
    }
    
    // For png_set_mDCV_fixed (8 png_fixed_point + 2 png_uint_32)
    png_fixed_point int_white_x = 0, int_white_y = 0;
    png_fixed_point int_red_x = 0, int_red_y = 0;
    png_fixed_point int_green_x = 0, int_green_y = 0;
    png_fixed_point int_blue_x = 0, int_blue_y = 0;
    png_uint_32 max_lum_scaled = 0, min_lum_scaled = 0;
    
    if (offset + 32 <= Size) {
        memcpy(&int_white_x, Data + offset, 4);
        memcpy(&int_white_y, Data + offset + 4, 4);
        memcpy(&int_red_x, Data + offset + 8, 4);
        memcpy(&int_red_y, Data + offset + 12, 4);
        memcpy(&int_green_x, Data + offset + 16, 4);
        memcpy(&int_green_y, Data + offset + 20, 4);
        memcpy(&int_blue_x, Data + offset + 24, 4);
        memcpy(&int_blue_y, Data + offset + 28, 4);
        offset += 32;
    }
    
    if (offset + 8 <= Size) {
        memcpy(&max_lum_scaled, Data + offset, 4);
        memcpy(&min_lum_scaled, Data + offset + 4, 4);
        offset += 8;
    }
    
    // For png_get_oFFs
    png_int_32 offset_x = 0, offset_y = 0;
    int unit_type = 0;
    if (offset + 12 <= Size) {
        memcpy(&offset_x, Data + offset, 4);
        memcpy(&offset_y, Data + offset + 4, 4);
        memcpy(&unit_type, Data + offset + 8, 4);
        offset += 12;
    }
    
    // Test API functions
    
    // 1. Test png_set_cLLI and png_get_cLLI
    png_set_cLLI(png_ptr, info_ptr, maxCLL, maxFALL);
    
    double retrieved_maxCLL = 0.0, retrieved_maxFALL = 0.0;
    png_uint_32 clli_result = png_get_cLLI(png_ptr, info_ptr, &retrieved_maxCLL, &retrieved_maxFALL);
    
    // 2. Test png_set_cLLI_fixed and png_get_cLLI_fixed
    png_set_cLLI_fixed(png_ptr, info_ptr, maxCLL_fixed, maxFALL_fixed);
    
    png_uint_32 retrieved_maxCLL_fixed = 0, retrieved_maxFALL_fixed = 0;
    png_uint_32 clli_fixed_result = png_get_cLLI_fixed(png_ptr, info_ptr, 
                                                      &retrieved_maxCLL_fixed, 
                                                      &retrieved_maxFALL_fixed);
    
    // 3. Test png_set_mDCV and png_get_mDCV
    png_set_mDCV(png_ptr, info_ptr, 
                 white_x, white_y,
                 red_x, red_y,
                 green_x, green_y,
                 blue_x, blue_y,
                 max_lum, min_lum);
    
    double out_white_x, out_white_y;
    double out_red_x, out_red_y;
    double out_green_x, out_green_y;
    double out_blue_x, out_blue_y;
    double out_max_lum, out_min_lum;
    
    png_uint_32 mdcv_result = png_get_mDCV(png_ptr, info_ptr,
                                          &out_white_x, &out_white_y,
                                          &out_red_x, &out_red_y,
                                          &out_green_x, &out_green_y,
                                          &out_blue_x, &out_blue_y,
                                          &out_max_lum, &out_min_lum);
    
    // 4. Test png_set_mDCV_fixed and png_get_mDCV_fixed
    png_set_mDCV_fixed(png_ptr, info_ptr,
                       int_white_x, int_white_y,
                       int_red_x, int_red_y,
                       int_green_x, int_green_y,
                       int_blue_x, int_blue_y,
                       max_lum_scaled, min_lum_scaled);
    
    png_fixed_point out_int_white_x, out_int_white_y;
    png_fixed_point out_int_red_x, out_int_red_y;
    png_fixed_point out_int_green_x, out_int_green_y;
    png_fixed_point out_int_blue_x, out_int_blue_y;
    png_uint_32 out_max_lum_scaled, out_min_lum_scaled;
    
    png_uint_32 mdcv_fixed_result = png_get_mDCV_fixed(png_ptr, info_ptr,
                                                      &out_int_white_x, &out_int_white_y,
                                                      &out_int_red_x, &out_int_red_y,
                                                      &out_int_green_x, &out_int_green_y,
                                                      &out_int_blue_x, &out_int_blue_y,
                                                      &out_max_lum_scaled, &out_min_lum_scaled);
    
    // 5. Test png_get_oFFs (note: we haven't set oFFs, so this should return 0)
    png_int_32 out_offset_x, out_offset_y;
    int out_unit_type;
    png_uint_32 offs_result = png_get_oFFs(png_ptr, info_ptr, 
                                          &out_offset_x, &out_offset_y, 
                                          &out_unit_type);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}