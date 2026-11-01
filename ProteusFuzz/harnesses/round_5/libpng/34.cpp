// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
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

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size < 9) return 0;

    // Create a dummy file if needed
    write_dummy_file(Data, Size);

    // Initialize PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp/longjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Initialize IO
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        png_init_io(png_ptr, fp);
        fclose(fp);
    }

    // Extract parameters from fuzz input safely
    int method = Data[0];
    int strategy = Data[1];
    int filter_method = Data[2];
    int filter_flags = (Data[3] << 8) | Data[4];
    int option_idx = Data[5];
    int option_onoff = Data[6];
    int window_bits = Data[7];
    int mem_level = Data[8];

    // Fuzz png_set_compression_method
    png_set_compression_method(png_ptr, method);

    // Fuzz png_set_compression_strategy
    png_set_compression_strategy(png_ptr, strategy);

    // Fuzz png_set_filter
    png_set_filter(png_ptr, filter_method, filter_flags);

    // Fuzz png_set_option
    int prev_state = png_set_option(png_ptr, option_idx, option_onoff);
    (void)prev_state;

    // Fuzz png_set_text_compression_window_bits
    png_set_text_compression_window_bits(png_ptr, window_bits);

    // Fuzz png_set_compression_mem_level
    png_set_compression_mem_level(png_ptr, mem_level);

    // Cleanup
    cleanup_png(png_ptr, info_ptr);

    return 0;
}