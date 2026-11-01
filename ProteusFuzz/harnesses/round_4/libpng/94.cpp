// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <fstream>

static void write_dummy_png(const uint8_t *data, size_t size, const char *filename) {
    // This function is kept for potential future use but not strictly needed
    // if we write raw data directly.
    (void)data;
    (void)size;
    (void)filename;
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }

    const char *filename = "./dummy_file";
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_uint_32 width = 0;
    png_fixed_point gamma = 0;
    png_uint_16 *hist = NULL;
    png_time mod_time;
    png_timep time_ptr = &mod_time;
    png_uint_32 x_ppm = 0;
    png_uint_16 val_16 = 0;

    // Initialize PNG read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the file
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Read the header
    png_read_info(png_ptr, info_ptr);

    // Test png_get_image_width
    width = png_get_image_width(png_ptr, info_ptr);

    // Test png_get_gAMA_fixed
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma);

    // Test png_get_x_pixels_per_meter
    x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Test png_get_hIST
    png_get_hIST(png_ptr, info_ptr, &hist);

    // Test png_get_tIME
    // The function expects a pointer to a png_timep (which is png_time*),
    // so we pass the address of our time_ptr variable.
    png_get_tIME(png_ptr, info_ptr, &time_ptr);

    // Test png_get_uint_16 with input data if available
    if (Size >= 2) {
        val_16 = png_get_uint_16(Data);
    }

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}