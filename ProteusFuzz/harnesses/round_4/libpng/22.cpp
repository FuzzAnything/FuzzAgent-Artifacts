// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
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

static void dummy_write(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Use the standard longjmp mechanism provided by libpng
    png_longjmp(png_ptr, 1);
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void* dummy_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void dummy_free(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Initialize libpng structures
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error, dummy_warning);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use setjmp to handle libpng errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Set dummy write function
    png_set_write_fn(png_ptr, nullptr, dummy_write, nullptr);

    // Extract parameters from fuzz input
    uint32_t width = (Data[0] << 8) | Data[1];
    uint32_t height = (Data[2] << 8) | Data[3];
    if (width == 0 || height == 0) {
        width = 1; height = 1;
    }
    if (width > 10000 || height > 10000) {
        width = 100; height = 100;
    }

    // Initialize basic image info
    png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Test png_set_first_frame_is_hidden
    png_byte is_hidden = (Size > 4) ? Data[4] % 2 : 0;
    png_set_first_frame_is_hidden(png_ptr, info_ptr, is_hidden);

    // Test png_set_next_frame_fcTL
    uint32_t frame_width = (Size > 5) ? Data[5] % width : width;
    uint32_t frame_height = (Size > 6) ? Data[6] % height : height;
    uint32_t x_offset = (Size > 7) ? Data[7] % (width - frame_width + 1) : 0;
    uint32_t y_offset = (Size > 8) ? Data[8] % (height - frame_height + 1) : 0;
    uint16_t delay_num = (Size > 9) ? Data[9] % 256 : 10;
    uint16_t delay_den = (Size > 10) ? Data[10] % 256 : 100;
    if (delay_den == 0) delay_den = 100;
    png_byte dispose_op = (Size > 11) ? Data[11] % 3 : 0;
    png_byte blend_op = (Size > 12) ? Data[12] % 2 : 0;

    png_set_next_frame_fcTL(png_ptr, info_ptr, frame_width, frame_height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);

    // Prepare row pointers for writing
    png_byte **row_pointers = (png_byte **)png_malloc(png_ptr, height * sizeof(png_byte *));
    if (!row_pointers) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    for (uint32_t y = 0; y < height; ++y) {
        row_pointers[y] = (png_byte *)png_malloc(png_ptr, width * 3);
        if (!row_pointers[y]) {
            // Cleanup partial allocation
            for (uint32_t k = 0; k < y; ++k) {
                png_free(png_ptr, row_pointers[k]);
            }
            png_free(png_ptr, row_pointers);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        memset(row_pointers[y], 0, width * 3);
    }

    // Test png_write_frame_head
    png_write_frame_head(png_ptr, info_ptr, row_pointers, frame_width, frame_height, x_offset, y_offset, delay_num, delay_den, dispose_op, blend_op);

    // Test png_get_acTL
    png_uint_32 num_frames = 0, num_plays = 0;
    png_get_acTL(png_ptr, info_ptr, &num_frames, &num_plays);

    // Test png_get_next_frame_height
    png_uint_32 next_height = png_get_next_frame_height(png_ptr, info_ptr);

    // Test png_get_next_frame_fcTL
    png_uint_32 g_width = 0, g_height = 0, g_x = 0, g_y = 0;
    png_uint_16 g_delay_num = 0, g_delay_den = 0;
    png_byte g_dispose = 0, g_blend = 0;
    png_get_next_frame_fcTL(png_ptr, info_ptr, &g_width, &g_height, &g_x, &g_y, &g_delay_num, &g_delay_den, &g_dispose, &g_blend);

    // Cleanup
    for (uint32_t y = 0; y < height; ++y) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}