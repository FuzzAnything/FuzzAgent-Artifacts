// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
#include <png.h>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    (void)png_ptr;
}

static bool is_valid_cHRM_XYZ(double red_X, double red_Y, double red_Z,
                              double green_X, double green_Y, double green_Z,
                              double blue_X, double blue_Y, double blue_Z) {
    if (red_X < 0 || red_Y < 0 || red_Z < 0 ||
        green_X < 0 || green_Y < 0 || green_Z < 0 ||
        blue_X < 0 || blue_Y < 0 || blue_Z < 0) {
        return false;
    }
    if (red_X > 1.0 || red_Y > 1.0 || red_Z > 1.0 ||
        green_X > 1.0 || green_Y > 1.0 || green_Z > 1.0 ||
        blue_X > 1.0 || blue_Y > 1.0 || blue_Z > 1.0) {
        return false;
    }
    return true;
}

static bool is_valid_cHRM(double white_x, double white_y,
                          double red_x, double red_y,
                          double green_x, double green_y,
                          double blue_x, double blue_y) {
    if (white_x < 0 || white_y < 0 || white_x > 1.0 || white_y > 1.0) return false;
    if (red_x < 0 || red_y < 0 || red_x > 1.0 || red_y > 1.0) return false;
    if (green_x < 0 || green_y < 0 || green_x > 1.0 || green_y > 1.0) return false;
    if (blue_x < 0 || blue_y < 0 || blue_x > 1.0 || blue_y > 1.0) return false;
    return true;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    png_infop end_info = png_create_info_struct(png_ptr);
    if (!end_info) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
        remove(dummy_file);
        return 0;
    }

    png_set_read_fn(png_ptr, NULL, dummy_read_fn);

    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    double file_gamma;
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z;
    png_fixed_point int_file_gamma;
    png_uint_16* hist = NULL;
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;

    png_uint_32 valid_flags[] = {
        PNG_INFO_tRNS, PNG_INFO_cHRM, PNG_INFO_gAMA, PNG_INFO_hIST, PNG_INFO_cICP
    };
    for (size_t i = 0; i < sizeof(valid_flags) / sizeof(valid_flags[0]); ++i) {
        png_get_valid(png_ptr, info_ptr, valid_flags[i]);
    }

    png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z, &int_green_X, &int_green_Y, &int_green_Z, &int_blue_X, &int_blue_Y, &int_blue_Z);

    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y, &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);

    png_get_gAMA(png_ptr, info_ptr, &file_gamma);
    png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);

    png_get_hIST(png_ptr, info_ptr, &hist);

    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);

    double fuzz_white_x = (Data[0] / 255.0);
    double fuzz_white_y = (Data[1] / 255.0);
    double fuzz_red_x = (Data[2] / 255.0);
    double fuzz_red_y = (Data[3] / 255.0);
    double fuzz_green_x = (Data[4] / 255.0);
    double fuzz_green_y = (Data[5] / 255.0);
    double fuzz_blue_x = (Data[6] / 255.0);
    double fuzz_blue_y = (Data[7] / 255.0);

    double fuzz_red_X = (Data[8] / 255.0);
    double fuzz_red_Y = (Data[9] / 255.0);
    double fuzz_red_Z = (Data[10] / 255.0);
    double fuzz_green_X = (Data[11] / 255.0);
    double fuzz_green_Y = (Data[12] / 255.0);
    double fuzz_green_Z = (Data[13] / 255.0);
    double fuzz_blue_X = (Data[14] / 255.0);
    double fuzz_blue_Y = (Data[15] / 255.0);
    double fuzz_blue_Z = (Data[16] / 255.0);

    double fuzz_file_gamma = (Data[17] / 255.0);

    if (is_valid_cHRM(fuzz_white_x, fuzz_white_y, fuzz_red_x, fuzz_red_y, fuzz_green_x, fuzz_green_y, fuzz_blue_x, fuzz_blue_y)) {
        png_set_cHRM(png_ptr, info_ptr, fuzz_white_x, fuzz_white_y, fuzz_red_x, fuzz_red_y, fuzz_green_x, fuzz_green_y, fuzz_blue_x, fuzz_blue_y);
    }

    if (is_valid_cHRM_XYZ(fuzz_red_X, fuzz_red_Y, fuzz_red_Z, fuzz_green_X, fuzz_green_Y, fuzz_green_Z, fuzz_blue_X, fuzz_blue_Y, fuzz_blue_Z)) {
        png_set_cHRM_XYZ(png_ptr, info_ptr, fuzz_red_X, fuzz_red_Y, fuzz_red_Z, fuzz_green_X, fuzz_green_Y, fuzz_green_Z, fuzz_blue_X, fuzz_blue_Y, fuzz_blue_Z);
    }

    if (is_valid_cHRM(fuzz_white_x, fuzz_white_y, fuzz_red_x, fuzz_red_y, fuzz_green_x, fuzz_green_y, fuzz_blue_x, fuzz_blue_y)) {
        png_set_cHRM_fixed(png_ptr, info_ptr, (png_fixed_point)(fuzz_white_x * 100000), (png_fixed_point)(fuzz_white_y * 100000),
                           (png_fixed_point)(fuzz_red_x * 100000), (png_fixed_point)(fuzz_red_y * 100000),
                           (png_fixed_point)(fuzz_green_x * 100000), (png_fixed_point)(fuzz_green_y * 100000),
                           (png_fixed_point)(fuzz_blue_x * 100000), (png_fixed_point)(fuzz_blue_y * 100000));
    }

    if (is_valid_cHRM_XYZ(fuzz_red_X, fuzz_red_Y, fuzz_red_Z, fuzz_green_X, fuzz_green_Y, fuzz_green_Z, fuzz_blue_X, fuzz_blue_Y, fuzz_blue_Z)) {
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, (png_fixed_point)(fuzz_red_X * 100000), (png_fixed_point)(fuzz_red_Y * 100000), (png_fixed_point)(fuzz_red_Z * 100000),
                               (png_fixed_point)(fuzz_green_X * 100000), (png_fixed_point)(fuzz_green_Y * 100000), (png_fixed_point)(fuzz_green_Z * 100000),
                               (png_fixed_point)(fuzz_blue_X * 100000), (png_fixed_point)(fuzz_blue_Y * 100000), (png_fixed_point)(fuzz_blue_Z * 100000));
    }

    png_set_gAMA(png_ptr, info_ptr, fuzz_file_gamma);
    png_set_gAMA_fixed(png_ptr, info_ptr, (png_fixed_point)(fuzz_file_gamma * 100000));

    if (Size >= 276) {
        png_uint_16 hist_data[256];
        for (int i = 0; i < 256; ++i) {
            hist_data[i] = (png_uint_16)(Data[20 + i] * 255);
        }
        png_set_hIST(png_ptr, info_ptr, hist_data);
    }

    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    remove(dummy_file);

    return 0;
}