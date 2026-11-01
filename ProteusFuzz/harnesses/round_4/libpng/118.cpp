// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
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

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_callback, dummy_warning_callback);

    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    png_read_info(png_ptr, info_ptr);

    // 1. png_get_gAMA_fixed
    png_fixed_point gamma_val;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);
    png_get_gAMA_fixed(png_ptr, info_ptr, NULL);

    // 2. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // 3. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // 4. png_get_uint_16
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val;
    }

    // 5. png_get_hIST
    png_uint_16 *hist = NULL;
    png_get_hIST(png_ptr, info_ptr, &hist);
    png_get_hIST(png_ptr, info_ptr, NULL);
    (void)hist;

    // 6. png_get_tIME
    // The function expects a pointer to a png_timep (which is png_time_struct*)
    // So we need png_timep *mod_time_ptr
    png_time_struct time_struct;
    png_timep time_ptr = &time_struct;
    png_get_tIME(png_ptr, info_ptr, &time_ptr);
    png_get_tIME(png_ptr, info_ptr, NULL);
    (void)time_struct;

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}