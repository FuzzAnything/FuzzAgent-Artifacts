// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void setup_png_structs(png_structp *png_ptr, png_infop *info_ptr) {
    *png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (*png_ptr == NULL) return;

    *info_ptr = png_create_info_struct(*png_ptr);
    if (*info_ptr == NULL) {
        png_destroy_read_struct(png_ptr, NULL, NULL);
        *png_ptr = NULL;
        return;
    }

    if (setjmp(png_jmpbuf(*png_ptr)) != 0) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
        return;
    }
}

static void cleanup_png_structs(png_structp *png_ptr, png_infop *info_ptr) {
    if (*png_ptr != NULL) {
        png_destroy_read_struct(png_ptr, info_ptr, NULL);
        *png_ptr = NULL;
        *info_ptr = NULL;
    }
}

extern "C" int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    setup_png_structs(&png_ptr, &info_ptr);
    if (png_ptr == NULL || info_ptr == NULL) {
        return 0;
    }

    // Write input data to a dummy file
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "wb");
    if (f == NULL) {
        cleanup_png_structs(&png_ptr, &info_ptr);
        return 0;
    }
    fwrite(Data, 1, Size, f);
    fclose(f);

    // Initialize reading from file
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png_structs(&png_ptr, &info_ptr);
        return 0;
    }

    f = fopen(dummy_file, "rb");
    if (f == NULL) {
        cleanup_png_structs(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, f);

    // Read the PNG signature and IHDR
    png_read_info(png_ptr, info_ptr);

    // Close file after reading info
    fclose(f);

    // 1. Fuzz png_get_hIST
    png_uint_16 *hist = NULL;
    png_get_hIST(png_ptr, info_ptr, &hist);
    if (hist != NULL) {
        // Just access to ensure no crash
        (void)hist[0];
    }

    // 2. Fuzz png_get_tIME
    png_timep mod_time = NULL;
    png_get_tIME(png_ptr, info_ptr, &mod_time);
    if (mod_time != NULL) {
        (void)mod_time->year;
    }

    // 3. Fuzz png_get_gAMA_fixed
    png_fixed_point int_file_gamma = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    (void)int_file_gamma;

    // 4. Fuzz png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // 5. Fuzz png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // 6. Fuzz png_get_uint_16 with input data
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val;
    }

    // Cleanup
    cleanup_png_structs(&png_ptr, &info_ptr);

    // Remove dummy file
    std::remove(dummy_file);

    return 0;
}