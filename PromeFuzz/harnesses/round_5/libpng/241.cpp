// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function that returns zeroes
    memset(data, 0, length);
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp message) {
    // Ignore warnings during fuzzing
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp message) {
    // Longjmp on error as expected by libpng
    longjmp(png_jmpbuf(png_ptr), 1);
}

extern "C" int LLVMFuzzerTestOneInput_241(const uint8_t *Data, size_t Size) {
    // Write data to dummy file as per instructions
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Setup custom read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);

    // Try to read PNG info
    png_read_info(png_ptr, info_ptr);

    // Call target API functions
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches;

    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_uint_32 phys_dpi = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_dpi;

    png_fixed_point y_offset_inches = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches;

    const png_byte* signature = png_get_signature(png_ptr, info_ptr);
    (void)signature;

    png_uint_32 phys_info = png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_info;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}