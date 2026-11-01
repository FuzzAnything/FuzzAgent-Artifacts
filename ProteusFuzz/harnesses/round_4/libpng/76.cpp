// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_write_sig at pngwutil.c:68:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
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

static void safe_cleanup(png_structp *png_ptr, png_infop *info_ptr) {
    if (png_ptr && *png_ptr) {
        png_destroy_write_struct(png_ptr, info_ptr);
    } else if (info_ptr && *info_ptr) {
        return;
    }
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    int fuzz_choice = Data[0] % 6;

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        safe_cleanup(&png_ptr, &info_ptr);
        return 0;
    }

    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush);
    png_set_error_fn(png_ptr, nullptr, dummy_error, dummy_warning);

    // Initialize minimal image info to allow some write operations
    png_uint_32 width = 1;
    png_uint_32 height = 1;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Prepare dummy row data
    png_bytep row = (png_bytep)png_malloc(png_ptr, png_get_rowbytes(png_ptr, info_ptr));
    if (!row) {
        safe_cleanup(&png_ptr, &info_ptr);
        return 0;
    }
    memset(row, 0, png_get_rowbytes(png_ptr, info_ptr));

    switch (fuzz_choice) {
        case 0: // png_write_sig
            png_write_sig(png_ptr);
            break;
        case 1: // png_write_flush
            png_write_flush(png_ptr);
            break;
        case 2: // png_set_flush
            {
                int nrows = 0;
                if (Size > 1) {
                    nrows = (Data[1] % 100) - 50; // Range -50 to 49
                }
                png_set_flush(png_ptr, nrows);
            }
            break;
        case 3: // png_write_png
            png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, nullptr);
            break;
        case 4: // png_write_end
            // Need to write at least one row to avoid error in some configurations
            png_write_info(png_ptr, info_ptr);
            png_write_row(png_ptr, row);
            png_write_end(png_ptr, info_ptr);
            break;
        case 5: // png_write_chunk_end
            // This requires a chunk start, which is complex to fuzz safely without full context
            // We skip direct invocation to avoid undefined behavior or crashes
            // Instead, we rely on other functions that might internally trigger chunk logic
            png_write_info(png_ptr, info_ptr);
            png_write_row(png_ptr, row);
            png_write_end(png_ptr, info_ptr);
            break;
    }

    png_free(png_ptr, row);
    safe_cleanup(&png_ptr, &info_ptr);
    return 0;
}