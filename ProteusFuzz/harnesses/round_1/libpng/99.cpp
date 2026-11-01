// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <png.h>

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_set_write_fn(png_ptr, NULL, dummy_write_data, dummy_flush);

    // Extract parameters from fuzz input
    int window_bits = (int)(Data[0] & 0xFF);
    int level = (int)(Data[1] & 0xFF);
    int mem_level = (int)(Data[1] & 0xFF);
    int strategy = (int)(Data[1] & 0xFF);
    int nrows = (int)(Data[1] & 0xFF);
    int method = 0; // Only valid value is 0
    int filters = (int)(Data[1] & 0xFF);

    // Call target functions with diverse inputs
    png_set_compression_window_bits(png_ptr, window_bits);
    png_set_compression_level(png_ptr, level);
    png_set_compression_mem_level(png_ptr, mem_level);
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_flush(png_ptr, nrows);
    png_set_filter(png_ptr, method, filters);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}