// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
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
#include <png.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void write_dummy_png_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    // Need at least 6 bytes to safely extract all parameters
    if (Size < 6) return 0;

    // Create a dummy file for input if needed
    write_dummy_png_file(Data, Size);

    // Initialize libpng write structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Use setjmp/longjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize IO
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    png_init_io(png_ptr, fp);
    fclose(fp);

    // Extract parameters from fuzz input safely
    int level = Data[0] % 10; // 0-9
    int mem_level = (Data[1] % 9) + 1; // 1-9
    int strategy = Data[2] % 4; // 0-3 (Z_DEFAULT_STRATEGY to Z_FIXED)
    int text_mem_level = (Data[3] % 9) + 1; // 1-9
    int window_bits = (Data[4] % 8) + 8; // 8-15
    int text_level = Data[5] % 10; // 0-9

    // Fuzz target API functions
    png_set_compression_level(png_ptr, level);
    png_set_compression_mem_level(png_ptr, mem_level);
    png_set_compression_strategy(png_ptr, strategy);
    png_set_text_compression_mem_level(png_ptr, text_mem_level);
    png_set_text_compression_window_bits(png_ptr, window_bits);
    png_set_text_compression_level(png_ptr, text_level);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}