// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_write_user_transform_fn at pngwrite.c:1360:1 in png.h
// png_set_write_status_fn at pngwrite.c:1348:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
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
#include <zlib.h>

static void user_transform_callback(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

static void write_status_callback(png_structp png_ptr, png_uint_32 row_number, int pass) {
    (void)png_ptr;
    (void)row_number;
    (void)pass;
}

static void write_data_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE *f = fopen("./dummy_file", "ab");
    if (f) {
        fwrite(data, 1, length, f);
        fclose(f);
    }
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    remove("./dummy_file");

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    png_set_write_fn(png_ptr, NULL, write_data_callback, NULL);

    size_t buffer_size = (Size > 0) ? Data[0] * 1024 : 1024;
    if (buffer_size == 0) buffer_size = 1024;
    png_set_compression_buffer_size(png_ptr, buffer_size);

    int strategy = (Size > 1) ? (Data[1] % 6) : Z_DEFAULT_STRATEGY;
    png_set_compression_strategy(png_ptr, strategy);

    png_set_write_user_transform_fn(png_ptr, user_transform_callback);
    png_set_write_status_fn(png_ptr, write_status_callback);

    size_t current_buffer_size = png_get_compression_buffer_size(png_ptr);
    (void)current_buffer_size;

    png_bytep row = (png_bytep)malloc(1024);
    if (!row) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    memset(row, 0, 1024);

    png_row_info row_info;
    row_info.width = 1;
    row_info.bit_depth = 8;
    row_info.color_type = PNG_COLOR_TYPE_RGB;
    row_info.channels = 3;
    row_info.pixel_depth = 24;
    row_info.rowbytes = 3;

    png_write_row(png_ptr, row);
    png_write_flush(png_ptr);

    free(row);
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}