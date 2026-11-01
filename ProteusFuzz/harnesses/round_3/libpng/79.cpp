// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <png.h>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_infop end_info = NULL;

    // Initialize PNG structures
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Setup error handling to prevent aborts during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }

    // Initialize dummy file and write fuzzing data to it
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Reopen for reading
    f = fopen("./dummy_file", "rb");
    if (!f) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        return 0;
    }

    png_init_io(png_ptr, f);

    // Read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Fuzzing logic for target APIs

    // 1. png_get_cHRM_fixed
    png_fixed_point wx, wy, rx, ry, gx, gy, bx, by;
    png_get_cHRM_fixed(png_ptr, info_ptr, &wx, &wy, &rx, &ry, &gx, &gy, &bx, &by);
    png_get_cHRM_fixed(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 2. png_get_cHRM_XYZ_fixed
    png_fixed_point rX, rY, rZ, gX, gY, gZ, bX, bY, bZ;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &rX, &rY, &rZ, &gX, &gY, &gZ, &bX, &bY, &bZ);
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 3. png_set_cHRM_XYZ_fixed (Set some values to test write path)
    png_fixed_point set_rX = (png_fixed_point)(Data[0] * 100000);
    png_fixed_point set_rY = (png_fixed_point)(Data[1] * 100000);
    png_fixed_point set_rZ = (png_fixed_point)(Data[2] * 100000);
    png_fixed_point set_gX = (png_fixed_point)(Data[3] * 100000);
    png_fixed_point set_gY = (png_fixed_point)(Data[4] * 100000);
    png_fixed_point set_gZ = (png_fixed_point)(Data[5] * 100000);
    png_fixed_point set_bX = (png_fixed_point)(Data[6] * 100000);
    png_fixed_point set_bY = (png_fixed_point)(Data[7] * 100000);
    png_fixed_point set_bZ = (png_fixed_point)(Data[8] * 100000);
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, set_rX, set_rY, set_rZ, set_gX, set_gY, set_gZ, set_bX, set_bY, set_bZ);

    // 4. png_set_mDCV_fixed
    png_fixed_point md_white_x = (png_fixed_point)(Data[9] * 100000);
    png_fixed_point md_white_y = (png_fixed_point)(Data[10] * 100000);
    png_fixed_point md_red_x = (png_fixed_point)(Data[11] * 100000);
    png_fixed_point md_red_y = (png_fixed_point)(Data[12] * 100000);
    png_fixed_point md_green_x = (png_fixed_point)(Data[13] * 100000);
    png_fixed_point md_green_y = (png_fixed_point)(Data[14] * 100000);
    png_fixed_point md_blue_x = (png_fixed_point)(Data[15] * 100000);
    png_fixed_point md_blue_y = (png_fixed_point)(Data[16] * 100000);
    png_uint_32 max_lum = (png_uint_32)(Data[17] * 10000);
    png_uint_32 min_lum = (png_uint_32)(Data[18] * 10000);
    png_set_mDCV_fixed(png_ptr, info_ptr, md_white_x, md_white_y, md_red_x, md_red_y,
                       md_green_x, md_green_y, md_blue_x, md_blue_y, max_lum, min_lum);

    // 5. png_get_sCAL_fixed
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
    png_get_sCAL_fixed(png_ptr, info_ptr, NULL, NULL, NULL);

    // 6. png_get_mDCV_fixed
    png_fixed_point g_md_white_x, g_md_white_y, g_md_red_x, g_md_red_y, g_md_green_x, g_md_green_y, g_md_blue_x, g_md_blue_y;
    png_uint_32 g_max_lum, g_min_lum;
    png_get_mDCV_fixed(png_ptr, info_ptr, &g_md_white_x, &g_md_white_y, &g_md_red_x, &g_md_red_y,
                       &g_md_green_x, &g_md_green_y, &g_md_blue_x, &g_md_blue_y, &g_max_lum, &g_min_lum);
    png_get_mDCV_fixed(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // Cleanup
    fclose(f);
    remove("./dummy_file");

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    return 0;
}