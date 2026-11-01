// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <png.h>
#include <setjmp.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    // Extract values from input data to drive fuzzing
    int level = (Data[0] % 10); // 0-9
    int window_bits = 8 + (Data[1] % 8); // 8-15
    int mem_level = 1 + (Data[2] % 9); // 1-9
    int method = 0; // Only valid method is 0
    int filters = Data[3]; // Any byte for filters
    int strategy = Data[4] % 5; // zlib strategies: 0-4

    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp/longjmp for error handling as required by libpng
    // Access jmpbuf via the public API or direct access if available in the version
    // In newer libpng versions, direct access to jmpbuf is restricted, so we use the macro if available
    // or assume the standard setjmp usage pattern.
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png_struct(png_ptr, info_ptr);
        return 0;
    }

    // Fuzz png_set_text_compression_level
    png_set_text_compression_level(png_ptr, level);

    // Fuzz png_set_text_compression_window_bits
    png_set_text_compression_window_bits(png_ptr, window_bits);

    // Fuzz png_set_compression_level
    png_set_compression_level(png_ptr, level);

    // Fuzz png_set_filter
    png_set_filter(png_ptr, method, filters);

    // Fuzz png_set_compression_mem_level
    png_set_compression_mem_level(png_ptr, mem_level);

    // Fuzz png_set_text_compression_strategy
    png_set_text_compression_strategy(png_ptr, strategy);

    // Cleanup
    cleanup_png_struct(png_ptr, info_ptr);

    return 0;
}