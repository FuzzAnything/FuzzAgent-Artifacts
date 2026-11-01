// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <cstring>
#include <cstdio>

extern "C" int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write fuzzing data to a dummy file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    // We use png_read_info which may fail on malformed data, handled by setjmp
    png_read_info(png_ptr, info_ptr);

    // Close the file after reading info
    fclose(fp);

    // Test png_get_gAMA_fixed
    png_fixed_point gamma_value = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_value);

    // Test png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);

    // Test png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Test png_get_uint_16 with fuzzing data (if size >= 2)
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val; // Suppress unused variable warning
    }

    // Test png_get_hIST
    png_uint_16 *hist = NULL;
    png_get_hIST(png_ptr, info_ptr, &hist);

    // Test png_get_tIME
    png_time mod_time;
    png_timep mod_time_ptr = &mod_time;
    // The function expects a pointer to a png_timep (png_time_struct **), so we pass &mod_time_ptr
    png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}