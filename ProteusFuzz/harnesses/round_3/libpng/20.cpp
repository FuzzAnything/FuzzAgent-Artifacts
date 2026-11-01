// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
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
#include <png.h>
#include <cstring>
#include <cstdio>

static void write_dummy_png(const uint8_t *Data, size_t Size, const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;
    fwrite(Data, 1, Size, f);
    fclose(f);
}

static void custom_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Silently ignore errors for fuzzing
}

static void custom_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Silently ignore warnings for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file
    write_dummy_png(Data, Size, "./dummy_file");

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    // Initialize PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom error/warning handlers to prevent crashes
    png_set_error_fn(png_ptr, NULL, custom_error_handler, custom_warning_handler);

    // Use setjmp to handle errors gracefully
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    png_read_info(png_ptr, info_ptr);
    fclose(fp);

    // --- Fuzz Target: png_get_gAMA_fixed ---
    png_fixed_point int_file_gamma = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    png_get_gAMA_fixed(png_ptr, info_ptr, NULL); // Test NULL pointer handling

    // --- Fuzz Target: png_get_cHRM_fixed ---
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
    // Test with some NULL pointers
    png_get_cHRM_fixed(png_ptr, info_ptr, NULL, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, NULL, NULL, NULL,
                       NULL, NULL, NULL, NULL);

    // --- Fuzz Target: png_get_cHRM_XYZ_fixed ---
    png_fixed_point int_red_X, int_red_Y, int_red_Z;
    png_fixed_point int_green_X, int_green_Y, int_green_Z;
    png_fixed_point int_blue_X, int_blue_Y, int_blue_Z;
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z,
                           &int_green_X, &int_green_Y, &int_green_Z,
                           &int_blue_X, &int_blue_Y, &int_blue_Z);
    // Test with NULL pointers
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, NULL, &int_red_Y, &int_red_Z,
                           &int_green_X, &int_green_Y, &int_green_Z,
                           &int_blue_X, &int_blue_Y, &int_blue_Z);

    // --- Fuzz Target: png_get_sRGB ---
    int file_srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);
    png_get_sRGB(png_ptr, info_ptr, NULL); // Test NULL pointer handling

    // --- Fuzz Target: png_set_sRGB_gAMA_and_cHRM ---
    // Note: This is a setter, usually called before writing, but we can call it on read structs
    // to test internal validation logic.
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, 0);
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, 1);
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, 2);
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, 3);
    png_set_sRGB_gAMA_and_cHRM(NULL, info_ptr, 0); // Test NULL png_ptr
    png_set_sRGB_gAMA_and_cHRM(png_ptr, NULL, 0);  // Test NULL info_ptr

    // --- Fuzz Target: png_set_cHRM_XYZ_fixed ---
    // Use values derived from input data to vary execution paths
    png_fixed_point fuzz_red_X = (Size > 0) ? Data[0] * 1000 : 0;
    png_fixed_point fuzz_red_Y = (Size > 1) ? Data[1] * 1000 : 0;
    png_fixed_point fuzz_red_Z = (Size > 2) ? Data[2] * 1000 : 0;
    png_fixed_point fuzz_green_X = (Size > 3) ? Data[3] * 1000 : 0;
    png_fixed_point fuzz_green_Y = (Size > 4) ? Data[4] * 1000 : 0;
    png_fixed_point fuzz_green_Z = (Size > 5) ? Data[5] * 1000 : 0;
    png_fixed_point fuzz_blue_X = (Size > 6) ? Data[6] * 1000 : 0;
    png_fixed_point fuzz_blue_Y = (Size > 7) ? Data[7] * 1000 : 0;
    png_fixed_point fuzz_blue_Z = (Size > 8) ? Data[8] * 1000 : 0;

    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, fuzz_red_X, fuzz_red_Y, fuzz_red_Z,
                           fuzz_green_X, fuzz_green_Y, fuzz_green_Z,
                           fuzz_blue_X, fuzz_blue_Y, fuzz_blue_Z);
    
    // Test with NULL pointers
    png_set_cHRM_XYZ_fixed(NULL, info_ptr, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    png_set_cHRM_XYZ_fixed(png_ptr, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}