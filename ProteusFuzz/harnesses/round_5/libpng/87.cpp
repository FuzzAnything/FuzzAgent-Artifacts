// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <vector>

// Suppress warnings about deprecated types if necessary, though we use them as per libpng API
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

static void write_dummy_png(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        // Write at least a minimal PNG header if data is too small, otherwise write data
        if (Size < 8) {
            file.write(reinterpret_cast<const char*>(Data), Size);
            // Pad with zeros to ensure we have some data for reading if needed
            for (size_t i = Size; i < 8; ++i) {
                file.put(0);
            }
        } else {
            file.write(reinterpret_cast<const char*>(Data), Size);
        }
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize libpng structures
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

    // Prepare dummy file for reading scenarios
    write_dummy_png(Data, Size);

    // --- Scenario 1: Writing cHRM data (set functions) ---
    // We create a write context to test set functions
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Extract random doubles from input for cHRM values
    // Ensure we have enough data
    double white_x = 0.3127, white_y = 0.3290;
    double red_x = 0.6400, red_y = 0.3300;
    double green_x = 0.3000, green_y = 0.6000;
    double blue_x = 0.1500, blue_y = 0.0600;

    double red_X = 0.4124, red_Y = 0.2126, red_Z = 0.0193;
    double green_X = 0.3576, green_Y = 0.7152, green_Z = 0.1192;
    double blue_X = 0.1805, blue_Y = 0.0722, blue_Z = 0.9505;

    // Use input data to perturb values if available
    if (Size > 32) {
        white_x = (double)Data[0] / 255.0;
        white_y = (double)Data[1] / 255.0;
        red_x = (double)Data[2] / 255.0;
        red_y = (double)Data[3] / 255.0;
        green_x = (double)Data[4] / 255.0;
        green_y = (double)Data[5] / 255.0;
        blue_x = (double)Data[6] / 255.0;
        blue_y = (double)Data[7] / 255.0;
        
        red_X = (double)Data[8] / 255.0;
        red_Y = (double)Data[9] / 255.0;
        red_Z = (double)Data[10] / 255.0;
        green_X = (double)Data[11] / 255.0;
        green_Y = (double)Data[12] / 255.0;
        green_Z = (double)Data[13] / 255.0;
        blue_X = (double)Data[14] / 255.0;
        blue_Y = (double)Data[15] / 255.0;
        blue_Z = (double)Data[16] / 255.0;
    }

    // Test png_set_cHRM
    png_set_cHRM(write_png_ptr, write_info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    // Test png_set_cHRM_XYZ
    png_set_cHRM_XYZ(write_png_ptr, write_info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // Test png_set_cHRM_fixed
    png_fixed_point fx = static_cast<png_fixed_point>(white_x * 100000);
    png_set_cHRM_fixed(write_png_ptr, write_info_ptr, fx, fx, fx, fx, fx, fx, fx, fx);

    // Test png_set_cHRM_XYZ_fixed
    png_fixed_point fX = static_cast<png_fixed_point>(red_X * 100000);
    png_set_cHRM_XYZ_fixed(write_png_ptr, write_info_ptr, fX, fX, fX, fX, fX, fX, fX, fX, fX);

    // Test png_set_sRGB_gAMA_and_cHRM
    int srgb_intent = (Size > 20) ? Data[20] % 4 : 0;
    png_set_sRGB_gAMA_and_cHRM(write_png_ptr, write_info_ptr, srgb_intent);

    // Test png_set_gAMA
    double gamma = (Size > 21) ? (double)Data[21] / 255.0 : 2.2;
    png_set_gAMA(write_png_ptr, write_info_ptr, gamma);
    png_set_gAMA_fixed(write_png_ptr, write_info_ptr, static_cast<png_fixed_point>(gamma * 100000));

    // Cleanup write structures
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);

    // --- Scenario 2: Reading cHRM data (get functions) ---
    // Attempt to open the dummy file to test get functions
    FILE *fp = fopen("./dummy_file", "rb");
    if (fp) {
        png_init_io(png_ptr, fp);

        // Try to read the signature (might fail if dummy data is invalid, which is fine for fuzzing)
        // We catch errors via setjmp above, but we also need to handle the case where reading fails gracefully
        // We don't call png_read_info here because the file is likely garbage, which will trigger errors.
        // Instead, we directly test the get functions on the info_ptr (which might be empty or partially filled).
        
        // Test png_get_cHRM
        double gx_white_x, gx_white_y, gx_red_x, gx_red_y, gx_green_x, gx_green_y, gx_blue_x, gx_blue_y;
        png_get_cHRM(png_ptr, info_ptr, &gx_white_x, &gx_white_y, &gx_red_x, &gx_red_y, &gx_green_x, &gx_green_y, &gx_blue_x, &gx_blue_y);

        // Test png_get_cHRM_XYZ
        double gx_red_X, gx_red_Y, gx_red_Z, gx_green_X, gx_green_Y, gx_green_Z, gx_blue_X, gx_blue_Y, gx_blue_Z;
        png_get_cHRM_XYZ(png_ptr, info_ptr, &gx_red_X, &gx_red_Y, &gx_red_Z, &gx_green_X, &gx_green_Y, &gx_green_Z, &gx_blue_X, &gx_blue_Y, &gx_blue_Z);

        // Test png_get_cHRM_fixed
        png_fixed_point gf_white_x, gf_white_y, gf_red_x, gf_red_y, gf_green_x, gf_green_y, gf_blue_x, gf_blue_y;
        png_get_cHRM_fixed(png_ptr, info_ptr, &gf_white_x, &gf_white_y, &gf_red_x, &gf_red_y, &gf_green_x, &gf_green_y, &gf_blue_x, &gf_blue_y);

        // Test png_get_cHRM_XYZ_fixed
        png_fixed_point gf_red_X, gf_red_Y, gf_red_Z, gf_green_X, gf_green_Y, gf_green_Z, gf_blue_X, gf_blue_Y, gf_blue_Z;
        png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &gf_red_X, &gf_red_Y, &gf_red_Z, &gf_green_X, &gf_green_Y, &gf_green_Z, &gf_blue_X, &gf_blue_Y, &gf_blue_Z);

        // Test png_get_gAMA
        double file_gamma;
        png_get_gAMA(png_ptr, info_ptr, &file_gamma);
        png_fixed_point int_file_gamma;
        png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);

        // Test png_get_cICP (if supported)
        png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
        png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);

        fclose(fp);
    }

    // Cleanup read structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}
#pragma clang diagnostic pop