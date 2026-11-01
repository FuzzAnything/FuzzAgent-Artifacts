// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_io_state at pngget.c:1334:1 in png.h
// png_get_io_chunk_type at pngget.c:1340:1 in png.h
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
#include <string.h>
#include <stdlib.h>

static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static int dummy_read_user_chunk(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
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

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_set_read_fn(png_ptr, (void*)Data, dummy_read_data);
    png_set_read_user_chunk_fn(png_ptr, (void*)Data, dummy_read_user_chunk);

    void *error_ptr = png_get_error_ptr(png_ptr);
    (void)error_ptr;

    void *io_ptr = png_get_io_ptr(png_ptr);
    (void)io_ptr;

    png_uint_32 io_state = png_get_io_state(png_ptr);
    (void)io_state;

    png_uint_32 chunk_type = png_get_io_chunk_type(png_ptr);
    (void)chunk_type;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}