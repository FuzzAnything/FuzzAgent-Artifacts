// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
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
#include <stdlib.h>
#include <string.h>

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    png_longjmp(png_ptr, 1);
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, NULL);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_set_write_fn(png_ptr, NULL, dummy_write_fn, NULL);

    // Initialize basic image parameters
    png_uint_32 width = 100;
    png_uint_32 height = 100;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    int interlace_type = PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_type = PNG_FILTER_TYPE_BASE;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_type, compression_type, filter_type);

    // Fuzz APNG specific functions
    png_uint_32 num_frames, num_plays;
    png_uint_32 frame_width, frame_height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;

    // Test png_get_acTL
    png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);

    // Test png_get_next_frame_fcTL
    png_get_next_frame_fcTL(png_ptr, info_ptr, &frame_width, &frame_height, &x_offset, &y_offset,
                            &delay_num, &delay_den, &dispose_op, &blend_op);

    // Test png_get_next_frame_width
    png_uint_32 next_width = png_get_next_frame_width(png_ptr, info_ptr);
    (void)next_width;

    // Test png_set_next_frame_fcTL with fuzzed data
    if (Size >= 20) {
        frame_width = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
        frame_height = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
        x_offset = Data[8] | (Data[9] << 8) | (Data[10] << 16) | (Data[11] << 24);
        y_offset = Data[12] | (Data[13] << 8) | (Data[14] << 16) | (Data[15] << 24);
        delay_num = Data[16] | (Data[17] << 8);
        delay_den = Data[18] | (Data[19] << 8);
        dispose_op = Data[20 % Size];
        blend_op = Data[21 % Size];

        png_set_next_frame_fcTL(png_ptr, info_ptr, frame_width, frame_height, x_offset, y_offset,
                                delay_num, delay_den, dispose_op, blend_op);
    }

    // Test png_set_first_frame_is_hidden
    png_byte is_hidden = (Size > 22) ? Data[22 % Size] : 0;
    png_set_first_frame_is_hidden(png_ptr, info_ptr, is_hidden);

    // Test png_write_frame_head
    png_byte **row_pointers = NULL;
    if (Size > 23) {
        row_pointers = (png_byte **)malloc(sizeof(png_byte *) * (Size - 23));
        if (row_pointers) {
            for (size_t i = 0; i < Size - 23; ++i) {
                row_pointers[i] = (png_byte *)malloc(Size);
                if (row_pointers[i]) {
                    memcpy(row_pointers[i], &Data[23], Size);
                }
            }
        }
    }

    png_write_frame_head(png_ptr, info_ptr, row_pointers, frame_width, frame_height,
                         x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);

    // Cleanup
    if (row_pointers) {
        for (size_t i = 0; i < Size - 23; ++i) {
            free(row_pointers[i]);
        }
        free(row_pointers);
    }

    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}