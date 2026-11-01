// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cstdlib>

static void write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Create a dummy PNG file with the fuzzing data
    write_dummy_png(Data, Size, "./dummy_file");

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Skip signature check to handle malformed data more gracefully
    png_set_sig_bytes(png_ptr, 0);

    // Attempt to read the header and info
    png_read_info(png_ptr, info_ptr);

    // --- Test png_get_sCAL_fixed ---
    int unit;
    png_fixed_point width, height;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // --- Test png_get_mDCV_fixed ---
    png_fixed_point int_white_x, int_white_y;
    png_fixed_point int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y;
    png_fixed_point int_blue_x, int_blue_y;
    png_uint_32 max_lum, min_lum;
    png_get_mDCV_fixed(png_ptr, info_ptr,
        &int_white_x, &int_white_y,
        &int_red_x, &int_red_y,
        &int_green_x, &int_green_y,
        &int_blue_x, &int_blue_y,
        &max_lum, &min_lum);

    // --- Test png_set_mDCV_fixed ---
    // Use values derived from input to explore state space
    png_fixed_point set_white_x = (png_fixed_point)(Data[0] * 100000);
    png_fixed_point set_white_y = (png_fixed_point)(Data[1] * 100000);
    png_fixed_point set_red_x = (png_fixed_point)(Data[2] * 100000);
    png_fixed_point set_red_y = (png_fixed_point)(Data[3] * 100000);
    png_fixed_point set_green_x = (png_fixed_point)(Data[4] * 100000);
    png_fixed_point set_green_y = (png_fixed_point)(Data[5] * 100000);
    png_fixed_point set_blue_x = (png_fixed_point)(Data[6] * 100000);
    png_fixed_point set_blue_y = (png_fixed_point)(Data[7] * 100000);
    png_uint_32 set_max_lum = (Size > 8) ? (png_uint_32)(Data[8] << 16) : 0;
    png_uint_32 set_min_lum = (Size > 9) ? (png_uint_32)(Data[9] << 16) : 0;

    // Note: png_set_mDCV_fixed expects a writable info_ptr, but we are in read mode.
    // This might trigger an error or do nothing depending on internal state.
    // We wrap it in a setjmp to catch potential errors.
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_mDCV_fixed(png_ptr, info_ptr,
            set_white_x, set_white_y,
            set_red_x, set_red_y,
            set_green_x, set_green_y,
            set_blue_x, set_blue_y,
            set_max_lum, set_min_lum);
    }

    // --- Test png_set_cHRM_XYZ_fixed ---
    png_fixed_point red_X = (png_fixed_point)(Data[0] * 100000);
    png_fixed_point red_Y = (png_fixed_point)(Data[1] * 100000);
    png_fixed_point red_Z = (png_fixed_point)(Data[2] * 100000);
    png_fixed_point green_X = (png_fixed_point)(Data[3] * 100000);
    png_fixed_point green_Y = (png_fixed_point)(Data[4] * 100000);
    png_fixed_point green_Z = (png_fixed_point)(Data[5] * 100000);
    png_fixed_point blue_X = (png_fixed_point)(Data[6] * 100000);
    png_fixed_point blue_Y = (png_fixed_point)(Data[7] * 100000);
    png_fixed_point blue_Z = (png_fixed_point)(Data[8] * 100000);

    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
            red_X, red_Y, red_Z,
            green_X, green_Y, green_Z,
            blue_X, blue_Y, blue_Z);
    }

    // --- Test png_set_pHYs ---
    png_uint_32 res_x = (Size > 10) ? (png_uint_32)(Data[10] << 16) : 72;
    png_uint_32 res_y = (Size > 11) ? (png_uint_32)(Data[11] << 16) : 72;
    int unit_type = (Size > 12) ? (Data[12] % 2) : 1;

    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);
    }

    // --- Test png_get_cHRM_XYZ_fixed ---
    png_fixed_point get_red_X, get_red_Y, get_red_Z;
    png_fixed_point get_green_X, get_green_Y, get_green_Z;
    png_fixed_point get_blue_X, get_blue_Y, get_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
        &get_red_X, &get_red_Y, &get_red_Z,
        &get_green_X, &get_green_Y, &get_green_Z,
        &get_blue_X, &get_blue_Y, &get_blue_Z);

    // --- Test png_get_cICP ---
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr,
        &colour_primaries, &transfer_function,
        &matrix_coefficients, &video_full_range_flag);

    // --- Test png_get_oFFs ---
    png_int_32 offset_x, offset_y;
    int off_unit;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &off_unit);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}