// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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

static void fuzz_png_read_and_get(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    // Create a dummy file for libpng to read from
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    // Set up error handling to avoid crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Read the info
    png_read_info(png_ptr, info_ptr);

    // 1. png_get_mDCV_fixed
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_uint_32 mastering_display_maximum_luminance_scaled_by_10000;
    png_uint_32 mastering_display_minimum_luminance_scaled_by_10000;

    png_get_mDCV_fixed(png_ptr, info_ptr,
        &int_white_x, &int_white_y,
        &int_red_x, &int_red_y,
        &int_green_x, &int_green_y,
        &int_blue_x, &int_blue_y,
        &mastering_display_maximum_luminance_scaled_by_10000,
        &mastering_display_minimum_luminance_scaled_by_10000);

    // Test with NULL pointers (should be safe)
    png_get_mDCV_fixed(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 2. png_get_cHRM_XYZ_fixed
    png_fixed_point int_red_X, int_red_Y, int_red_Z;
    png_fixed_point int_green_X, int_green_Y, int_green_Z;
    png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;

    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
        &int_red_X, &int_red_Y, &int_red_Z,
        &int_green_X, &int_green_Y, &int_green_Z,
        &int_blue_X, &int_blue_Y, &int_blue_Z);

    // Test with NULL pointers
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 3. png_get_sCAL_fixed
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // Test with NULL pointers
    png_get_sCAL_fixed(png_ptr, info_ptr, NULL, NULL, NULL);

    // 4. png_get_IHDR
    png_uint_32 width_ihdr, height_ihdr;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr,
        &width_ihdr, &height_ihdr,
        &bit_depth, &color_type,
        &interlace_method, &compression_method, &filter_method);

    // Test with NULL pointers
    png_get_IHDR(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // 5. png_get_valid
    // Note: PNG_INFO_IHDR is not a standard flag for png_get_valid in some versions,
    // so we use common flags like PNG_INFO_mDCV, PNG_INFO_cHRM, PNG_INFO_sCAL, PNG_INFO_tRNS.
    png_get_valid(png_ptr, info_ptr, PNG_INFO_mDCV);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cHRM);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_sCAL);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA);
    png_get_valid(png_ptr, info_ptr, PNG_INFO_cHRM);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    // Remove dummy file
    remove(filename);
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    fuzz_png_read_and_get(Data, Size);
    return 0;
}