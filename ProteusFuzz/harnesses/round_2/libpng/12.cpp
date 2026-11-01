// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_next_frame_delay_num at pngget.c:1469:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
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
#include <setjmp.h>

static void fuzzer_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void fuzzer_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void fuzzer_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void fuzzer_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    int ret;
    png_uint_32 width, height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    png_byte **row_pointers = nullptr;
    png_uint_32 w, h;
    png_uint_16 dn, dd;
    png_byte do_op, bo_op;

    // Initialize png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzzer_error_fn, fuzzer_warning_fn);
    if (!png_ptr) return 0;

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

    // Initialize basic info for testing
    width = 1;
    height = 1;
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Test 1: png_get_next_frame_delay_num with valid pointers (should return 0 if no acTL)
    png_uint_16 num = png_get_next_frame_delay_num(png_ptr, info_ptr);
    (void)num;

    // Test 2: png_get_next_frame_delay_den with valid pointers
    png_uint_16 den = png_get_next_frame_delay_den(png_ptr, info_ptr);
    (void)den;

    // Test 3: png_read_frame_head (will likely fail without proper file data, but safe to call)
    png_read_frame_head(png_ptr, info_ptr);

    // Test 4: png_get_next_frame_fcTL
    ret = png_get_next_frame_fcTL(png_ptr, info_ptr, &width, &height, &x_offset, &y_offset, &delay_num, &delay_den, &dispose_op, &blend_op);
    (void)ret;

    // Test 5: png_set_next_frame_fcTL
    ret = png_set_next_frame_fcTL(png_ptr, info_ptr, width, height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);
    (void)ret;

    // Test 6: png_write_frame_head
    // Allocate dummy row pointers for write test
    row_pointers = (png_byte **)malloc(sizeof(png_byte *) * height);
    if (row_pointers) {
        for (png_uint_32 i = 0; i < height; ++i) {
            row_pointers[i] = (png_byte *)malloc(width * 3); // RGB
            if (row_pointers[i]) {
                memset(row_pointers[i], 0, width * 3);
            }
        }
    }

    // Setup write struct for png_write_frame_head test
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzzer_error_fn, fuzzer_warning_fn);
    if (png_ptr_write) {
        png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
        if (info_ptr_write) {
            if (setjmp(png_jmpbuf(png_ptr_write))) {
                png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
                png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
                if (row_pointers) {
                    for (png_uint_32 i = 0; i < height; ++i) {
                        free(row_pointers[i]);
                    }
                    free(row_pointers);
                }
                return 0;
            }

            png_set_IHDR(png_ptr_write, info_ptr_write, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
            png_set_write_fn(png_ptr_write, nullptr, fuzzer_write_data, nullptr);

            // Call png_write_frame_head
            png_write_frame_head(png_ptr_write, info_ptr_write, row_pointers, width, height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);

            png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        } else {
            png_destroy_write_struct(&png_ptr_write, nullptr);
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    if (row_pointers) {
        for (png_uint_32 i = 0; i < height; ++i) {
            free(row_pointers[i]);
        }
        free(row_pointers);
    }

    return 0;
}