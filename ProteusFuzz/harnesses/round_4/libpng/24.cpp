// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_info_struct at png.c:396:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
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
#include <climits>

// Dummy callback functions required by libpng
static void
dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void
dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void
dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

static void
dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void
dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void
cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr && *png_ptr) {
        png_destroy_write_struct(png_ptr, info_ptr);
        *png_ptr = NULL;
    }
    if (info_ptr && *info_ptr) {
        png_destroy_info_struct(NULL, info_ptr);
        *info_ptr = NULL;
    }
}

static png_structp
create_png_struct(const uint8_t *data, size_t size, int is_write) {
    png_structp png_ptr;
    png_infop info_ptr = NULL;

    if (is_write) {
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    } else {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    }

    if (!png_ptr) {
        return NULL;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return NULL;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(&png_ptr, &info_ptr);
        return NULL;
    }

    if (is_write) {
        png_set_write_fn(png_ptr, NULL, dummy_write_data, dummy_flush);
    } else {
        png_set_read_fn(png_ptr, NULL, dummy_read_data);
    }

    // Set error/warning handlers to prevent abort
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    // Return only png_ptr, caller must manage info_ptr separately or pass it back
    // To simplify, we will not return info_ptr from this function, but the caller
    // will create and destroy it in the main loop if needed, or we handle it here.
    // Actually, for these specific APIs, info_ptr is not strictly required for the struct itself,
    // but png_create_info_struct allocates memory. We must ensure it's destroyed.
    // Let's destroy it here before returning, as the target APIs don't need it.
    png_destroy_info_struct(png_ptr, &info_ptr);

    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use first byte to decide read vs write mode
    int is_write = (Data[0] % 2);
    
    // Create png_struct
    png_structp png_ptr = create_png_struct(Data, Size, is_write);
    if (!png_ptr) {
        return 0;
    }

    // We don't strictly need info_ptr for these specific APIs, 
    // and we already cleaned it up in create_png_struct to avoid leaks.

    // Extract parameters from fuzz data
    int compression_level = (Size > 1) ? (Data[1] % 10) : 0; // 0-9
    int strategy = (Size > 2) ? (Data[2] % 5) : 0; // 0-4
    int window_bits = (Size > 3) ? (Data[3] % 16) + 8 : 12; // 8-15
    int method = (Size > 4) ? (Data[4] % 2) + 8 : 8; // 8 (deflate) or 9 (invalid)

    // Test png_set_compression_level
    png_set_compression_level(png_ptr, compression_level);

    // Test png_set_text_compression_strategy
    png_set_text_compression_strategy(png_ptr, strategy);

    // Test png_set_compression_window_bits
    png_set_compression_window_bits(png_ptr, window_bits);

    // Test png_set_text_compression_window_bits
    png_set_text_compression_window_bits(png_ptr, window_bits);

    // Test png_set_compression_method
    png_set_compression_method(png_ptr, method);

    // Test png_reset_zstream (Deprecated, but requested)
    // This function returns Z_STREAM_ERROR if png_ptr is NULL, but we have a valid ptr.
    // It might fail if the stream isn't initialized properly, which is fine for fuzzing.
    int reset_result = png_reset_zstream(png_ptr);
    (void)reset_result; // Suppress unused warning

    // Cleanup
    png_destroy_write_struct(&png_ptr, NULL);

    return 0;
}