// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstring>

static void dummy_read_fn(png_structp png_ptr, png_bytep outBytes, png_size_t byteCountToRead) {
    // Dummy read function that does nothing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function that does nothing
}

extern "C" int LLVMFuzzerTestOneInput_340(const uint8_t *Data, size_t Size) {
    // Create dummy PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
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
    
    // Set up dummy I/O
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    
    // Extract values from fuzzer input for various functions
    // We'll use the input data to populate parameters for the API calls
    
    // For functions that need png_byte parameters (cICP)
    png_byte colour_primaries = 0;
    png_byte transfer_function = 0;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;
    
    if (Size >= 4) {
        colour_primaries = Data[0];
        transfer_function = Data[1];
        matrix_coefficients = Data[2];
        video_full_range_flag = Data[3];
    }
    
    // For functions that need double parameters
    double d_red_X = 0.0, d_red_Y = 0.0, d_red_Z = 0.0;
    double d_green_X = 0.0, d_green_Y = 0.0, d_green_Z = 0.0;
    double d_blue_X = 0.0, d_blue_Y = 0.0, d_blue_Z = 0.0;
    double d_white_x = 0.0, d_white_y = 0.0;
    double d_red_x = 0.0, d_red_y = 0.0;
    double d_green_x = 0.0, d_green_y = 0.0;
    double d_blue_x = 0.0, d_blue_y = 0.0;
    
    // Extract doubles from input if we have enough data
    if (Size >= 9 * sizeof(double)) {
        size_t offset = 0;
        memcpy(&d_red_X, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_red_Y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_red_Z, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_green_X, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_green_Y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_green_Z, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_blue_X, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_blue_Y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_blue_Z, Data + offset, sizeof(double));
    }
    
    if (Size >= 8 * sizeof(double)) {
        size_t offset = 0;
        memcpy(&d_white_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_white_y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_red_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_red_y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_green_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_green_y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_blue_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&d_blue_y, Data + offset, sizeof(double));
    }
    
    // For functions that need png_fixed_point parameters
    png_fixed_point fp_red_X = 0, fp_red_Y = 0, fp_red_Z = 0;
    png_fixed_point fp_green_X = 0, fp_green_Y = 0, fp_green_Z = 0;
    png_fixed_point fp_blue_X = 0, fp_blue_Y = 0, fp_blue_Z = 0;
    png_fixed_point fp_white_x = 0, fp_white_y = 0;
    png_fixed_point fp_red_x = 0, fp_red_y = 0;
    png_fixed_point fp_green_x = 0, fp_green_y = 0;
    png_fixed_point fp_blue_x = 0, fp_blue_y = 0;
    
    // Extract fixed points from input if we have enough data
    if (Size >= 9 * sizeof(png_fixed_point)) {
        size_t offset = 0;
        memcpy(&fp_red_X, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_red_Y, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_red_Z, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_green_X, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_green_Y, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_green_Z, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_blue_X, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_blue_Y, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_blue_Z, Data + offset, sizeof(png_fixed_point));
    }
    
    if (Size >= 8 * sizeof(png_fixed_point)) {
        size_t offset = 0;
        memcpy(&fp_white_x, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_white_y, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_red_x, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_red_y, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_green_x, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_green_y, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_blue_x, Data + offset, sizeof(png_fixed_point)); offset += sizeof(png_fixed_point);
        memcpy(&fp_blue_y, Data + offset, sizeof(png_fixed_point));
    }
    
    // Call target API functions with various combinations of parameters
    
    // 1. png_get_cHRM_XYZ (floating point version)
    double out_red_X, out_red_Y, out_red_Z;
    double out_green_X, out_green_Y, out_green_Z;
    double out_blue_X, out_blue_Y, out_blue_Z;
    png_uint_32 result = png_get_cHRM_XYZ(png_ptr, info_ptr,
                                         &out_red_X, &out_red_Y, &out_red_Z,
                                         &out_green_X, &out_green_Y, &out_green_Z,
                                         &out_blue_X, &out_blue_Y, &out_blue_Z);
    
    // 2. png_set_cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                          fp_red_X, fp_red_Y, fp_red_Z,
                          fp_green_X, fp_green_Y, fp_green_Z,
                          fp_blue_X, fp_blue_Y, fp_blue_Z);
    
    // 3. png_set_cHRM_XYZ (floating point version)
    png_set_cHRM_XYZ(png_ptr, info_ptr,
                    d_red_X, d_red_Y, d_red_Z,
                    d_green_X, d_green_Y, d_green_Z,
                    d_blue_X, d_blue_Y, d_blue_Z);
    
    // 4. png_get_cHRM_fixed
    png_fixed_point out_white_x, out_white_y;
    png_fixed_point out_red_x, out_red_y;
    png_fixed_point out_green_x, out_green_y;
    png_fixed_point out_blue_x, out_blue_y;
    result = png_get_cHRM_fixed(png_ptr, info_ptr,
                               &out_white_x, &out_white_y,
                               &out_red_x, &out_red_y,
                               &out_green_x, &out_green_y,
                               &out_blue_x, &out_blue_y);
    
    // 5. png_set_cICP
    png_set_cICP(png_ptr, info_ptr,
                colour_primaries, transfer_function,
                matrix_coefficients, video_full_range_flag);
    
    // 6. png_get_cHRM (floating point version)
    double out_d_white_x, out_d_white_y;
    double out_d_red_x, out_d_red_y;
    double out_d_green_x, out_d_green_y;
    double out_d_blue_x, out_d_blue_y;
    result = png_get_cHRM(png_ptr, info_ptr,
                         &out_d_white_x, &out_d_white_y,
                         &out_d_red_x, &out_d_red_y,
                         &out_d_green_x, &out_d_green_y,
                         &out_d_blue_x, &out_d_blue_y);
    
    // Also test png_get_cHRM_XYZ_fixed
    png_fixed_point out_fp_red_X, out_fp_red_Y, out_fp_red_Z;
    png_fixed_point out_fp_green_X, out_fp_green_Y, out_fp_green_Z;
    png_fixed_point out_fp_blue_X, out_fp_blue_Y, out_fp_blue_Z;
    result = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                   &out_fp_red_X, &out_fp_red_Y, &out_fp_red_Z,
                                   &out_fp_green_X, &out_fp_green_Y, &out_fp_green_Z,
                                   &out_fp_blue_X, &out_fp_blue_Y, &out_fp_blue_Z);
    
    // Test png_get_cICP
    png_byte out_colour_primaries, out_transfer_function;
    png_byte out_matrix_coefficients, out_video_full_range_flag;
    result = png_get_cICP(png_ptr, info_ptr,
                         &out_colour_primaries, &out_transfer_function,
                         &out_matrix_coefficients, &out_video_full_range_flag);
    
    // Test png_set_cHRM (floating point version)
    png_set_cHRM(png_ptr, info_ptr,
                d_white_x, d_white_y,
                d_red_x, d_red_y,
                d_green_x, d_green_y,
                d_blue_x, d_blue_y);
    
    // Test png_set_cHRM_fixed
    png_set_cHRM_fixed(png_ptr, info_ptr,
                      fp_white_x, fp_white_y,
                      fp_red_x, fp_red_y,
                      fp_green_x, fp_green_y,
                      fp_blue_x, fp_blue_y);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}