// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_oFFs at pngset.c:465:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
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
#include <stddef.h>
#include <string.h>

static void
fuzzer_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void
fuzzer_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, fuzzer_error_callback, fuzzer_warning_callback);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling for read operations
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize a dummy file for reading
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Test png_get_x_offset_microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns;

    // Test png_get_y_offset_microns
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    (void)y_offset_microns;

    // Test png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    // Test png_get_x_offset_pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    // Test png_get_y_offset_pixels
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // Test png_get_y_offset_inches_fixed
    png_fixed_point y_offset_inches = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches;

    // Test png_get_pHYs_dpi
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)res_x;
    (void)res_y;
    (void)unit_type;

    // Test png_set_oFFs with various inputs
    png_int_32 offset_x = (Size > 0) ? (png_int_32)Data[0] : 0;
    png_int_32 offset_y = (Size > 1) ? (png_int_32)Data[1] : 0;
    int unit = (Size > 2) ? (int)Data[2] : 0;
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit);

    // Clean up
    png_read_end(png_ptr, NULL);
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    remove(dummy_file);

    return 0;
}