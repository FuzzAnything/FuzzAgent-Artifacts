// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
// png_get_next_frame_delay_num at pngget.c:1469:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <setjmp.h>

// Define constants if not present in the included png.h version
#ifndef PNG_DISPOSE_OP_NONE
#define PNG_DISPOSE_OP_NONE 0
#endif
#ifndef PNG_DISPOSE_OP_BACKGROUND
#define PNG_DISPOSE_OP_BACKGROUND 1
#endif
#ifndef PNG_DISPOSE_OP_PREVIOUS
#define PNG_DISPOSE_OP_PREVIOUS 2
#endif

#ifndef PNG_BLEND_OP_SOURCE
#define PNG_BLEND_OP_SOURCE 0
#endif
#ifndef PNG_BLEND_OP_OVER
#define PNG_BLEND_OP_OVER 1
#endif

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_read_row_fn(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

static void dummy_write_row_fn(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

static void dummy_progressive_info_fn(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_progressive_row_fn(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

static void dummy_progressive_end_fn(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_user_chunk_fn(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
}

static void dummy_frame_info_fn(png_structp png_ptr) {
    (void)png_ptr;
}

static void *dummy_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    int ret = 0;

    // Initialize png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    // Initialize png_info
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Configure read callbacks
    png_set_read_fn(png_ptr, nullptr, dummy_read_data_fn);

    // Set up some basic info to make the structure valid for APNG functions
    png_uint_32 width = 100;
    png_uint_32 height = 100;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    int interlace_type = PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_type = PNG_FILTER_TYPE_BASE;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type, compression_type, filter_type);

    // Test png_get_next_frame_delay_den
    png_uint_16 delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    (void)delay_den;

    // Test png_get_next_frame_delay_num
    png_uint_16 delay_num = png_get_next_frame_delay_num(png_ptr, info_ptr);
    (void)delay_num;

    // Test png_get_next_frame_fcTL
    png_uint_32 w, h, x_off, y_off;
    png_uint_16 d_num, d_den;
    png_byte disp_op, blend_op;
    ret = png_get_next_frame_fcTL(png_ptr, info_ptr, &w, &h, &x_off, &y_off, &d_num, &d_den, &disp_op, &blend_op);
    (void)ret;

    // Test png_set_next_frame_fcTL
    ret = png_set_next_frame_fcTL(png_ptr, info_ptr, 50, 50, 10, 10, 1, 10, PNG_DISPOSE_OP_NONE, PNG_BLEND_OP_SOURCE);
    (void)ret;

    // Cleanup read struct before switching to write
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    // Switch to write mode
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_set_write_fn(png_ptr, nullptr, dummy_write_data_fn, dummy_flush_fn);

    // Set IHDR for write
    png_set_IHDR(png_ptr, info_ptr, 100, 100, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_BASE);

    // Prepare dummy row pointers
    png_byte **row_pointers = (png_byte **)malloc(sizeof(png_byte *) * 100);
    if (!row_pointers) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    for (int i = 0; i < 100; ++i) {
        row_pointers[i] = (png_byte *)malloc(300);
        if (!row_pointers[i]) {
            for (int j = 0; j < i; ++j) free(row_pointers[j]);
            free(row_pointers);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        memset(row_pointers[i], 0, 300);
    }

    // Call png_write_frame_head
    png_write_frame_head(png_ptr, info_ptr, row_pointers, 100, 100, 0, 0, 1, 10, PNG_DISPOSE_OP_NONE, PNG_BLEND_OP_SOURCE);

    // Cleanup row pointers
    for (int i = 0; i < 100; ++i) free(row_pointers[i]);
    free(row_pointers);

    // Cleanup write struct
    png_destroy_write_struct(&png_ptr, &info_ptr);

    // Test png_read_frame_head
    // Re-initialize for read
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    png_set_read_fn(png_ptr, nullptr, dummy_read_data_fn);
    png_set_IHDR(png_ptr, info_ptr, 100, 100, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_BASE);

    // Call png_read_frame_head (may fail if acTL not present, but we test it)
    png_read_frame_head(png_ptr, info_ptr);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}