// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL at pngset.c:664:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
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
#include <stdexcept>
#include <cmath>

static void safe_png_destroy(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Create a dummy file for output
    const char *filename = "./dummy_file";
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    // Initialize the file pointer
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        safe_png_destroy(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Set basic image parameters
    png_set_IHDR(png_ptr, info_ptr, 16, 16, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, 
                 PNG_FILTER_TYPE_DEFAULT);

    // Parse input data to generate diverse test values
    double white_x = (double)(Data[0] & 0xFF) / 255.0;
    double white_y = (double)(Data[1] & 0xFF) / 255.0;
    double red_x = (double)(Data[2] & 0xFF) / 255.0;
    double red_y = (double)(Data[3] & 0xFF) / 255.0;
    double green_x = (double)(Data[4] & 0xFF) / 255.0;
    double green_y = (double)(Data[5] & 0xFF) / 255.0;
    double blue_x = (double)(Data[6] & 0xFF) / 255.0;
    double blue_y = (double)(Data[7] & 0xFF) / 255.0;
    
    double max_lum = (double)(Data[8] & 0xFF) * 1000.0;
    double min_lum = (double)(Data[9] & 0xFF) * 10.0;
    
    double max_content_light = (double)(Size % 10000) * 100.0;
    double max_frame_avg = (double)(Size % 5000) * 10.0;
    
    double gamma = 0.1 + (double)(Data[0] & 0xFF) / 255.0 * 2.0;
    
    int unit = Data[0] % 2; // 0 or 1
    double width = 0.001 + (double)(Data[1] & 0xFF) / 1000.0;
    double height = 0.001 + (double)(Data[2] & 0xFF) / 1000.0;

    // Fuzz png_set_mDCV
    try {
        png_set_mDCV(png_ptr, info_ptr, white_x, white_y, red_x, red_y, 
                     green_x, green_y, blue_x, blue_y, max_lum, min_lum);
    } catch (...) {
        // Ignore exceptions
    }

    // Fuzz png_set_cLLI
    try {
        png_set_cLLI(png_ptr, info_ptr, max_content_light, max_frame_avg);
    } catch (...) {
        // Ignore exceptions
    }

    // Fuzz png_set_gAMA
    try {
        png_set_gAMA(png_ptr, info_ptr, gamma);
    } catch (...) {
        // Ignore exceptions
    }

    // Fuzz png_set_sCAL
    try {
        png_set_sCAL(png_ptr, info_ptr, unit, width, height);
    } catch (...) {
        // Ignore exceptions
    }

    // Write info
    try {
        png_write_info(png_ptr, info_ptr);
    } catch (...) {
        // Ignore exceptions
    }

    // Write dummy image data
    png_bytep row = (png_bytep)png_malloc(png_ptr, 16 * 3);
    memset(row, 0, 16 * 3);
    for (int i = 0; i < 16; ++i) {
        png_write_row(png_ptr, row);
    }
    png_free(png_ptr, row);

    // Write end
    try {
        png_write_end(png_ptr, info_ptr);
    } catch (...) {
        // Ignore exceptions
    }

    // Cleanup
    safe_png_destroy(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}