// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_chunk at pngwutil.c:209:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

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

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid data
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just prevent the default behavior of aborting
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    // Create dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }

    // Create png_struct for writing
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    // Create png_info
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

    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Set custom write functions to avoid actual file I/O during fuzzing
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);

    // Set basic image parameters (using small fixed values for fuzzing)
    png_set_IHDR(png_ptr, info_ptr, 
                 8, 8,                    // width, height
                 8,                       // bit depth
                 PNG_COLOR_TYPE_RGB,      // color type
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    // Test png_write_info
    png_write_info(png_ptr, info_ptr);

    // Test png_set_flush with value derived from input
    int nrows = Data[0] % 100;  // Limit to reasonable values
    png_set_flush(png_ptr, nrows);

    // Prepare row data for writing
    png_byte row[24];  // 8 pixels * 3 bytes per pixel (RGB)
    size_t row_size = sizeof(row);
    
    // Fill row with data from input, cycling if needed
    for (size_t i = 0; i < row_size; i++) {
        row[i] = Data[i % Size];
    }

    // Test png_write_row
    png_write_row(png_ptr, row);

    // Test png_write_rows
    png_byte *rows_array[3];
    for (int i = 0; i < 3; i++) {
        rows_array[i] = row;
    }
    png_uint_32 num_rows = (Data[1] % 3) + 1;  // 1-3 rows
    png_write_rows(png_ptr, rows_array, num_rows);

    // Test png_write_chunk if we have enough data
    if (Size >= 14) {
        // Use bytes 2-5 for chunk name (must be 4 bytes)
        const png_byte chunk_name[4] = {
            static_cast<png_byte>(Data[2]),
            static_cast<png_byte>(Data[3]),
            static_cast<png_byte>(Data[4]),
            static_cast<png_byte>(Data[5])
        };
        
        // Use remaining data for chunk data
        const png_byte *chunk_data = Data + 6;
        size_t chunk_length = Size - 6;
        if (chunk_length > 1000) {
            chunk_length = 1000;  // Limit chunk size
        }
        
        png_write_chunk(png_ptr, chunk_name, chunk_data, chunk_length);
    }

    // Test png_write_flush
    png_write_flush(png_ptr);

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    
    // Remove the dummy file
    remove("./dummy_file");

    return 0;
}