// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
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
#include <setjmp.h>

static void fuzz_png_mDCV_and_pHYs(png_structp png_ptr, png_infop info_ptr,
                                   const uint8_t *data, size_t size) {
    // Minimum size required: 8 fixed_points (32 bytes) + 2 uint32 (8 bytes) + 2 uint32 (8 bytes) + 1 int (4 bytes) = 52 bytes
    if (size < 52) return;

    // Extract values from fuzz data safely
    png_fixed_point white_x = *(const png_fixed_point*)(data);
    png_fixed_point white_y = *(const png_fixed_point*)(data + 4);
    png_fixed_point red_x = *(const png_fixed_point*)(data + 8);
    png_fixed_point red_y = *(const png_fixed_point*)(data + 12);
    png_fixed_point green_x = *(const png_fixed_point*)(data + 16);
    png_fixed_point green_y = *(const png_fixed_point*)(data + 20);
    png_fixed_point blue_x = *(const png_fixed_point*)(data + 24);
    png_fixed_point blue_y = *(const png_fixed_point*)(data + 28);
    
    png_uint_32 max_lum = *(const png_uint_32*)(data + 32);
    png_uint_32 min_lum = *(const png_uint_32*)(data + 36);
    
    png_uint_32 res_x = *(const png_uint_32*)(data + 40);
    png_uint_32 res_y = *(const png_uint_32*)(data + 44);
    int unit_type = data[48] % 2; // 0 or 1

    // Set mDCV
    png_set_mDCV_fixed(png_ptr, info_ptr,
                       white_x, white_y,
                       red_x, red_y,
                       green_x, green_y,
                       blue_x, blue_y,
                       max_lum, min_lum);

    // Set pHYs
    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);

    // Get mDCV back
    png_fixed_point g_white_x, g_white_y, g_red_x, g_red_y;
    png_fixed_point g_green_x, g_green_y, g_blue_x, g_blue_y;
    png_uint_32 g_max_lum, g_min_lum;

    png_get_mDCV_fixed(png_ptr, info_ptr,
                       &g_white_x, &g_white_y,
                       &g_red_x, &g_red_y,
                       &g_green_x, &g_green_y,
                       &g_blue_x, &g_blue_y,
                       &g_max_lum, &g_min_lum);

    // Test with NULL pointers (should be safe)
    png_get_mDCV_fixed(png_ptr, info_ptr,
                       NULL, NULL,
                       NULL, NULL,
                       NULL, NULL,
                       NULL, NULL,
                       NULL, NULL);
}

static void fuzz_png_cHRM_and_cHRM_XYZ(png_structp png_ptr, png_infop info_ptr,
                                       const uint8_t *data, size_t size) {
    // Minimum size required: 9 fixed_points = 36 bytes
    if (size < 36) return;

    // Extract XYZ values
    png_fixed_point red_X = *(const png_fixed_point*)(data);
    png_fixed_point red_Y = *(const png_fixed_point*)(data + 4);
    png_fixed_point red_Z = *(const png_fixed_point*)(data + 8);
    png_fixed_point green_X = *(const png_fixed_point*)(data + 12);
    png_fixed_point green_Y = *(const png_fixed_point*)(data + 16);
    png_fixed_point green_Z = *(const png_fixed_point*)(data + 20);
    png_fixed_point blue_X = *(const png_fixed_point*)(data + 24);
    png_fixed_point blue_Y = *(const png_fixed_point*)(data + 28);
    png_fixed_point blue_Z = *(const png_fixed_point*)(data + 32);

    // Set cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                           red_X, red_Y, red_Z,
                           green_X, green_Y, green_Z,
                           blue_X, blue_Y, blue_Z);

    // Get cHRM_XYZ_fixed
    png_fixed_point g_red_X, g_red_Y, g_red_Z;
    png_fixed_point g_green_X, g_green_Y, g_green_Z;
    png_fixed_point g_blue_X, g_blue_Y, g_blue_Z;

    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                           &g_red_X, &g_red_Y, &g_red_Z,
                           &g_green_X, &g_green_Y, &g_green_Z,
                           &g_blue_X, &g_blue_Y, &g_blue_Z);

    // Test with NULL pointers
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                           NULL, NULL, NULL,
                           NULL, NULL, NULL,
                           NULL, NULL, NULL);
}

static void fuzz_png_sCAL(png_structp png_ptr, png_infop info_ptr,
                          const uint8_t *data, size_t size) {
    // Minimum size required: 1 int + 2 fixed_points = 4 + 8 = 12 bytes
    if (size < 12) return;

    int unit = data[0] % 2; // 0 or 1
    png_fixed_point width = *(const png_fixed_point*)(data + 4);
    png_fixed_point height = *(const png_fixed_point*)(data + 8);

    // Note: png_set_sCAL_fixed is not in the target list, so we only test get
    // We assume the chunk might be set by other means or we test get on potentially empty info
    png_fixed_point g_width, g_height;
    int g_unit;

    png_get_sCAL_fixed(png_ptr, info_ptr, &g_unit, &g_width, &g_height);

    // Test with NULL pointers
    png_get_sCAL_fixed(png_ptr, info_ptr, NULL, NULL, NULL);
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // We don't actually read a file, but we can still test the set/get functions
    // on the info structure without a real PNG file.
    // Some functions might require specific flags or states, but we test them anyway.

    // Fuzz mDCV and pHYs
    fuzz_png_mDCV_and_pHYs(png_ptr, info_ptr, Data, Size);

    // Fuzz cHRM and cHRM_XYZ
    fuzz_png_cHRM_and_cHRM_XYZ(png_ptr, info_ptr, Data, Size);

    // Fuzz sCAL
    fuzz_png_sCAL(png_ptr, info_ptr, Data, Size);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}