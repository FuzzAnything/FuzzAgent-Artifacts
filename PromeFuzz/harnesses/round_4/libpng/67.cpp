// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rows at pngset.c:1845:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_get_rows at pngget.c:50:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "png.h"

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 32) {
        return 0;
    }

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Set up dummy write functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_fn, dummy_flush_fn);

    // Extract parameters from fuzzer input
    uint32_t width = 0, height = 0;
    if (Size >= 8) {
        memcpy(&width, Data, 4);
        memcpy(&height, Data + 4, 4);
        width = width % 256 + 1;  // Limit to reasonable size
        height = height % 256 + 1;
    }

    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    int interlace_type = PNG_INTERLACE_NONE;
    int compression_type = PNG_COMPRESSION_TYPE_DEFAULT;
    int filter_type = PNG_FILTER_TYPE_DEFAULT;

    // Set image parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_type, compression_type, filter_type);

    // Create dummy row data
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if (rowbytes == 0 || rowbytes > 1024 * 1024) {  // Sanity check
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    std::vector<png_byte> row_data(rowbytes * height);
    std::vector<png_byte*> rows(height);

    // Fill row data with fuzzer input (cycling if needed)
    for (size_t i = 0; i < row_data.size(); i++) {
        row_data[i] = Data[i % Size];
    }

    // Set up row pointers
    for (size_t i = 0; i < height; i++) {
        rows[i] = row_data.data() + i * rowbytes;
    }

    // Call target functions based on fuzzer input
    uint8_t function_selector = Data[0] % 6;

    switch (function_selector) {
        case 0:
            // Test png_set_rows
            png_set_rows(png_ptr, info_ptr, rows.data());
            break;

        case 1:
            // Test png_get_rowbytes
            png_get_rowbytes(png_ptr, info_ptr);
            break;

        case 2:
            // Test png_write_image
            png_write_info(png_ptr, info_ptr);
            png_write_image(png_ptr, rows.data());
            png_write_end(png_ptr, info_ptr);
            break;

        case 3:
            // Test png_get_rows (simulate read scenario)
            {
                png_set_rows(png_ptr, info_ptr, rows.data());
                png_byte** retrieved_rows = png_get_rows(png_ptr, info_ptr);
                (void)retrieved_rows;  // Use result to avoid unused variable warning
            }
            break;

        case 4:
            // Test png_write_row
            png_write_info(png_ptr, info_ptr);
            for (size_t i = 0; i < height; i++) {
                png_write_row(png_ptr, rows[i]);
            }
            png_write_end(png_ptr, info_ptr);
            break;

        case 5:
            // Test png_write_rows
            png_write_info(png_ptr, info_ptr);
            png_write_rows(png_ptr, rows.data(), height);
            png_write_end(png_ptr, info_ptr);
            break;
    }

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}