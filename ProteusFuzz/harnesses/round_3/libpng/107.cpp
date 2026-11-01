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
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
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

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file for libpng to read from
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes during parsing
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Open the dummy file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        safe_cleanup(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Fuzz target API functions

    // 1. png_get_gAMA_fixed
    png_fixed_point gamma_value;
    png_uint_32 gama_ret = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_value);
    (void)gama_ret; // Suppress unused variable warning

    // 2. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // 3. png_get_hIST
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_ret = png_get_hIST(png_ptr, info_ptr, &hist);
    (void)hist_ret;
    if (hist) {
        // hist is allocated by libpng, do not free manually
        // Just access to ensure no crash
        (void)hist[0]; 
    }

    // 4. png_get_tIME
    png_time mod_time;
    png_timep mod_time_ptr = &mod_time;
    png_uint_32 time_ret = png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);
    (void)time_ret;

    // 5. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // 6. png_get_uint_16
    // We need a buffer of at least 2 bytes. Use the input data if available.
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val;
    }

    // Cleanup
    fclose(fp);
    safe_cleanup(png_ptr, info_ptr);

    return 0;
}