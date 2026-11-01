// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_flush at pngwrite.c:978:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create dummy file if needed (though libpng usually writes to stdout or custom IO)
    // Here we just ensure the file exists for any potential side effects
    write_dummy_file(Data, Size);

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int ret = 0;

    // Initialize libpng structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Use setjmp/longjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Extract fuzzing data for parameters
    // Ensure we have enough data for basic parameters
    if (Size < 4) return 0;

    png_uint_32 width = Data[0];
    png_uint_32 height = Data[1];
    int bit_depth = Data[2] % 16; // 1-16
    int color_type = Data[3] % 10; // 0-9

    // Avoid invalid combinations that might crash early
    if (width == 0 || height == 0) {
        width = 1;
        height = 1;
    }

    // Fuzz png_set_IHDR
    // Parameters: png_ptr, info_ptr, width, height, bit_depth, color_type,
    //             interlace_method, compression_method, filter_method
    int interlace_method = (Size > 4) ? (Data[4] % 2) : 0;
    int compression_method = (Size > 5) ? (Data[5] % 2) : 0;
    int filter_method = (Size > 6) ? (Data[6] % 2) : 0;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);

    // Fuzz png_set_filter
    // Parameters: png_ptr, method, filters
    int method = (Size > 7) ? (Data[7] % 2) : 0;
    int filters = (Size > 8) ? (Data[8] % 256) : 0;
    png_set_filter(png_ptr, method, filters);

    // Fuzz png_set_flush
    // Parameters: png_ptr, nrows
    int nrows = (Size > 9) ? (int)Data[9] : 0;
    png_set_flush(png_ptr, nrows);

    // Fuzz png_set_gamma_fixed
    // Parameters: png_ptr, screen_gamma, override_file_gamma
    // Use fixed point values (multiplied by 100000)
    png_fixed_point screen_gamma = (Size > 10) ? (png_fixed_point)Data[10] * 1000 : 100000;
    png_fixed_point override_file_gamma = (Size > 11) ? (png_fixed_point)Data[11] * 1000 : 100000;
    // Ensure non-zero to avoid errors
    if (screen_gamma <= 0) screen_gamma = 100000;
    if (override_file_gamma <= 0) override_file_gamma = 100000;
    png_set_gamma_fixed(png_ptr, screen_gamma, override_file_gamma);

    // Fuzz png_write_flush
    // This requires a valid write context, but we haven't opened a file yet.
    // It will likely do nothing or return safely if not initialized for writing.
    png_write_flush(png_ptr);

    // Fuzz png_write_png
    // This function requires image data and a valid file handle.
    // Since we are fuzzing the API behavior and not necessarily generating valid PNGs,
    // we can call it. It will likely fail gracefully if no file is opened or data is missing.
    // We pass a dummy transform and params.
    int transforms = (Size > 12) ? (int)Data[12] : 0;
    void *params = (Size > 13) ? (void*)&Data[13] : NULL;
    png_write_png(png_ptr, info_ptr, transforms, params);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}