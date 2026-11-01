// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cmath>

static void dummy_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void write_dummy_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_inforp info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);
    png_set_write_fn(png_ptr, NULL, write_dummy_data, NULL);

    double white_x = (Data[0] / 255.0);
    double white_y = (Data[1] / 255.0);
    double red_x = (Data[2] / 255.0);
    double red_y = (Data[3] / 255.0);
    double green_x = (Data[4] / 255.0);
    double green_y = (Data[5] / 255.0);
    double blue_x = (Data[6] / 255.0);
    double blue_y = (Data[7] / 255.0);
    double max_luminance = (Data[8] * 100.0) + 100.0;
    double min_luminance = (Data[9] * 10.0) + 1.0;

    png_set_mDCV(png_ptr, info_ptr,
                 white_x, white_y,
                 red_x, red_y,
                 green_x, green_y,
                 blue_x, blue_y,
                 max_luminance, min_luminance);

    double max_content_light = (Data[10] * 100.0) + 100.0;
    double max_frame_avg_light = (Data[11] * 10.0) + 10.0;
    png_set_cLLI(png_ptr, info_ptr, max_content_light, max_frame_avg_light);

    double gamma = (Data[12] / 255.0) + 0.1;
    png_set_gAMA(png_ptr, info_ptr, gamma);

    // Fix: Ensure unit is strictly 0 or 1 to avoid "Invalid sCAL unit" error
    int unit = (Data[13] % 2);
    double width = (Data[14] / 255.0) + 0.1;
    double height = (Data[15] / 255.0) + 0.1;
    
    // Use setjmp/longjmp to handle potential errors from libpng gracefully
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_sCAL(png_ptr, info_ptr, unit, width, height);
        png_write_info(png_ptr, info_ptr);
        png_write_end(png_ptr, info_ptr);
    }

    png_destroy_write_struct(&png_ptr, (png_infopp)&info_ptr);

    return 0;
}