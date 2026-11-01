// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
// png_process_data_skip at pngpread.c:92:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_process_data_pause at pngpread.c:65:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
// png_progressive_combine_row at pngpread.c:1093:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstddef>
#include <cstdio>

static void
progressive_info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void
progressive_row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

static void
progressive_end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void
progressive_combine_row_callback(png_structp png_ptr, png_bytep old_row, const png_bytep new_row) {
    (void)png_ptr;
    (void)old_row;
    (void)new_row;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    void *user_ptr = (void *)Data;
    png_set_progressive_read_fn(png_ptr, user_ptr,
                                progressive_info_callback,
                                progressive_row_callback,
                                progressive_end_callback);

    void *retrieved_ptr = png_get_progressive_ptr(png_ptr);
    (void)retrieved_ptr;

    png_process_data(png_ptr, info_ptr, (png_byte *)Data, Size);

    png_uint_32 skip_bytes = png_process_data_skip(png_ptr);
    (void)skip_bytes;

    size_t remaining_bytes = png_process_data_pause(png_ptr, 1);
    (void)remaining_bytes;

    remaining_bytes = png_process_data_pause(png_ptr, 0);
    (void)remaining_bytes;

    png_byte old_row[256] = {0};
    png_progressive_combine_row(png_ptr, old_row, (const png_byte *)Data);
    png_progressive_combine_row(png_ptr, old_row, NULL);

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}