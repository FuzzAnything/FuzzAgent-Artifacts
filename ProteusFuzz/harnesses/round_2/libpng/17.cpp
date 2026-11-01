// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_tRNS_to_alpha at pngrtran.c:969:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <vector>

static void write_dummy_png_file(const char* filename, const uint8_t* data, size_t size) {
    FILE* f = fopen(filename, "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file with the fuzzed data
    const char* dummy_file = "./dummy_file";
    write_dummy_png_file(dummy_file, Data, Size);

    // Initialize PNG read structure
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

    FILE* fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Try to read the PNG signature and IHDR
    png_byte sig[8];
    if (fread(sig, 1, 8, fp) != 8) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    if (!png_sig_cmp(sig, 0, 8)) {
        png_set_sig_bytes(png_ptr, 8);
    } else {
        // If not a valid PNG, we still try to fuzz the API with invalid data
        // to test error handling paths, but we must be careful not to crash
    }

    // Read info (this may fail if data is invalid, which is fine for fuzzing)
    png_read_info(png_ptr, info_ptr);

    // Prepare test data for API calls
    png_color_16 background_color;
    background_color.index = 0;
    background_color.red = 255;
    background_color.green = 255;
    background_color.blue = 255;
    background_color.gray = 0;
    background_color.red = 0; // Reuse red for blue if needed, but struct has separate fields

    int gamma_code = 0; // PNG_BACKGROUND_GAMMA_UNKNOWN
    int need_expand = 0;
    double background_gamma = 1.0;
    png_fixed_point fixed_background_gamma = PNG_FP_1;

    double screen_gamma = 1.0;
    double override_file_gamma = 1.0;
    png_fixed_point fixed_screen_gamma = PNG_FP_1;
    png_fixed_point fixed_override_file_gamma = PNG_FP_1;

    int error_action = 0;
    double red_coeff = 0.3;
    double green_coeff = 0.59;
    png_fixed_point fixed_red_coeff = PNG_FP_1 * 30000; // Approximate 0.3
    png_fixed_point fixed_green_coeff = PNG_FP_1 * 59000; // Approximate 0.59

    int alpha_mode = 0; // PNG_ALPHA_PNG
    double output_gamma = PNG_DEFAULT_sRGB;
    png_fixed_point fixed_output_gamma = PNG_DEFAULT_sRGB;

    int nrows = 10;

    // Fuzz png_set_background
    png_set_background(png_ptr, &background_color, gamma_code, need_expand, background_gamma);
    png_set_background_fixed(png_ptr, &background_color, gamma_code, need_expand, fixed_background_gamma);

    // Fuzz png_set_gamma
    png_set_gamma(png_ptr, screen_gamma, override_file_gamma);
    png_set_gamma_fixed(png_ptr, fixed_screen_gamma, fixed_override_file_gamma);

    // Fuzz png_set_rgb_to_gray
    png_set_rgb_to_gray(png_ptr, error_action, red_coeff, green_coeff);
    png_set_rgb_to_gray_fixed(png_ptr, error_action, fixed_red_coeff, fixed_green_coeff);
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Suppress unused warning

    // Fuzz png_set_alpha_mode
    png_set_alpha_mode(png_ptr, alpha_mode, output_gamma);
    png_set_alpha_mode_fixed(png_ptr, alpha_mode, fixed_output_gamma);

    // Fuzz png_set_gray_to_rgb
    png_set_gray_to_rgb(png_ptr);

    // Fuzz png_set_tRNS_to_alpha
    png_set_tRNS_to_alpha(png_ptr);

    // Fuzz png_set_flush
    png_set_flush(png_ptr, nrows);

    // Fuzz png_set_scale_16
    png_set_scale_16(png_ptr);

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}