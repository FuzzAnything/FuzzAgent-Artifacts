// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_write_status_fn at pngwrite.c:1348:1 in png.h
// png_set_write_user_transform_fn at pngwrite.c:1360:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_get_current_row_number at pngtrans.c:847:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
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

#include "code/png.h"

static void dummy_write_status_fn(png_structp png_ptr, png_uint_32 row, int pass) {
    // Dummy callback for write status
    (void)png_ptr;
    (void)row;
    (void)pass;
}

static void dummy_user_transform_fn(png_structp png_ptr, png_row_infop row_info, png_bytep data) {
    // Dummy user transform function
    (void)png_ptr;
    (void)row_info;
    (void)data;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that discards data
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_405(const uint8_t *Data, size_t Size) {
    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

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

    // Set up write functions
    png_set_write_fn(png_ptr, fp, dummy_write_fn, dummy_flush_fn);

    // Set image parameters based on fuzzer input
    png_uint_32 width = 64;
    png_uint_32 height = 64;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    
    if (Size >= 4) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        if (width == 0) width = 1;
        if (height == 0) height = 1;
        if (width > 1000) width = 1000;
        if (height > 1000) height = 1000;
    }

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Call target API functions in various combinations
    if (Size > 0) {
        // Test png_set_bgr
        if (Data[0] & 0x01) {
            png_set_bgr(png_ptr);
        }

        // Test png_set_write_status_fn
        if (Data[0] & 0x02) {
            png_set_write_status_fn(png_ptr, dummy_write_status_fn);
        }

        // Test png_set_write_user_transform_fn
        if (Data[0] & 0x04) {
            png_set_write_user_transform_fn(png_ptr, dummy_user_transform_fn);
        }

        // Test png_write_info
        png_write_info(png_ptr, info_ptr);

        // Test png_write_rows
        if (Size > 1) {
            // Allocate row pointers
            png_bytep row_pointers[height];
            png_byte row_data[width * 3]; // RGB
            
            // Initialize with fuzzer data
            for (png_uint_32 y = 0; y < height; y++) {
                for (size_t x = 0; x < sizeof(row_data); x++) {
                    size_t data_idx = (y * sizeof(row_data) + x) % Size;
                    row_data[x] = Data[data_idx];
                }
                row_pointers[y] = row_data;
            }

            // Write rows in different patterns based on fuzzer input
            if (Data[0] & 0x08) {
                // Write all rows at once
                png_write_rows(png_ptr, row_pointers, height);
            } else {
                // Write rows one by one
                for (png_uint_32 y = 0; y < height; y++) {
                    png_write_rows(png_ptr, &row_pointers[y], 1);
                }
            }
        }

        // Test png_get_current_row_number
        if (Data[0] & 0x10) {
            png_uint_32 current_row = png_get_current_row_number(png_ptr);
            (void)current_row; // Use result to avoid unused variable warning
        }
    }

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    
    // Remove dummy file
    remove("./dummy_file");

    return 0;
}