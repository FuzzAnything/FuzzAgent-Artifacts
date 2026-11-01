// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
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
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void free_png_resources(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        // Should not happen if png_ptr is valid, but safety check
        png_destroy_read_struct(nullptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write input data to a temporary file
    write_dummy_png(Data, Size);

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize libpng read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling to avoid crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        free_png_resources(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG signature and header
    png_read_info(png_ptr, info_ptr);

    // Fuzz the target API functions
    // These functions are safe to call even if data is missing; they return 0 in such cases.
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm; // Suppress unused variable warning

    png_uint_32 ppi = png_get_pixels_per_inch(png_ptr, info_ptr);
    (void)ppi;

    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    png_uint_32 y_ppi = png_get_y_pixels_per_inch(png_ptr, info_ptr);
    (void)y_ppi;

    png_uint_32 y_ppm = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    (void)y_ppm;

    png_uint_32 x_ppi = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    (void)x_ppi;

    // Cleanup
    free_png_resources(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}