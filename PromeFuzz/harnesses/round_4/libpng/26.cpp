// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
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

#include "png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to read as PNG file
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Create read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Initialize IO
    png_init_io(png_ptr, fp);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Test png_set_sRGB_gAMA_and_cHRM
    if (Size > 0) {
        int srgb_intent = Data[0] % 4;  // Valid intents are 0-3
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    }
    
    // Test png_get_gAMA
    double file_gamma;
    png_uint_32 gamma_result = png_get_gAMA(png_ptr, info_ptr, &file_gamma);
    (void)gamma_result;
    
    // Test png_get_gAMA_fixed
    png_fixed_point int_file_gamma;
    png_uint_32 fixed_gamma_result = png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    (void)fixed_gamma_result;
    
    // Test png_set_gAMA
    if (Size > 1) {
        double set_gamma = (Data[1] / 255.0) * 10.0;  // Scale to reasonable range
        png_set_gAMA(png_ptr, info_ptr, set_gamma);
    }
    
    // Test png_set_gAMA_fixed
    if (Size > 2) {
        png_fixed_point int_set_gamma = Data[2] * 1000;
        png_set_gAMA_fixed(png_ptr, info_ptr, int_set_gamma);
    }
    
    // Test png_get_cHRM
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    png_uint_32 chrm_result = png_get_cHRM(png_ptr, info_ptr, 
                                           &white_x, &white_y,
                                           &red_x, &red_y,
                                           &green_x, &green_y,
                                           &blue_x, &blue_y);
    (void)chrm_result;
    
    // Test png_get_cHRM_XYZ
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_uint_32 chrm_xyz_result = png_get_cHRM_XYZ(png_ptr, info_ptr,
                                                   &red_X, &red_Y, &red_Z,
                                                   &green_X, &green_Y, &green_Z,
                                                   &blue_X, &blue_Y, &blue_Z);
    (void)chrm_xyz_result;
    
    // Test png_get_cHRM_fixed
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_uint_32 chrm_fixed_result = png_get_cHRM_fixed(png_ptr, info_ptr,
                                                       &int_white_x, &int_white_y,
                                                       &int_red_x, &int_red_y,
                                                       &int_green_x, &int_green_y,
                                                       &int_blue_x, &int_blue_y);
    (void)chrm_fixed_result;
    
    // Test png_get_cHRM_XYZ_fixed
    png_fixed_point int_red_X, int_red_Y, int_red_Z;
    png_fixed_point int_green_X, int_green_Y, int_green_Z;
    png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
    png_uint_32 chrm_xyz_fixed_result = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                                               &int_red_X, &int_red_Y, &int_red_Z,
                                                               &int_green_X, &int_green_Y, &int_green_Z,
                                                               &int_blue_X, &int_blue_Y, &int_blue_Z);
    (void)chrm_xyz_fixed_result;
    
    // Test png_set_cHRM
    if (Size > 10) {
        double w_x = (Data[3] / 255.0) * 0.8;
        double w_y = (Data[4] / 255.0) * 0.8;
        double r_x = (Data[5] / 255.0) * 0.8;
        double r_y = (Data[6] / 255.0) * 0.8;
        double g_x = (Data[7] / 255.0) * 0.8;
        double g_y = (Data[8] / 255.0) * 0.8;
        double b_x = (Data[9] / 255.0) * 0.8;
        double b_y = (Data[10] / 255.0) * 0.8;
        png_set_cHRM(png_ptr, info_ptr, w_x, w_y, r_x, r_y, g_x, g_y, b_x, b_y);
    }
    
    // Test png_set_cHRM_XYZ
    if (Size > 18) {
        double rX = (Data[11] / 255.0) * 0.8;
        double rY = (Data[12] / 255.0) * 0.8;
        double rZ = (Data[13] / 255.0) * 0.8;
        double gX = (Data[14] / 255.0) * 0.8;
        double gY = (Data[15] / 255.0) * 0.8;
        double gZ = (Data[16] / 255.0) * 0.8;
        double bX = (Data[17] / 255.0) * 0.8;
        double bY = (Data[18] / 255.0) * 0.8;
        double bZ = (Size > 19) ? (Data[19] / 255.0) * 0.8 : 0.1;
        png_set_cHRM_XYZ(png_ptr, info_ptr, rX, rY, rZ, gX, gY, gZ, bX, bY, bZ);
    }
    
    // Test png_set_cHRM_fixed
    if (Size > 26) {
        png_fixed_point iw_x = Data[20] * 1000;
        png_fixed_point iw_y = Data[21] * 1000;
        png_fixed_point ir_x = Data[22] * 1000;
        png_fixed_point ir_y = Data[23] * 1000;
        png_fixed_point ig_x = Data[24] * 1000;
        png_fixed_point ig_y = Data[25] * 1000;
        png_fixed_point ib_x = Data[26] * 1000;
        png_fixed_point ib_y = (Size > 27) ? Data[27] * 1000 : 1000;
        png_set_cHRM_fixed(png_ptr, info_ptr, iw_x, iw_y, ir_x, ir_y, ig_x, ig_y, ib_x, ib_y);
    }
    
    // Test png_set_cHRM_XYZ_fixed
    if (Size > 35) {
        png_fixed_point irX = Data[28] * 1000;
        png_fixed_point irY = Data[29] * 1000;
        png_fixed_point irZ = Data[30] * 1000;
        png_fixed_point igX = Data[31] * 1000;
        png_fixed_point igY = Data[32] * 1000;
        png_fixed_point igZ = Data[33] * 1000;
        png_fixed_point ibX = Data[34] * 1000;
        png_fixed_point ibY = Data[35] * 1000;
        png_fixed_point ibZ = (Size > 36) ? Data[36] * 1000 : 1000;
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, irX, irY, irZ, igX, igY, igZ, ibX, ibY, ibZ);
    }
    
    // Test png_get_cICP
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_uint_32 cicp_result = png_get_cICP(png_ptr, info_ptr,
                                           &colour_primaries, &transfer_function,
                                           &matrix_coefficients, &video_full_range_flag);
    (void)cicp_result;
    
    // Test png_get_hIST
    png_uint_16p hist;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_result;
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    // Also test with write structure
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                    nullptr, dummy_error_fn, dummy_warning_fn);
    if (write_ptr) {
        png_infop write_info = png_create_info_struct(write_ptr);
        if (write_info) {
            if (setjmp(png_jmpbuf(write_ptr))) {
                png_destroy_write_struct(&write_ptr, &write_info);
                return 0;
            }
            
            // Set dummy write function
            png_set_write_fn(write_ptr, nullptr, dummy_write_fn, nullptr);
            
            // Test setting functions on write structure
            if (Size > 0) {
                int write_intent = Data[0] % 4;
                png_set_sRGB_gAMA_and_cHRM(write_ptr, write_info, write_intent);
            }
            
            if (Size > 1) {
                double write_gamma = (Data[1] / 255.0) * 10.0;
                png_set_gAMA(write_ptr, write_info, write_gamma);
            }
            
            png_destroy_write_struct(&write_ptr, &write_info);
        } else {
            png_destroy_write_struct(&write_ptr, nullptr);
        }
    }
    
    return 0;
}