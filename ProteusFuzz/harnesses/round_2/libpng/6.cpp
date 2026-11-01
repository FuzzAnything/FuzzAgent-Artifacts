// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_filter at pngwrite.c:1058:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <setjmp.h>

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

// Custom error handler to prevent crashes during fuzzing
static void fuzz_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do nothing, just return to avoid aborting
}

static void fuzz_warning_handler(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
    // Do nothing, just return
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create write struct
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_error_handler, fuzz_warning_handler);
    if (!png_ptr) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Set up error handling to prevent crashes from png_error calls
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        // If an error occurred, clean up and return
        cleanup_png(png_ptr, info_ptr);
        return 0;
    }

    // Extract parameters from fuzz input safely
    int method = Data[0];
    int filter_method = Size > 1 ? Data[1] : 0;
    int filters = Size > 2 ? Data[2] : 0;
    int mem_level = Size > 3 ? Data[3] : 1;
    int strategy = Size > 4 ? Data[4] : 0;
    int option = Size > 5 ? Data[5] : 0;
    int onoff = Size > 6 ? Data[6] : 0;
    int window_bits = Size > 7 ? Data[7] : 8;

    // 1. png_set_compression_method
    png_set_compression_method(png_ptr, method);

    // 2. png_set_filter
    // Only method 0 is valid. Invalid methods trigger png_error.
    // We wrap in setjmp to catch the error.
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_filter(png_ptr, filter_method, filters);
    }

    // 3. png_set_compression_mem_level
    png_set_compression_mem_level(png_ptr, mem_level);

    // 4. png_set_compression_strategy
    png_set_compression_strategy(png_ptr, strategy);

    // 5. png_set_option
    int prev_state = png_set_option(png_ptr, option, onoff);
    (void)prev_state;

    // 6. png_set_text_compression_window_bits
    png_set_text_compression_window_bits(png_ptr, window_bits);

    // Cleanup
    cleanup_png(png_ptr, info_ptr);

    return 0;
}