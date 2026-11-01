// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
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

// Custom error handling to prevent libpng from aborting
// Note: The function pointer types must match exactly what libpng expects.
// png_error_ptr is defined as void (*)(png_structp, png_const_charp)
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort or longjmp. Just return to allow fuzzer to continue.
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Helper to convert uint8_t data to double
static double
data_to_double(const uint8_t *data, size_t size) {
    if (size < 8) {
        return 0.0;
    }
    double d;
    memcpy(&d, data, 8);
    return d;
}

// Helper to convert uint8_t data to png_fixed_point
static png_fixed_point
data_to_fixed(const uint8_t *data, size_t size) {
    if (size < 4) {
        return 0;
    }
    int32_t val;
    memcpy(&val, data, 4);
    return (png_fixed_point)val;
}

// Helper to convert uint8_t data to png_byte
static png_byte
data_to_byte(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }
    return data[0];
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Initialize libpng structures for writing
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return 0;
    }

    // Set custom error handlers to prevent aborts during fuzzing
    // Ensure the function signatures match png_error_ptr exactly
    png_set_error_fn(png_ptr, NULL, fuzz_png_error, fuzz_png_warning);

    // Create a dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize write
    png_init_io(png_ptr, fp);

    // Set IHDR (required before writing other chunks)
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    size_t offset = 0;
    
    // 1. Test png_set_cHRM (double)
    if (Size >= offset + 64) {
        double white_x = data_to_double(Data + offset, Size - offset);
        double white_y = data_to_double(Data + offset + 8, Size - offset - 8);
        double red_x = data_to_double(Data + offset + 16, Size - offset - 16);
        double red_y = data_to_double(Data + offset + 24, Size - offset - 24);
        double green_x = data_to_double(Data + offset + 32, Size - offset - 32);
        double green_y = data_to_double(Data + offset + 40, Size - offset - 40);
        double blue_x = data_to_double(Data + offset + 48, Size - offset - 48);
        double blue_y = data_to_double(Data + offset + 56, Size - offset - 56);
        
        png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
        offset += 64;
    }

    // 2. Test png_set_cHRM_fixed
    if (Size >= offset + 32) {
        png_fixed_point int_white_x = data_to_fixed(Data + offset, Size - offset);
        png_fixed_point int_white_y = data_to_fixed(Data + offset + 4, Size - offset - 4);
        png_fixed_point int_red_x = data_to_fixed(Data + offset + 8, Size - offset - 8);
        png_fixed_point int_red_y = data_to_fixed(Data + offset + 12, Size - offset - 12);
        png_fixed_point int_green_x = data_to_fixed(Data + offset + 16, Size - offset - 16);
        png_fixed_point int_green_y = data_to_fixed(Data + offset + 20, Size - offset - 20);
        png_fixed_point int_blue_x = data_to_fixed(Data + offset + 24, Size - offset - 24);
        png_fixed_point int_blue_y = data_to_fixed(Data + offset + 28, Size - offset - 28);
        
        png_set_cHRM_fixed(png_ptr, info_ptr, int_white_x, int_white_y, int_red_x, int_red_y, 
                           int_green_x, int_green_y, int_blue_x, int_blue_y);
        offset += 32;
    }

    // 3. Test png_set_cHRM_XYZ (double)
    if (Size >= offset + 72) {
        double red_X = data_to_double(Data + offset, Size - offset);
        double red_Y = data_to_double(Data + offset + 8, Size - offset - 8);
        double red_Z = data_to_double(Data + offset + 16, Size - offset - 16);
        double green_X = data_to_double(Data + offset + 24, Size - offset - 24);
        double green_Y = data_to_double(Data + offset + 32, Size - offset - 32);
        double green_Z = data_to_double(Data + offset + 40, Size - offset - 40);
        double blue_X = data_to_double(Data + offset + 48, Size - offset - 48);
        double blue_Y = data_to_double(Data + offset + 56, Size - offset - 56);
        double blue_Z = data_to_double(Data + offset + 64, Size - offset - 64);
        
        png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
        offset += 72;
    }

    // 4. Test png_set_cHRM_XYZ_fixed
    if (Size >= offset + 36) {
        png_fixed_point int_red_X = data_to_fixed(Data + offset, Size - offset);
        png_fixed_point int_red_Y = data_to_fixed(Data + offset + 4, Size - offset - 4);
        png_fixed_point int_red_Z = data_to_fixed(Data + offset + 8, Size - offset - 8);
        png_fixed_point int_green_X = data_to_fixed(Data + offset + 12, Size - offset - 12);
        png_fixed_point int_green_Y = data_to_fixed(Data + offset + 16, Size - offset - 16);
        png_fixed_point int_green_Z = data_to_fixed(Data + offset + 20, Size - offset - 20);
        png_fixed_point int_blue_X = data_to_fixed(Data + offset + 24, Size - offset - 24);
        png_fixed_point int_blue_Y = data_to_fixed(Data + offset + 28, Size - offset - 28);
        png_fixed_point int_blue_Z = data_to_fixed(Data + offset + 32, Size - offset - 32);
        
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, int_red_X, int_red_Y, int_red_Z, 
                               int_green_X, int_green_Y, int_green_Z, 
                               int_blue_X, int_blue_Y, int_blue_Z);
        offset += 36;
    }

    // 5. Test png_set_sRGB_gAMA_and_cHRM
    if (Size >= offset + 1) {
        int srgb_intent = (int)data_to_byte(Data + offset, Size - offset);
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
        offset += 1;
    }

    // 6. Test png_set_gAMA
    if (Size >= offset + 8) {
        double file_gamma = data_to_double(Data + offset, Size - offset);
        // Clamp gamma to valid range (0.01 to 10.0) to prevent libpng from triggering an error
        if (file_gamma > 0.01 && file_gamma < 10.0) {
            png_set_gAMA(png_ptr, info_ptr, file_gamma);
        } else {
            // Use a safe default if input is out of range
            png_set_gAMA(png_ptr, info_ptr, 2.2);
        }
        offset += 8;
    }

    // 7. Test png_set_gAMA_fixed
    if (Size >= offset + 4) {
        png_fixed_point int_file_gamma = data_to_fixed(Data + offset, Size - offset);
        // Clamp fixed gamma to valid range (10000 to 1000000) to prevent libpng from triggering an error
        if (int_file_gamma > 10000 && int_file_gamma < 1000000) {
            png_set_gAMA_fixed(png_ptr, info_ptr, int_file_gamma);
        } else {
            // Use a safe default if input is out of range
            png_set_gAMA_fixed(png_ptr, info_ptr, 45455); // 2.2 in fixed point
        }
        offset += 4;
    }

    // Now try to call getters. Note: These usually return 0 if the chunk hasn't been read.
    // However, calling them with valid pointers is safe and tests the internal logic.
    
    double white_x_out, white_y_out, red_x_out, red_y_out, green_x_out, green_y_out, blue_x_out, blue_y_out;
    png_get_cHRM(png_ptr, info_ptr, &white_x_out, &white_y_out, &red_x_out, &red_y_out, 
                 &green_x_out, &green_y_out, &blue_x_out, &blue_y_out);

    double red_X_out, red_Y_out, red_Z_out, green_X_out, green_Y_out, green_Z_out, blue_X_out, blue_Y_out, blue_Z_out;
    png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X_out, &red_Y_out, &red_Z_out, 
                     &green_X_out, &green_Y_out, &green_Z_out, 
                     &blue_X_out, &blue_Y_out, &blue_Z_out);

    png_fixed_point int_white_x_out, int_white_y_out, int_red_x_out, int_red_y_out, int_green_x_out, int_green_y_out, int_blue_x_out, int_blue_y_out;
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x_out, &int_white_y_out, &int_red_x_out, &int_red_y_out, 
                       &int_green_x_out, &int_green_y_out, &int_blue_x_out, &int_blue_y_out);

    png_fixed_point int_red_X_out, int_red_Y_out, int_red_Z_out, int_green_X_out, int_green_Y_out, int_green_Z_out, int_blue_X_out, int_blue_Y_out, int_blue_Z_out;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X_out, &int_red_Y_out, &int_red_Z_out, 
                           &int_green_X_out, &int_green_Y_out, &int_green_Z_out, 
                           &int_blue_X_out, &int_blue_Y_out, &int_blue_Z_out);

    double file_gamma_out;
    png_get_gAMA(png_ptr, info_ptr, &file_gamma_out);

    png_fixed_point int_file_gamma_out;
    png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma_out);

    // Test png_get_cICP if supported (requires specific flags, but calling it is safe)
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);

    // Test png_get_hIST
    png_uint_16 *hist = NULL;
    png_get_hIST(png_ptr, info_ptr, &hist);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}