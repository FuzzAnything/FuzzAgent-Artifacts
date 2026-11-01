// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_get_next_frame_delay_num at pngget.c:1469:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <cstdlib>

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_data_fn(png_structp png_ptr, png_bytep data, size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_read_data_fn(png_structp png_ptr, png_bytep data, size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    int ret = 0;

    // Initialize png_struct
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize write struct for write-related functions
    png_structp png_write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_write_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    png_infop info_write_ptr = png_create_info_struct(png_write_ptr);
    if (!info_write_ptr) {
        png_destroy_write_struct(&png_write_ptr, nullptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_write_ptr))) {
        png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize read function pointers
    png_set_read_fn(png_ptr, nullptr, dummy_read_data_fn);
    // Initialize write function pointers
    png_set_write_fn(png_write_ptr, nullptr, dummy_write_data_fn, nullptr);

    // Parse input data for parameters
    png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
    png_uint_16 delay_num = 0, delay_den = 0;
    png_byte dispose_op = 0, blend_op = 0;

    if (Size >= 24) {
        width = *(const png_uint_32*)(Data + 0);
        height = *(const png_uint_32*)(Data + 4);
        x_offset = *(const png_uint_32*)(Data + 8);
        y_offset = *(const png_uint_32*)(Data + 12);
        delay_num = *(const png_uint_16*)(Data + 16);
        delay_den = *(const png_uint_16*)(Data + 18);
        dispose_op = Data[20];
        blend_op = Data[21];
    }

    // Limit values to reasonable ranges to avoid extreme allocations
    if (width > 10000) width = 10000;
    if (height > 10000) height = 10000;
    if (x_offset > 10000) x_offset = 10000;
    if (y_offset > 10000) y_offset = 10000;
    if (delay_num > 65535) delay_num = 65535;
    if (delay_den > 65535) delay_den = 65535;

    // Test png_get_next_frame_fcTL
    png_uint_32 out_width, out_height, out_x_offset, out_y_offset;
    png_uint_16 out_delay_num, out_delay_den;
    png_byte out_dispose_op, out_blend_op;
    ret = png_get_next_frame_fcTL(png_ptr, info_ptr, &out_width, &out_height, &out_x_offset, &out_y_offset,
                                  &out_delay_num, &out_delay_den, &out_dispose_op, &out_blend_op);
    (void)ret;

    // Test png_get_next_frame_delay_num
    png_uint_16 dnum = png_get_next_frame_delay_num(png_ptr, info_ptr);
    (void)dnum;

    // Test png_get_next_frame_delay_den
    png_uint_16 dden = png_get_next_frame_delay_den(png_ptr, info_ptr);
    (void)dden;

    // Test png_set_next_frame_fcTL
    png_uint_32 set_ret = png_set_next_frame_fcTL(png_write_ptr, info_write_ptr, width, height, x_offset, y_offset,
                                                  delay_num, delay_den, dispose_op, blend_op);
    (void)set_ret;

    // Test png_write_frame_head
    // Prepare dummy row pointers
    png_byte **row_pointers = nullptr;
    if (width > 0 && height > 0) {
        row_pointers = (png_byte **)malloc(height * sizeof(png_byte *));
        if (row_pointers) {
            for (png_uint_32 i = 0; i < height; ++i) {
                row_pointers[i] = (png_byte *)malloc(width);
                if (row_pointers[i]) {
                    memset(row_pointers[i], 0, width);
                }
            }
        }
    }

    if (row_pointers) {
        png_write_frame_head(png_write_ptr, info_write_ptr, row_pointers, width, height, x_offset, y_offset,
                             delay_num, delay_den, dispose_op, blend_op);
        for (png_uint_32 i = 0; i < height; ++i) {
            free(row_pointers[i]);
        }
        free(row_pointers);
    }

    // Test png_read_frame_head
    png_read_frame_head(png_ptr, info_ptr);

    // Cleanup
    png_destroy_write_struct(&png_write_ptr, &info_write_ptr);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}