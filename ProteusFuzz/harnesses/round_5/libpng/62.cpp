// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
#include <png.h>
#include <setjmp.h>
#include <cstdint>
#include <cstddef>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    // Extract parameters from fuzz input
    int mem_level = Data[0] % 10; // 0-9
    int method = Data[1] % 10;    // 0-9
    int strategy = Data[2] % 10;  // 0-9
    int window_bits = Data[3] % 20; // 0-19
    int comp_mem_level = Data[4] % 10; // 0-9
    int comp_level = Data[5] % 10; // 0-9

    // Create png_struct for writing
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        dummy_error_fn,
        dummy_warning_fn
    );

    if (!png_ptr) {
        return 0;
    }

    // Create png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Invoke target API functions
    png_set_text_compression_mem_level(png_ptr, mem_level);
    png_set_text_compression_method(png_ptr, method);
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_text_compression_window_bits(png_ptr, window_bits);
    png_set_compression_mem_level(png_ptr, comp_mem_level);
    png_set_text_compression_level(png_ptr, comp_level);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}