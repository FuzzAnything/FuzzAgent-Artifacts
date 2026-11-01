// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <png.h>

static void png_fuzzer_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_fuzzer_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set error handlers
    png_set_error_fn(png_ptr, NULL, png_fuzzer_error_callback, png_fuzzer_warning_callback);

    // Use setjmp to catch libpng errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize IHDR to make the structure valid for some operations
    // We use a minimal valid configuration: 1x1 pixel, 8-bit RGB
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Helper to extract fixed point from fuzz data
    auto get_fixed = [&](size_t &idx) -> png_fixed_point {
        if (idx + sizeof(png_fixed_point) > Size) return 0;
        png_fixed_point val;
        memcpy(&val, &Data[idx], sizeof(png_fixed_point));
        idx += sizeof(png_fixed_point);
        return val;
    };

    auto get_uint32 = [&](size_t &idx) -> png_uint_32 {
        if (idx + sizeof(png_uint_32) > Size) return 0;
        png_uint_32 val;
        memcpy(&val, &Data[idx], sizeof(png_uint_32));
        idx += sizeof(png_uint_32);
        return val;
    };

    // 1. Fuzz png_set_cHRM_XYZ_fixed
    size_t idx = 0;
    png_fixed_point rx = get_fixed(idx);
    png_fixed_point ry = get_fixed(idx);
    png_fixed_point rz = get_fixed(idx);
    png_fixed_point gx = get_fixed(idx);
    png_fixed_point gy = get_fixed(idx);
    png_fixed_point gz = get_fixed(idx);
    png_fixed_point bx = get_fixed(idx);
    png_fixed_point by = get_fixed(idx);
    png_fixed_point bz = get_fixed(idx);
    
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, rx, ry, rz, gx, gy, gz, bx, by, bz);

    // 2. Fuzz png_get_cHRM_fixed
    png_fixed_point w_x, w_y, r_x, r_y, g_x, g_y, b_x, b_y;
    png_get_cHRM_fixed(png_ptr, info_ptr, &w_x, &w_y, &r_x, &r_y, &g_x, &g_y, &b_x, &b_y);

    // 3. Fuzz png_get_cHRM_XYZ_fixed
    png_fixed_point r_X, r_Y, r_Z, g_X, g_Y, g_Z, b_X, b_Y, b_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &r_X, &r_Y, &r_Z, &g_X, &g_Y, &g_Z, &b_X, &b_Y, &b_Z);

    // 4. Fuzz png_set_mDCV_fixed
    idx = 0;
    png_fixed_point md_w_x = get_fixed(idx);
    png_fixed_point md_w_y = get_fixed(idx);
    png_fixed_point md_r_x = get_fixed(idx);
    png_fixed_point md_r_y = get_fixed(idx);
    png_fixed_point md_g_x = get_fixed(idx);
    png_fixed_point md_g_y = get_fixed(idx);
    png_fixed_point md_b_x = get_fixed(idx);
    png_fixed_point md_b_y = get_fixed(idx);
    png_uint_32 max_lum = get_uint32(idx);
    png_uint_32 min_lum = get_uint32(idx);

    png_set_mDCV_fixed(png_ptr, info_ptr, md_w_x, md_w_y, md_r_x, md_r_y, md_g_x, md_g_y, md_b_x, md_b_y, max_lum, min_lum);

    // 5. Fuzz png_get_mDCV_fixed
    png_fixed_point get_md_w_x, get_md_w_y, get_md_r_x, get_md_r_y, get_md_g_x, get_md_g_y, get_md_b_x, get_md_b_y;
    png_uint_32 get_max_lum, get_min_lum;
    png_get_mDCV_fixed(png_ptr, info_ptr, &get_md_w_x, &get_md_w_y, &get_md_r_x, &get_md_r_y, &get_md_g_x, &get_md_g_y, &get_md_b_x, &get_md_b_y, &get_max_lum, &get_min_lum);

    // 6. Fuzz png_get_sCAL_fixed
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}