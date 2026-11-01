// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
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
#include <cstddef>
#include <cstdlib>
#include <cstring>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else if (info_ptr) {
        // If png_ptr is null but info_ptr exists (should not happen in normal flow),
        // we cannot safely destroy info_ptr without png_ptr in libpng API.
        // This case is handled by ensuring we always create png_ptr first.
    }
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for writing
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }

    // Initialize png_struct and png_info
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Set up error handling to avoid crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Extract fuzzing parameters from Data
    int method = Data[0];
    int mem_level = Data[1 % Size];
    int strategy = Data[2 % Size];
    int window_bits = Data[3 % Size];
    int level = Data[4 % Size];

    // Invoke target API functions with fuzzed inputs
    png_set_text_compression_method(png_ptr, method);
    png_set_compression_method(png_ptr, method);
    png_set_text_compression_mem_level(png_ptr, mem_level);
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_text_compression_window_bits(png_ptr, window_bits);
    png_set_text_compression_level(png_ptr, level);

    // Write minimal PNG header to ensure the functions are exercised in context
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Attempt to write info (may trigger errors if settings are invalid, caught by setjmp)
    png_write_info(png_ptr, info_ptr);

    // Write a single row of dummy data
    png_byte row_data[3]; // 1 pixel, RGB
    memset(row_data, 0xFF, sizeof(row_data));
    png_write_row(png_ptr, row_data);

    // Finish writing
    png_write_end(png_ptr, info_ptr);

    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}