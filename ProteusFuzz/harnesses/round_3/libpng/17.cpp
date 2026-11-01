// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
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

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size, f);
        }
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create dummy file for I/O
    write_dummy_file(Data, Size);

    // Initialize libpng write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Open dummy file for writing
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Extract fuzz data to generate parameters safely
    // Ensure we have enough data before accessing indices
    int method = 8; // Default valid method
    int level = 6;  // Default valid level
    int strategy = 0; // Default strategy
    int window_bits = 15; // Default window bits
    int num_palette = 1;
    int max_colors = 1;
    int full_quantize = 0;

    if (Size >= 2) {
        method = (Data[0] % 10) + (Data[1] % 2 == 0 ? 0 : 100);
    }
    if (Size >= 3) {
        level = Data[2] % 10;
    }
    if (Size >= 4) {
        strategy = Data[3] % 5;
    }
    if (Size >= 5) {
        window_bits = (Data[4] % 10) + 6;
    }
    if (Size >= 6) {
        num_palette = (Data[5] % 256) + 1;
    }
    if (Size >= 7) {
        max_colors = (Data[6] % 256) + 1;
    }
    if (Size >= 8) {
        full_quantize = Data[7] % 2;
    }

    // Prepare palette and histogram
    // Limit allocation size to prevent huge allocations
    if (num_palette > 1024) num_palette = 1024;
    if (max_colors > 1024) max_colors = 1024;

    png_color *palette = new png_color[num_palette];
    png_uint_16 *histogram = new png_uint_16[num_palette];
    
    // Initialize palette and histogram safely
    for (int i = 0; i < num_palette; ++i) {
        palette[i].red = (Size > 0) ? Data[(8 + i * 3) % Size] : 0;
        palette[i].green = (Size > 0) ? Data[(9 + i * 3) % Size] : 0;
        palette[i].blue = (Size > 0) ? Data[(10 + i * 3) % Size] : 0;
        histogram[i] = (Size > 0) ? (png_uint_16)(Data[(11 + i * 3) % Size] * 256) : 0;
    }

    // Call target API functions with fuzzed parameters
    png_set_compression_method(png_ptr, method);
    png_set_compression_level(png_ptr, level);
    png_set_compression_strategy(png_ptr, strategy);
    png_set_compression_window_bits(png_ptr, window_bits);
    png_set_quantize(png_ptr, palette, num_palette, max_colors, histogram, full_quantize);

    // Attempt to flush (requires prior row write, but we call it anyway to test safety)
    png_write_flush(png_ptr);

    // Cleanup
    delete[] palette;
    delete[] histogram;
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}