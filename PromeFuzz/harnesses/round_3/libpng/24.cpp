// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy read function
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy write function
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing - dummy flush function
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create dummy PNG file with fuzzer data
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                 dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize IO
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Test png_get_cHRM
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 result_cHRM = png_get_cHRM(png_ptr, info_ptr,
                                           &white_x, &white_y,
                                           &red_x, &red_y,
                                           &green_x, &green_y,
                                           &blue_x, &blue_y);

    // Test png_get_cHRM_XYZ
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_uint_32 result_cHRM_XYZ = png_get_cHRM_XYZ(png_ptr, info_ptr,
                                                   &red_X, &red_Y, &red_Z,
                                                   &green_X, &green_Y, &green_Z,
                                                   &blue_X, &blue_Y, &blue_Z);

    // Test png_get_cHRM_fixed
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_uint_32 result_cHRM_fixed = png_get_cHRM_fixed(png_ptr, info_ptr,
                                                       &int_white_x, &int_white_y,
                                                       &int_red_x, &int_red_y,
                                                       &int_green_x, &int_green_y,
                                                       &int_blue_x, &int_blue_y);

    // Test png_get_cHRM_XYZ_fixed
    png_fixed_point int_red_X, int_red_Y, int_red_Z;
    png_fixed_point int_green_X, int_green_Y, int_green_Z;
    png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
    png_uint_32 result_cHRM_XYZ_fixed = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                                               &int_red_X, &int_red_Y, &int_red_Z,
                                                               &int_green_X, &int_green_Y, &int_green_Z,
                                                               &int_blue_X, &int_blue_Y, &int_blue_Z);

    // Test png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_uint_32 result_cICP = png_get_cICP(png_ptr, info_ptr,
                                           &colour_primaries, &transfer_function,
                                           &matrix_coefficients, &video_full_range_flag);

    // Test png_get_gAMA
    double file_gamma;
    png_uint_32 result_gAMA = png_get_gAMA(png_ptr, info_ptr, &file_gamma);

    // Test png_get_gAMA_fixed
    png_fixed_point int_file_gamma;
    png_uint_32 result_gAMA_fixed = png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);

    // Test png_get_hIST
    png_uint_16p hist;
    png_uint_32 result_hIST = png_get_hIST(png_ptr, info_ptr, &hist);

    // Clean up read structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    // Now test setter functions with a write structure
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                                    dummy_error_fn, dummy_warning_fn);
    if (!write_ptr) {
        return 0;
    }

    png_infop write_info_ptr = png_create_info_struct(write_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }

    // Set up error handling for write structure
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_write_struct(&write_ptr, &write_info_ptr);
        return 0;
    }

    // Initialize write IO
    png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);

    // Use fuzzer data to generate parameter values
    // We'll use the first bytes of data to generate values
    size_t data_index = 0;
    
    // Generate double values from fuzzer data
    double d1 = 0.0, d2 = 0.0, d3 = 0.0, d4 = 0.0, d5 = 0.0, d6 = 0.0, d7 = 0.0, d8 = 0.0;
    if (Size >= 8 * sizeof(double)) {
        memcpy(&d1, Data + data_index, sizeof(double)); data_index += sizeof(double);
        memcpy(&d2, Data + data_index, sizeof(double)); data_index += sizeof(double);
        memcpy(&d3, Data + data_index, sizeof(double)); data_index += sizeof(double);
        memcpy(&d4, Data + data_index, sizeof(double)); data_index += sizeof(double);
        memcpy(&d5, Data + data_index, sizeof(double)); data_index += sizeof(double);
        memcpy(&d6, Data + data_index, sizeof(double)); data_index += sizeof(double);
        memcpy(&d7, Data + data_index, sizeof(double)); data_index += sizeof(double);
        memcpy(&d8, Data + data_index, sizeof(double)); data_index += sizeof(double);
    } else {
        // Use simple values if not enough data
        d1 = d2 = d3 = d4 = d5 = d6 = d7 = d8 = 0.5;
    }

    // Test png_set_cHRM
    png_set_cHRM(write_ptr, write_info_ptr,
                 d1, d2,  // white_x, white_y
                 d3, d4,  // red_x, red_y
                 d5, d6,  // green_x, green_y
                 d7, d8); // blue_x, blue_y

    // Test png_set_cHRM_XYZ
    png_set_cHRM_XYZ(write_ptr, write_info_ptr,
                     d1, d2, d3,    // red_X, red_Y, red_Z
                     d4, d5, d6,    // green_X, green_Y, green_Z
                     d7, d8, d1);   // blue_X, blue_Y, blue_Z

    // Generate fixed point values
    png_fixed_point f1 = 50000, f2 = 50000, f3 = 50000, f4 = 50000;
    png_fixed_point f5 = 50000, f6 = 50000, f7 = 50000, f8 = 50000;
    
    // Test png_set_cHRM_fixed
    png_set_cHRM_fixed(write_ptr, write_info_ptr,
                       f1, f2,  // int_white_x, int_white_y
                       f3, f4,  // int_red_x, int_red_y
                       f5, f6,  // int_green_x, int_green_y
                       f7, f8); // int_blue_x, int_blue_y

    // Test png_set_cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(write_ptr, write_info_ptr,
                           f1, f2, f3,    // int_red_X, int_red_Y, int_red_Z
                           f4, f5, f6,    // int_green_X, int_green_Y, int_green_Z
                           f7, f8, f1);   // int_blue_X, int_blue_Y, int_blue_Z

    // Test png_set_sRGB_gAMA_and_cHRM
    // Use first byte modulo 4 for srgb_intent (valid values: 0-3)
    int srgb_intent = Data[0] % 4;
    png_set_sRGB_gAMA_and_cHRM(write_ptr, write_info_ptr, srgb_intent);

    // Test png_set_gAMA
    png_set_gAMA(write_ptr, write_info_ptr, d1);

    // Test png_set_gAMA_fixed
    png_set_gAMA_fixed(write_ptr, write_info_ptr, f1);

    // Clean up write structures
    png_destroy_write_struct(&write_ptr, &write_info_ptr);

    return 0;
}