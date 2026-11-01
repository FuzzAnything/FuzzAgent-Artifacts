// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_tIME at pngget.c:1184:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
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
#include <setjmp.h>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return NULL;
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) return NULL;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return NULL;
    }
    return info_ptr;
}

static void cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr && *png_ptr) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
    }
}

extern "C" int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_png(Data, Size);

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        cleanup_png(&png_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header
    png_read_info(png_ptr, info_ptr);

    // Fuzz target API: png_get_gAMA_fixed
    png_fixed_point gamma_val = 0;
    png_uint_32 gama_result = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);
    (void)gama_result;

    // Fuzz target API: png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // Fuzz target API: png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // Fuzz target API: png_get_uint_16
    if (Size >= 2) {
        png_uint_16 val16 = png_get_uint_16(Data);
        (void)val16;
    }

    // Fuzz target API: png_get_hIST
    png_uint_16 *hist = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_result;
    (void)hist;

    // Fuzz target API: png_get_tIME
    // The third argument must be a pointer to a png_timep (which is png_time*)
    png_time mod_time_struct;
    png_timep mod_time = &mod_time_struct;
    png_uint_32 time_result = png_get_tIME(png_ptr, info_ptr, &mod_time);
    (void)time_result;
    (void)mod_time;

    // Cleanup
    cleanup_png(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}