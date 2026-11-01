// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_frame_tail at pngwrite.c:1529:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
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

static png_voidp dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

static png_voidp dummy_realloc_fn(png_structp png_ptr, png_voidp ptr, png_size_t size) {
    (void)png_ptr;
    return realloc(ptr, size);
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    int result = 0;

    // Initialize png_struct
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

    // Set custom memory and write functions
    png_set_mem_fn(png_ptr, nullptr, dummy_malloc_fn, dummy_free_fn);
    png_set_write_fn(png_ptr, nullptr, dummy_write_data_fn, nullptr);

    // Initialize with some basic data to simulate a valid PNG context
    png_uint_32 width = (Data[0] % 100) + 1;
    png_uint_32 height = (Data[1] % 100) + 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Fuzz png_write_frame_head
    png_byte **row_pointers = (png_byte **)malloc(sizeof(png_byte *) * height);
    if (row_pointers) {
        for (png_uint_32 y = 0; y < height; ++y) {
            row_pointers[y] = (png_byte *)malloc(width * 3);
            if (row_pointers[y]) {
                memset(row_pointers[y], 0, width * 3);
            }
        }

        png_uint_32 x_offset = (Data[2] % width);
        png_uint_32 y_offset = (Data[3] % height);
        png_uint_16 delay_num = (Data[4] % 100) + 1;
        png_uint_16 delay_den = (Data[5] % 100) + 1;
        png_byte dispose_op = Data[6] % 2;
        png_byte blend_op = Data[7] % 2;

        png_write_frame_head(png_ptr, info_ptr, row_pointers, width, height,
                             x_offset, y_offset, delay_num, delay_den,
                             dispose_op, blend_op);

        // Fuzz png_write_frame_tail
        png_write_frame_tail(png_ptr, info_ptr);

        // Cleanup row_pointers
        for (png_uint_32 y = 0; y < height; ++y) {
            free(row_pointers[y]);
        }
        free(row_pointers);
    }

    // Fuzz png_set_next_frame_fcTL
    png_uint_32 new_width = (Data[8] % 100) + 1;
    png_uint_32 new_height = (Data[9] % 100) + 1;
    png_uint_32 new_x_offset = (Data[10] % new_width);
    png_uint_32 new_y_offset = (Data[11] % new_height);
    png_uint_16 new_delay_num = (Data[12] % 100) + 1;
    png_uint_16 new_delay_den = (Data[13] % 100) + 1;
    png_byte new_dispose_op = Data[14] % 2;
    png_byte new_blend_op = Data[15] % 2;

    png_set_next_frame_fcTL(png_ptr, info_ptr, new_width, new_height,
                            new_x_offset, new_y_offset, new_delay_num, new_delay_den,
                            new_dispose_op, new_blend_op);

    // Fuzz png_get_next_frame_fcTL
    png_uint_32 out_width, out_height, out_x_offset, out_y_offset;
    png_uint_16 out_delay_num, out_delay_den;
    png_byte out_dispose_op, out_blend_op;

    result = png_get_next_frame_fcTL(png_ptr, info_ptr, &out_width, &out_height,
                                     &out_x_offset, &out_y_offset, &out_delay_num, &out_delay_den,
                                     &out_dispose_op, &out_blend_op);

    // Fuzz png_get_next_frame_delay_den
    png_uint_16 delay_den = png_get_next_frame_delay_den(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}