// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
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
#include <png.h>
#include <zlib.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr != nullptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    int window_bits = Data[0];
    int level = Data[1];
    int strategy = Data[Size / 2];
    int mem_level = Data[Size - 1];

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png_ptr == nullptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        cleanup_png(png_ptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Initialize a dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp == nullptr) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Fuzz target functions
    png_set_text_compression_window_bits(png_ptr, window_bits);
    png_set_compression_level(png_ptr, level);
    png_set_compression_window_bits(png_ptr, window_bits);
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_text_compression_mem_level(png_ptr, mem_level);

    // Test deprecated function
    png_reset_zstream(png_ptr);

    // Cleanup
    fclose(fp);
    cleanup_png(png_ptr, info_ptr);

    return 0;
}