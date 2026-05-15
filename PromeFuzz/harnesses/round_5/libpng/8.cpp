// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstring>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    return png_create_info_struct(png_ptr);
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, nullptr);
        return 0;
    }
    
    // Extract values from fuzzer input for various parameters
    // We'll use the data to generate different parameter combinations
    
    // For offset parameters
    png_int_32 offset_x = 0;
    png_int_32 offset_y = 0;
    int unit_type = 0;
    
    // For mDCV parameters
    png_fixed_point int_white_x = 0;
    png_fixed_point int_white_y = 0;
    png_fixed_point int_red_x = 0;
    png_fixed_point int_red_y = 0;
    png_fixed_point int_green_x = 0;
    png_fixed_point int_green_y = 0;
    png_fixed_point int_blue_x = 0;
    png_fixed_point int_blue_y = 0;
    png_uint_32 max_luminance = 0;
    png_uint_32 min_luminance = 0;
    
    // Extract values from fuzzer input if we have enough data
    size_t offset = 0;
    
    if (Size >= sizeof(png_int_32) * 2 + sizeof(int)) {
        memcpy(&offset_x, Data + offset, sizeof(png_int_32));
        offset += sizeof(png_int_32);
        memcpy(&offset_y, Data + offset, sizeof(png_int_32));
        offset += sizeof(png_int_32);
        memcpy(&unit_type, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    // Extract mDCV parameters if we have enough data
    if (Size - offset >= sizeof(png_fixed_point) * 8 + sizeof(png_uint_32) * 2) {
        memcpy(&int_white_x, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&int_white_y, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&int_red_x, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&int_red_y, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&int_green_x, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&int_green_y, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&int_blue_x, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&int_blue_y, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&max_luminance, Data + offset, sizeof(png_uint_32));
        offset += sizeof(png_uint_32);
        memcpy(&min_luminance, Data + offset, sizeof(png_uint_32));
    }
    
    // Set oFFs chunk data
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);
    
    // Try to get offset values using different functions
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    
    // Try to get oFFs using png_get_oFFs
    png_int_32 get_offset_x = 0;
    png_int_32 get_offset_y = 0;
    int get_unit_type = 0;
    png_uint_32 oFFs_ret = png_get_oFFs(png_ptr, info_ptr, &get_offset_x, &get_offset_y, &get_unit_type);
    
    // Set mDCV chunk data
    png_set_mDCV_fixed(png_ptr, info_ptr,
                       int_white_x, int_white_y,
                       int_red_x, int_red_y,
                       int_green_x, int_green_y,
                       int_blue_x, int_blue_y,
                       max_luminance, min_luminance);
    
    // Clean up
    cleanup_png(png_ptr, info_ptr);
    
    return 0;
}