// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_progressive_read_fn at pngpread.c:1109:1 in png.h
// png_get_progressive_ptr at pngpread.c:1136:1 in png.h
// png_process_data_skip at pngpread.c:92:1 in png.h
// png_process_data at pngpread.c:50:1 in png.h
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
#include <cstdio>

static void info_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

static void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass) {
    (void)png_ptr;
    (void)new_row;
    (void)row_num;
    (void)pass;
}

static void end_callback(png_structp png_ptr, png_infop info_ptr) {
    (void)png_ptr;
    (void)info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Test png_set_progressive_read_fn
    png_set_progressive_read_fn(png_ptr, (void*)Data, info_callback, row_callback, end_callback);

    // Test png_get_progressive_ptr
    void *ptr = png_get_progressive_ptr(png_ptr);
    (void)ptr;

    // Test png_process_data_skip (deprecated, always returns 0)
    png_uint_32 skip = png_process_data_skip(png_ptr);
    (void)skip;

    // Test png_process_data with varying buffer sizes
    size_t offset = 0;
    while (offset < Size) {
        size_t chunk_size = (Size - offset) > 1024 ? 1024 : (Size - offset);
        if (chunk_size == 0) break;

        png_process_data(png_ptr, info_ptr, const_cast<png_byte*>(Data + offset), chunk_size);
        offset += chunk_size;

        // Test png_process_data_pause inside the loop
        size_t remaining = png_process_data_pause(png_ptr, 1);
        (void)remaining;
    }

    // Test png_progressive_combine_row with dummy data
    png_byte old_row[1024] = {0};
    png_byte new_row[1024] = {0};
    if (Size > 0) {
        new_row[0] = Data[0];
    }
    png_progressive_combine_row(png_ptr, old_row, new_row);
    png_progressive_combine_row(png_ptr, old_row, nullptr);

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}