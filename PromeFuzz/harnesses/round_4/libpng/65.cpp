// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_flush at pngwrite.c:966:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_data(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error function to prevent abort
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning function
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;

    // Initialize write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Initialize info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return 0;
    }

    // Set up write function
    png_set_write_fn(png_ptr, fp, dummy_write_data, dummy_flush_data);

    // Set basic image info (minimal valid PNG)
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Use fuzzer data to determine which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;

    switch (selector) {
        case 0: { // png_write_chunk
            if (remaining_size >= 4) {
                png_byte chunk_name[4];
                memcpy(chunk_name, remaining_data, 4);
                size_t chunk_data_len = remaining_size > 4 ? remaining_size - 4 : 0;
                const png_byte* chunk_data = remaining_data + 4;
                png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_data_len);
            }
            break;
        }
        case 1: // png_write_info
            png_write_info(png_ptr, info_ptr);
            break;
        case 2: { // png_set_flush
            int nrows = 0;
            if (remaining_size >= 1) {
                nrows = remaining_data[0];
            }
            png_set_flush(png_ptr, nrows);
            break;
        }
        case 3: { // png_write_row
            if (remaining_size >= 1) {
                png_byte row[1] = {0};
                if (remaining_size >= 1) {
                    row[0] = remaining_data[0];
                }
                png_write_info(png_ptr, info_ptr); // Must be called before write_row
                png_write_row(png_ptr, row);
            }
            break;
        }
        case 4: { // png_write_rows
            if (remaining_size >= 1) {
                png_byte row_data[1] = {0};
                if (remaining_size >= 1) {
                    row_data[0] = remaining_data[0];
                }
                png_byte* row_pointers[1] = {row_data};
                png_write_info(png_ptr, info_ptr); // Must be called before write_rows
                png_write_rows(png_ptr, row_pointers, 1);
            }
            break;
        }
        case 5: // png_write_flush
            png_write_flush(png_ptr);
            break;
    }

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);

    return 0;
}