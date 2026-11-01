// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
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

// Custom error handling to prevent libpng from calling abort
static void
fuzz_png_error_ptr(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Suppress error to allow fuzzing to continue
}

static void
fuzz_png_warning_ptr(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

static void
fuzz_png_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void
fuzz_png_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to satisfy libpng's requirement for a file handle if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom error/warning handlers to prevent crashes
    png_set_error_fn(png_ptr, NULL, fuzz_png_error_ptr, fuzz_png_warning_ptr);

    // Create a write struct to test the set functions properly
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!write_png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_png_ptr, NULL);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_set_error_fn(write_png_ptr, NULL, fuzz_png_error_ptr, fuzz_png_warning_ptr);

    // Initialize the write info structure
    // We pass NULL for the file pointer as we are not actually writing to a file
    png_init_io(write_png_ptr, NULL);

    // Test 1: png_set_sRGB_gAMA_and_cHRM
    int srgb_intent = (Size > 0) ? (Data[0] % 4) : 0;
    png_set_sRGB_gAMA_and_cHRM(write_png_ptr, write_info_ptr, srgb_intent);

    // Test 2: png_set_cHRM
    double white_x = (Size > 1) ? (double)(Data[1] % 256) / 255.0 : 0.3127;
    double white_y = (Size > 2) ? (double)(Data[2] % 256) / 255.0 : 0.3290;
    double red_x = (Size > 3) ? (double)(Data[3] % 256) / 255.0 : 0.6400;
    double red_y = (Size > 4) ? (double)(Data[4] % 256) / 255.0 : 0.3300;
    double green_x = (Size > 5) ? (double)(Data[5] % 256) / 255.0 : 0.3000;
    double green_y = (Size > 6) ? (double)(Data[6] % 256) / 255.0 : 0.6000;
    double blue_x = (Size > 7) ? (double)(Data[7] % 256) / 255.0 : 0.1500;
    double blue_y = (Size > 8) ? (double)(Data[8] % 256) / 255.0 : 0.0600;

    png_set_cHRM(write_png_ptr, write_info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    // Test 3: png_set_cHRM_XYZ
    double red_X = (Size > 9) ? (double)(Data[9] % 256) / 255.0 : 0.4124;
    double red_Y = (Size > 10) ? (double)(Data[10] % 256) / 255.0 : 0.2126;
    double red_Z = (Size > 11) ? (double)(Data[11] % 256) / 255.0 : 0.0193;
    double green_X = (Size > 12) ? (double)(Data[12] % 256) / 255.0 : 0.3576;
    double green_Y = (Size > 13) ? (double)(Data[13] % 256) / 255.0 : 0.7152;
    double green_Z = (Size > 14) ? (double)(Data[14] % 256) / 255.0 : 0.1192;
    double blue_X = (Size > 15) ? (double)(Data[15] % 256) / 255.0 : 0.1805;
    double blue_Y = (Size > 16) ? (double)(Data[16] % 256) / 255.0 : 0.0722;
    double blue_Z = (Size > 17) ? (double)(Data[17] % 256) / 255.0 : 0.9505;

    png_set_cHRM_XYZ(write_png_ptr, write_info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // Test 4: png_set_cHRM_fixed
    png_fixed_point int_white_x = (Size > 18) ? (png_fixed_point)(Data[18] * 10000) : 31270;
    png_fixed_point int_white_y = (Size > 19) ? (png_fixed_point)(Data[19] * 10000) : 32900;
    png_fixed_point int_red_x = (Size > 20) ? (png_fixed_point)(Data[20] * 10000) : 64000;
    png_fixed_point int_red_y = (Size > 21) ? (png_fixed_point)(Data[21] * 10000) : 33000;
    png_fixed_point int_green_x = (Size > 22) ? (png_fixed_point)(Data[22] * 10000) : 30000;
    png_fixed_point int_green_y = (Size > 23) ? (png_fixed_point)(Data[23] * 10000) : 60000;
    png_fixed_point int_blue_x = (Size > 24) ? (png_fixed_point)(Data[24] * 10000) : 15000;
    png_fixed_point int_blue_y = (Size > 25) ? (png_fixed_point)(Data[25] * 10000) : 6000;

    png_set_cHRM_fixed(write_png_ptr, write_info_ptr, int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y);

    // Test 5: png_set_cHRM_XYZ_fixed
    png_fixed_point int_red_X = (Size > 26) ? (png_fixed_point)(Data[26] * 10000) : 41240;
    png_fixed_point int_red_Y = (Size > 27) ? (png_fixed_point)(Data[27] * 10000) : 21260;
    png_fixed_point int_red_Z = (Size > 28) ? (png_fixed_point)(Data[28] * 10000) : 1930;
    png_fixed_point int_green_X = (Size > 29) ? (png_fixed_point)(Data[29] * 10000) : 35760;
    png_fixed_point int_green_Y = (Size > 30) ? (png_fixed_point)(Data[30] * 10000) : 71520;
    png_fixed_point int_green_Z = (Size > 31) ? (png_fixed_point)(Data[31] * 10000) : 11920;
    png_fixed_point int_blue_X = (Size > 32) ? (png_fixed_point)(Data[32] * 10000) : 18050;
    png_fixed_point int_blue_Y = (Size > 33) ? (png_fixed_point)(Data[33] * 10000) : 7220;
    png_fixed_point int_blue_Z = (Size > 34) ? (png_fixed_point)(Data[34] * 10000) : 95050;

    png_set_cHRM_XYZ_fixed(write_png_ptr, write_info_ptr, int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z);

    // Test 6: png_get_cHRM
    double get_white_x, get_white_y, get_red_x, get_red_y, get_green_x, get_green_y, get_blue_x, get_blue_y;
    png_uint_32 result = png_get_cHRM(write_png_ptr, write_info_ptr, &get_white_x, &get_white_y, &get_red_x, &get_red_y, &get_green_x, &get_green_y, &get_blue_x, &get_blue_y);
    (void)result;

    // Test 7: png_get_cHRM_XYZ
    double get_red_X, get_red_Y, get_red_Z, get_green_X, get_green_Y, get_green_Z, get_blue_X, get_blue_Y, get_blue_Z;
    result = png_get_cHRM_XYZ(write_png_ptr, write_info_ptr, &get_red_X, &get_red_Y, &get_red_Z, &get_green_X, &get_green_Y, &get_green_Z, &get_blue_X, &get_blue_Y, &get_blue_Z);
    (void)result;

    // Test 8: png_get_cHRM_fixed
    png_fixed_point get_int_white_x, get_int_white_y, get_int_red_x, get_int_red_y, get_int_green_x, get_int_green_y, get_int_blue_x, get_int_blue_y;
    result = png_get_cHRM_fixed(write_png_ptr, write_info_ptr, &get_int_white_x, &get_int_white_y, &get_int_red_x, &get_int_red_y, &get_int_green_x, &get_int_green_y, &get_int_blue_x, &get_int_blue_y);
    (void)result;

    // Test 9: png_get_cHRM_XYZ_fixed
    png_fixed_point get_int_red_X, get_int_red_Y, get_int_red_Z, get_int_green_X, get_int_green_Y, get_int_green_Z, get_int_blue_X, get_int_blue_Y, get_int_blue_Z;
    result = png_get_cHRM_XYZ_fixed(write_png_ptr, write_info_ptr, &get_int_red_X, &get_int_red_Y, &get_int_red_Z, &get_int_green_X, &get_int_green_Y, &get_int_green_Z, &get_int_blue_X, &get_int_blue_Y, &get_int_blue_Z);
    (void)result;

    // Test 10: png_set_gAMA
    double file_gamma = (Size > 35) ? (double)(Data[35] % 256) / 255.0 : 2.2;
    png_set_gAMA(write_png_ptr, write_info_ptr, file_gamma);

    // Test 11: png_get_gAMA
    double retrieved_gamma;
    result = png_get_gAMA(write_png_ptr, write_info_ptr, &retrieved_gamma);
    (void)result;

    // Test 12: png_set_gAMA_fixed
    png_fixed_point int_file_gamma = (Size > 36) ? (png_fixed_point)(Data[36] * 10000) : 22000;
    png_set_gAMA_fixed(write_png_ptr, write_info_ptr, int_file_gamma);

    // Test 13: png_get_gAMA_fixed
    png_fixed_point retrieved_int_gamma;
    result = png_get_gAMA_fixed(write_png_ptr, write_info_ptr, &retrieved_int_gamma);
    (void)result;

    // Test 14: png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    result = png_get_cICP(write_png_ptr, write_info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);
    (void)result;

    // Cleanup
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Remove the dummy file
    remove(dummy_file);

    return 0;
}