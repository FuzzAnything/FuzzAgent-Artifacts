// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_png at pngwrite.c:1376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_sRGB at pngset.c:819:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
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
#include <fstream>
#include <vector>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
}

static void custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Suppress error messages during fuzzing to avoid clutter
}

static void custom_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Suppress warning messages during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Need at least 12 bytes to safely access indices 0-11
    if (Size < 12) {
        return 0;
    }

    // Create a dummy file for writing output
    write_dummy_file(Data, Size);

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, custom_error_fn, custom_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp to handle errors gracefully
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Open the dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set basic image properties
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Extract fuzzing data for various parameters safely
    int srgb_intent = Data[0] % 4; // Valid intents: 0-3
    png_fixed_point gamma = (png_fixed_point)(Data[1] * 100000); // Fixed-point gamma
    png_fixed_point red_X = (png_fixed_point)(Data[2] * 100000);
    png_fixed_point red_Y = (png_fixed_point)(Data[3] * 100000);
    png_fixed_point red_Z = (png_fixed_point)(Data[4] * 100000);
    png_fixed_point green_X = (png_fixed_point)(Data[5] * 100000);
    png_fixed_point green_Y = (png_fixed_point)(Data[6] * 100000);
    png_fixed_point green_Z = (png_fixed_point)(Data[7] * 100000);
    png_fixed_point blue_X = (png_fixed_point)(Data[8] * 100000);
    png_fixed_point blue_Y = (png_fixed_point)(Data[9] * 100000);
    png_fixed_point blue_Z = (png_fixed_point)(Data[10] * 100000);
    int mask = Data[11] % 100; // Mask for png_set_invalid

    // Call target API functions
    png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    png_set_sRGB(png_ptr, info_ptr, srgb_intent);
    png_set_gAMA_fixed(png_ptr, info_ptr, gamma);
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr, red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z);
    png_set_invalid(png_ptr, info_ptr, mask);

    // Prepare dummy image data
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    png_bytep row = (png_bytep)png_malloc(png_ptr, row_bytes);
    if (row) {
        memset(row, 0, row_bytes);
    }

    // Write the image
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);

    // Cleanup
    if (row) {
        png_free(png_ptr, row);
    }
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}