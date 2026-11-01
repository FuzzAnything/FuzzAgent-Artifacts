// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
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
#include <setjmp.h>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to extract all parameters safely
    if (Size < 4) {
        return 0;
    }

    // Create a dummy file for output
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }

    // Create write structure
    png_structp png_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        dummy_error_fn,
        dummy_warning_fn
    );

    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Initialize error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    // Initialize the file pointer
    png_init_io(png_ptr, fp);

    // Extract fuzzing parameters from input safely
    int window_bits = Data[0];
    int level = Data[1];
    int allowed = Data[2];
    int mem_level = Data[3];

    // Fuzz png_set_text_compression_window_bits
    png_set_text_compression_window_bits(png_ptr, window_bits);

    // Fuzz png_set_compression_level
    png_set_compression_level(png_ptr, level);

    // Fuzz png_set_benign_errors
    png_set_benign_errors(png_ptr, allowed);

    // Fuzz png_set_compression_window_bits
    png_set_compression_window_bits(png_ptr, window_bits);

    // Fuzz png_reset_zstream (deprecated but tested)
    png_reset_zstream(png_ptr);

    // Fuzz png_set_text_compression_mem_level
    png_set_text_compression_mem_level(png_ptr, mem_level);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}