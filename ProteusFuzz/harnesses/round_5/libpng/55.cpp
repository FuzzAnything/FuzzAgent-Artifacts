// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
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
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int ret = 0;

    // Initialize libpng structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize write context
    png_set_write_fn(png_ptr, NULL, dummy_write_fn, NULL);

    // Extract data from fuzz input
    double white_x = (double)(Data[0] / 255.0);
    double white_y = (double)(Data[1] / 255.0);
    double red_x = (double)((Data[0] + 1) % 256 / 255.0);
    double red_y = (double)((Data[1] + 1) % 256 / 255.0);
    double green_x = (double)((Data[0] + 2) % 256 / 255.0);
    double green_y = (double)((Data[1] + 2) % 256 / 255.0);
    double blue_x = (double)((Data[0] + 3) % 256 / 255.0);
    double blue_y = (double)((Data[1] + 3) % 256 / 255.0);

    double red_X = (double)(Data[0] / 100.0);
    double red_Y = (double)(Data[1] / 100.0);
    double red_Z = (double)((Data[0] + Data[1]) / 200.0);
    double green_X = (double)((Data[0] + 1) / 100.0);
    double green_Y = (double)((Data[1] + 1) / 100.0);
    double green_Z = (double)((Data[0] + Data[1] + 1) / 200.0);
    double blue_X = (double)((Data[0] + 2) / 100.0);
    double blue_Y = (double)((Data[1] + 2) / 100.0);
    double blue_Z = (double)((Data[0] + Data[1] + 2) / 200.0);

    double max_lum = (double)(Data[0] * 100.0);
    double min_lum = (double)(Data[1] * 0.1);

    png_fixed_point fx = (png_fixed_point)(Data[0] * 10000);
    png_fixed_point fy = (png_fixed_point)(Data[1] * 10000);
    png_uint_32 lum_scaled = (png_uint_32)(Data[0] * 10000);

    // Fuzz png_set_cHRM
    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    // Fuzz png_set_cHRM_fixed
    png_set_cHRM_fixed(png_ptr, info_ptr, fx, fy, fx, fy, fx, fy, fx, fy);

    // Fuzz png_set_cHRM_XYZ
    png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // Fuzz png_set_cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, fx, fy, fx, fx, fy, fx, fx, fy, fx);

    // Fuzz png_set_mDCV
    png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y, max_lum, min_lum);

    // Fuzz png_set_mDCV_fixed
    png_set_mDCV_fixed(png_ptr, info_ptr, fx, fy, fx, fy, fx, fy, fx, fy, lum_scaled, lum_scaled);

    // Fuzz png_get_cHRM
    double g_white_x, g_white_y, g_red_x, g_red_y, g_green_x, g_green_y, g_blue_x, g_blue_y;
    ret = png_get_cHRM(png_ptr, info_ptr, &g_white_x, &g_white_y, &g_red_x, &g_red_y, &g_green_x, &g_green_y, &g_blue_x, &g_blue_y);
    (void)ret;

    // Fuzz png_get_cHRM_fixed
    png_fixed_point g_fx, g_fy;
    ret = png_get_cHRM_fixed(png_ptr, info_ptr, &g_fx, &g_fy, &g_fx, &g_fy, &g_fx, &g_fy, &g_fx, &g_fy);
    (void)ret;

    // Fuzz png_get_cHRM_XYZ
    double g_red_X, g_red_Y, g_red_Z, g_green_X, g_green_Y, g_green_Z, g_blue_X, g_blue_Y, g_blue_Z;
    ret = png_get_cHRM_XYZ(png_ptr, info_ptr, &g_red_X, &g_red_Y, &g_red_Z, &g_green_X, &g_green_Y, &g_green_Z, &g_blue_X, &g_blue_Y, &g_blue_Z);
    (void)ret;

    // Fuzz png_get_cHRM_XYZ_fixed
    png_fixed_point g_xyz;
    ret = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &g_xyz, &g_xyz, &g_xyz, &g_xyz, &g_xyz, &g_xyz, &g_xyz, &g_xyz, &g_xyz);
    (void)ret;

    // Fuzz png_get_mDCV
    double g_max_lum, g_min_lum;
    ret = png_get_mDCV(png_ptr, info_ptr, &g_white_x, &g_white_y, &g_red_x, &g_red_y, &g_green_x, &g_green_y, &g_blue_x, &g_blue_y, &g_max_lum, &g_min_lum);
    (void)ret;

    // Fuzz png_get_mDCV_fixed
    png_uint_32 g_lum_scaled;
    ret = png_get_mDCV_fixed(png_ptr, info_ptr, &g_fx, &g_fy, &g_fx, &g_fy, &g_fx, &g_fy, &g_fx, &g_fy, &g_lum_scaled, &g_lum_scaled);
    (void)ret;

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}