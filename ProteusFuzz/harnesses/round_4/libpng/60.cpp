// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
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

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        } else {
            // Write a minimal valid PNG header if input is empty to ensure parsing starts
            unsigned char header[8] = {137, 80, 78, 71, 13, 10, 26, 10};
            fwrite(header, 1, 8, f);
        }
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the signature and IHDR
    png_read_info(png_ptr, info_ptr);

    // Fuzz target: png_get_gAMA_fixed
    png_fixed_point gamma_val;
    png_uint_32 gamma_ret = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);
    (void)gamma_ret;

    // Fuzz target: png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // Fuzz target: png_get_x_pixels_per_meter
    png_uint_32 xppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)xppm;

    // Fuzz target: png_get_uint_16
    // We create a small buffer from the input data to test this function
    if (Size >= 2) {
        png_uint_16 val16 = png_get_uint_16(Data);
        (void)val16;
    }

    // Fuzz target: png_get_hIST
    png_uint_16 *hist_ptr = nullptr;
    png_uint_32 hist_ret = png_get_hIST(png_ptr, info_ptr, &hist_ptr);
    (void)hist_ret;
    (void)hist_ptr;

    // Fuzz target: png_get_tIME
    // png_get_tIME expects a pointer to a png_timep (which is png_time_struct*)
    // So we need a png_time_struct variable, and pass the address of a pointer to it
    png_time_struct time_val;
    png_timep time_ptr = &time_val;
    png_uint_32 time_ret = png_get_tIME(png_ptr, info_ptr, &time_ptr);
    (void)time_ret;

    // Cleanup
    safe_cleanup(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}