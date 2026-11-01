// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
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
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

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

    png_set_write_fn(png_ptr, NULL, dummy_write_data, dummy_flush);
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    int interlace_method = PNG_INTERLACE_NONE;
    int compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_method = PNG_FILTER_TYPE_DEFAULT;

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method);

    if (Size >= 10) {
        png_uint_32 num_exif = Data[0];
        if (num_exif > 0 && num_exif <= Size - 1) {
            png_byte *exif_data = (png_byte*)(Data + 1);
            png_set_eXIf_1(png_ptr, info_ptr, num_exif, exif_data);
        }
    }

    if (Size >= 5) {
        png_byte colour_primaries = Data[1];
        png_byte transfer_function = Data[2];
        png_byte matrix_coefficients = Data[3];
        png_byte video_full_range_flag = Data[4];
        png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag);
    }

    png_write_info_before_PLTE(png_ptr, info_ptr);
    png_write_info(png_ptr, info_ptr);

    png_byte **row_pointers = (png_byte **)png_malloc(png_ptr, height * sizeof(png_byte *));
    if (!row_pointers) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    for (png_uint_32 y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
        if (!row_pointers[y]) {
            for (png_uint_32 i = 0; i < y; i++) {
                png_free(png_ptr, row_pointers[i]);
            }
            png_free(png_ptr, row_pointers);
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        memset(row_pointers[y], 0, png_get_rowbytes(png_ptr, info_ptr));
    }

    png_set_rows(png_ptr, info_ptr, row_pointers);

    png_write_image(png_ptr, row_pointers);

    for (png_uint_32 y = 0; y < height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    return 0;
}