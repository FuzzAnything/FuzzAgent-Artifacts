// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
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

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Suppress error messages during fuzzing to avoid noise
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Suppress warning messages during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to simulate a PNG file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Set error handlers to suppress output
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);

    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Use setjmp to handle errors gracefully
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Fuzz target API functions
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns; // Use the value to avoid unused variable warning

    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)y_offset_inches;

    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    (void)x_offset_inches;

    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)pixel_aspect_ratio;

    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // Optional: Call png_get_pHYs_dpi if supported
#ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)res_x;
    (void)res_y;
    (void)unit_type;
#endif

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    remove(filename);

    return 0;
}