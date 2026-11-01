// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstring>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write fuzzing data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);
    png_set_read_fn(png_ptr, fp, NULL);

    // Read the PNG header
    png_read_info(png_ptr, info_ptr);

    // Fuzz target API functions
    // 1. png_get_y_pixels_per_inch
    png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)y_ppi;

    // 2. png_get_y_offset_inches
    float y_offset_in = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)y_offset_in;

#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point y_offset_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_fixed;
#endif

    // 3. png_get_pixel_aspect_ratio
    float aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)aspect_ratio;

#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio_fixed;
#endif

    // 4. png_get_pixels_per_meter
    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    // 5. png_get_y_pixels_per_meter
    png_uint_32 y_ppm = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    (void)y_ppm;

    // 6. png_get_x_offset_inches
    float x_offset_in = png_get_x_offset_inches(png_ptr, info_ptr);
    (void)x_offset_in;

#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_fixed;
#endif

    // Additional calls for completeness based on provided signatures
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)res_x;
    (void)res_y;
    (void)unit_type;

    png_int_32 x_offset_px = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_px;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}