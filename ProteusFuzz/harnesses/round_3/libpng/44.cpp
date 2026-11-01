// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
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
#include <stdlib.h>
#include <string.h>

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Do nothing, just a dummy to satisfy the API
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file to satisfy libpng's file I/O expectations if needed,
    // though we will use a dummy read callback to avoid complex state management.
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_callback, dummy_warning_callback);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize IO with a dummy callback to avoid file descriptor issues during fuzzing
    // We pass a dummy pointer as the io_ptr
    png_set_read_fn(png_ptr, (png_voidp)Data, dummy_read_callback);

    // Prepare dummy color
    png_color_16 background_color = {0, 0, 0, 0, 0};
    
    // Prepare random values from input
    int gamma_code = (Size > 0) ? Data[0] % 4 : 0;
    int need_expand = (Size > 1) ? Data[1] % 2 : 0;
    png_fixed_point gamma_val = (Size > 2) ? ((png_fixed_point)Data[2] * 100000) : 100000;
    
    // Ensure gamma is valid (>0) for some calls
    if (gamma_val <= 0) gamma_val = 100000;

    int mode = (Size > 3) ? Data[3] % 4 : 0; // 0 to 3 for modes
    double output_gamma_double = (Size > 4) ? (double)Data[4] / 10.0 : 1.0;
    if (output_gamma_double <= 0) output_gamma_double = 1.0;
    png_fixed_point output_gamma_fixed = (png_fixed_point)(output_gamma_double * 100000);
    
    png_fixed_point screen_gamma = (Size > 5) ? ((png_fixed_point)Data[5] * 100000) : 100000;
    if (screen_gamma <= 0) screen_gamma = 100000;
    png_fixed_point override_file_gamma = (Size > 6) ? ((png_fixed_point)Data[6] * 100000) : 100000;
    if (override_file_gamma <= 0) override_file_gamma = 100000;

    // Attempt to read the IHDR to ensure the struct is in a valid state for some operations
    // We wrap in setjmp to catch any read errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Try to read info. Since our read callback does nothing, this might fail or return partial data,
    // which is fine for fuzzing the API logic.
    png_read_info(png_ptr, info_ptr);

    // 1. png_set_background_fixed
    png_set_background_fixed(png_ptr, &background_color, gamma_code, need_expand, gamma_val);

    // 2. png_set_alpha_mode
    png_set_alpha_mode(png_ptr, mode, output_gamma_double);

    // 3. png_set_alpha_mode_fixed
    png_set_alpha_mode_fixed(png_ptr, mode, output_gamma_fixed);

    // 4. png_set_background
    png_set_background(png_ptr, &background_color, gamma_code, need_expand, output_gamma_double);

    // 5. png_set_gamma_fixed
    png_set_gamma_fixed(png_ptr, screen_gamma, override_file_gamma);

    // 6. png_set_strip_alpha
    png_set_strip_alpha(png_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}