// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
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
#include <stddef.h>
#include <cstdio>
#include <cstring>

static void write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    const char *filename = "./dummy_file";
    write_dummy_png(Data, Size, filename);

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    FILE *fp = fopen(filename, "rb");
    if (!fp) return 0;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
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

    png_init_io(png_ptr, fp);

    // Attempt to read the header and info
    png_read_info(png_ptr, info_ptr);

    // Invoke target API functions
    png_uint_32 y_ppm = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    png_uint_32 x_ppi = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    png_uint_32 ppi = png_get_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);

    // Use values to avoid unused variable warnings
    (void)y_ppm;
    (void)x_ppi;
    (void)x_ppm;
    (void)ppi;
    (void)y_ppi;
    (void)ppm;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
}