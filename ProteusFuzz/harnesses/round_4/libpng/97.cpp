// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_flush at pngwrite.c:966:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
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

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely extract parameters
    // We need at least 20 bytes for the parameters used below
    if (Size < 20) return 0;

    // Create a dummy file to simulate I/O
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    
    // Write fuzzing data to the dummy file
    size_t written = fwrite(Data, 1, Size, fp);
    fclose(fp);
    if (written != Size) return 0;

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return 0;
    }

    // Setup error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Initialize I/O
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }
    png_init_io(png_ptr, fp);
    fclose(fp);

    // Extract fuzzing data for parameters safely
    int srgb_intent = Data[0] % 4; // 0-3
    double screen_gamma = (Data[1] / 255.0) * 2.0 + 0.5; // 0.5 to 2.5
    double file_gamma = (Data[2] / 255.0) * 2.0 + 0.5;
    
    // Generate chromaticity values
    double white_x = (Data[3] / 255.0);
    double white_y = (Data[4] / 255.0);
    double red_x = (Data[5] / 255.0);
    double red_y = (Data[6] / 255.0);
    double green_x = (Data[7] / 255.0);
    double green_y = (Data[8] / 255.0);
    double blue_x = (Data[9] / 255.0);
    double blue_y = (Data[10] / 255.0);

    double red_X = (Data[11] / 255.0);
    double red_Y = (Data[12] / 255.0);
    double red_Z = (Data[13] / 255.0);
    double green_X = (Data[14] / 255.0);
    double green_Y = (Data[15] / 255.0);
    double green_Z = (Data[16] / 255.0);
    double blue_X = (Data[17] / 255.0);
    double blue_Y = (Data[18] / 255.0);
    double blue_Z = (Data[19] / 255.0);

    // Fuzz png_set_sRGB_gAMA_and_cHRM
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);

    // Fuzz png_set_gAMA
    png_set_gAMA(png_ptr, info_ptr, file_gamma);

    // Fuzz png_set_cHRM
    png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);

    // Fuzz png_set_cHRM_XYZ
    png_set_cHRM_XYZ(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);

    // Fuzz png_set_gamma
    png_set_gamma(png_ptr, screen_gamma, file_gamma);

    // Fuzz png_set_flush
    int nrows = Data[19] % 100; // Use last available byte
    png_set_flush(png_ptr, nrows);

    // Prepare for reading back to test getters (simulate a minimal valid PNG structure if possible)
    // Since we just wrote dummy data, reading might fail, but we wrap in setjmp to handle it.
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Re-open for reading to test getters
    fp = fopen(dummy_file, "rb");
    if (fp) {
        png_init_io(png_ptr, fp);
        
        // Try to read header
        png_read_info(png_ptr, info_ptr);
        fclose(fp);

        // Fuzz png_get_gAMA
        double file_gamma_out;
        png_get_gAMA(png_ptr, info_ptr, &file_gamma_out);

        // Fuzz png_get_cHRM
        double w_x, w_y, r_x, r_y, g_x, g_y, b_x, b_y;
        png_get_cHRM(png_ptr, info_ptr, &w_x, &w_y, &r_x, &r_y, &g_x, &g_y, &b_x, &b_y);

        // Fuzz png_get_cHRM_XYZ
        double r_X, r_Y, r_Z, g_X, g_Y, g_Z, b_X, b_Y, b_Z;
        png_get_cHRM_XYZ(png_ptr, info_ptr, &r_X, &r_Y, &r_Z, &g_X, &g_Y, &g_Z, &b_X, &b_Y, &b_Z);
        
        // Fuzz png_get_hIST (might fail if not supported or data missing, but safe with setjmp)
        png_uint_16 *hist = NULL;
        png_get_hIST(png_ptr, info_ptr, &hist);
    }

    safe_cleanup(png_ptr, info_ptr);
    return 0;
}