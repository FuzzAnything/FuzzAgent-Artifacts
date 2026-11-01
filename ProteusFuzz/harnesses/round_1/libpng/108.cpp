// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void fuzz_png_file(const uint8_t *data, size_t size) {
    if (size == 0) return;

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Read the signature and IHDR chunk
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_hIST
    png_uint_16 *hist = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_result;
    (void)hist;

    // Fuzz png_get_tIME
    png_timep mod_time = NULL;
    png_uint_32 time_result = png_get_tIME(png_ptr, info_ptr, &mod_time);
    (void)time_result;
    (void)mod_time;

    // Fuzz png_get_gAMA_fixed
    png_fixed_point file_gamma = 0;
    png_uint_32 gama_result = png_get_gAMA_fixed(png_ptr, info_ptr, &file_gamma);
    (void)gama_result;
    (void)file_gamma;

    // Fuzz png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // Fuzz png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // Fuzz png_get_uint_16 with input data if size >= 2
    if (size >= 2) {
        png_uint_16 val = png_get_uint_16(data);
        (void)val;
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    fuzz_png_file(Data, Size);
    return 0;
}