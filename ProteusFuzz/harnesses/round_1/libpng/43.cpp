// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fuzzer_custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Ignore errors during fuzzing to prevent aborts
}

static void fuzzer_custom_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Ignore warnings during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for the fuzzer input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Initialize PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set error handling to avoid crashes
    png_set_error_fn(png_ptr, NULL, fuzzer_custom_error_fn, fuzzer_custom_warning_fn);

    // Use setjmp/longjmp for error recovery
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    // We do not read the full image to save time, just the header info needed for pHYs
    png_read_info(png_ptr, info_ptr);

    // Call the target API functions
    // These functions are safe to call even if pHYs chunk is missing (they return 0)
    png_uint_32 x_ppi = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    png_uint_32 ppi = png_get_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    png_uint_32 y_ppm = png_get_y_pixels_per_meter(png_ptr, info_ptr);

    // Use the values to prevent compiler optimizations
    volatile png_uint_32 unused = x_ppi + y_ppi + ppm + ppi + x_ppm + y_ppm;
    (void)unused;

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}