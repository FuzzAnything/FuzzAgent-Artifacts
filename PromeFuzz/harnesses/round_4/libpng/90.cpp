// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static png_structp create_png_struct(bool is_write) {
    if (is_write) {
        return png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                       dummy_error_fn, dummy_warning_fn);
    } else {
        return png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                      dummy_error_fn, dummy_warning_fn);
    }
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    // Need at least 12 bytes for three 32-bit values
    if (Size < 12) return 0;
    
    // Extract fuzzed values from input
    png_uint_32 res_x = *(reinterpret_cast<const png_uint_32*>(Data));
    png_uint_32 res_y = *(reinterpret_cast<const png_uint_32*>(Data + 4));
    int unit_type = *(reinterpret_cast<const int*>(Data + 8));
    
    // Create write structure and info
    png_structp write_ptr = create_png_struct(true);
    if (!write_ptr) return 0;
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Test png_set_pHYs with fuzzed values
    png_set_pHYs(write_ptr, write_info, res_x, res_y, unit_type);
    
    // Create read structure and info to test get functions
    png_structp read_ptr = create_png_struct(false);
    if (!read_ptr) {
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Set up error handling for read
    if (setjmp(png_jmpbuf(read_ptr))) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Copy pHYs data from write to read info for testing get functions
    png_uint_32 write_res_x, write_res_y;
    int write_unit_type;
    png_get_pHYs(write_ptr, write_info, &write_res_x, &write_res_y, &write_unit_type);
    
    // Set the same values in read info
    png_set_pHYs(read_ptr, read_info, write_res_x, write_res_y, write_unit_type);
    
    // Test all get functions
    png_uint_32 get_res_x, get_res_y;
    int get_unit_type;
    
    // Test png_get_pHYs
    png_uint_32 info_phys = png_get_pHYs(read_ptr, read_info, &get_res_x, &get_res_y, &get_unit_type);
    
    // Test png_get_y_pixels_per_inch
    png_uint_32 y_ppi = png_get_y_pixels_per_inch(read_ptr, read_info);
    
    // Test png_get_x_pixels_per_inch
    png_uint_32 x_ppi = png_get_x_pixels_per_inch(read_ptr, read_info);
    
    // Test png_get_pixels_per_inch
    png_uint_32 ppi = png_get_pixels_per_inch(read_ptr, read_info);
    
    // Test png_get_pHYs_dpi
    png_uint_32 dpi_res_x, dpi_res_y;
    int dpi_unit_type;
    png_uint_32 info_phys_dpi = png_get_pHYs_dpi(read_ptr, read_info, &dpi_res_x, &dpi_res_y, &dpi_unit_type);
    
    // Test with null pointers to ensure they don't crash
    png_get_pHYs(read_ptr, read_info, nullptr, nullptr, nullptr);
    png_get_pHYs_dpi(read_ptr, read_info, nullptr, nullptr, nullptr);
    
    // Test with different combinations of null pointers
    png_get_pHYs(read_ptr, read_info, &get_res_x, nullptr, nullptr);
    png_get_pHYs(read_ptr, read_info, nullptr, &get_res_y, nullptr);
    png_get_pHYs(read_ptr, read_info, nullptr, nullptr, &get_unit_type);
    
    // Clean up
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    return 0;
}