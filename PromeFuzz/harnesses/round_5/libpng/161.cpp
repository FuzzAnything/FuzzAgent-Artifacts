// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_write_info_before_PLTE at pngwrite.c:83:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_row at pngwrite.c:756:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_image at pngwrite.c:659:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
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
#include <fstream>

#include "code/png.h"

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy writer
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_data(png_structp png_ptr) {
    // Do nothing - dummy flush
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    // Initialize dummy file
    std::ofstream dummy_file("./dummy_file", std::ios::binary);
    if (!dummy_file.is_open()) {
        return 0;
    }
    dummy_file.write(reinterpret_cast<const char*>(Data), Size);
    dummy_file.close();

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
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);

    // Initialize basic image info from fuzz data
    // Use small dimensions to avoid excessive memory usage
    png_set_IHDR(png_ptr, info_ptr,
                 (Size > 0) ? (Data[0] % 64 + 1) : 16,  // width: 1-64
                 (Size > 1) ? (Data[1] % 64 + 1) : 16,  // height: 1-64
                 (Size > 2) ? ((Data[2] % 4 + 1) * 2) : 8,  // bit_depth: 2,4,6,8
                 (Size > 3) ? (Data[3] % 6) : PNG_COLOR_TYPE_GRAY,  // color_type: 0-5
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    // Set rowbytes based on width and bit depth
    png_byte bit_depth = (Size > 2) ? ((Data[2] % 4 + 1) * 2) : 8;
    png_byte color_type = (Size > 3) ? (Data[3] % 6) : PNG_COLOR_TYPE_GRAY;
    png_uint_32 width = (Size > 0) ? (Data[0] % 64 + 1) : 16;
    
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    if (rowbytes == 0) {
        // Calculate manually if png_get_rowbytes fails
        int channels = 1;
        switch (color_type) {
            case PNG_COLOR_TYPE_GRAY:
                channels = 1; break;
            case PNG_COLOR_TYPE_GRAY_ALPHA:
                channels = 2; break;
            case PNG_COLOR_TYPE_PALETTE:
                channels = 1; break;
            case PNG_COLOR_TYPE_RGB:
                channels = 3; break;
            case PNG_COLOR_TYPE_RGB_ALPHA:
                channels = 4; break;
        }
        rowbytes = (width * channels * bit_depth + 7) / 8;
    }

    // Allocate row data
    png_byte** row_pointers = nullptr;
    png_uint_32 height = (Size > 1) ? (Data[1] % 64 + 1) : 16;
    
    if (height > 0 && rowbytes > 0) {
        row_pointers = static_cast<png_byte**>(png_malloc(png_ptr, height * sizeof(png_byte*)));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = static_cast<png_byte*>(png_malloc(png_ptr, rowbytes));
            // Fill with fuzz data (wrap around if needed)
            for (size_t x = 0; x < rowbytes; x++) {
                row_pointers[y][x] = (x + y < Size) ? Data[x + y] : 0;
            }
        }
    }

    // Test different write sequences based on fuzz data
    if (Size > 4) {
        // Use Data[4] to determine which functions to call
        uint8_t selector = Data[4];
        
        if (selector & 0x01) {
            // Call png_write_info_before_PLTE
            png_write_info_before_PLTE(png_ptr, info_ptr);
        }
        
        if (selector & 0x02) {
            // Call png_write_info
            png_write_info(png_ptr, info_ptr);
        }
        
        if (selector & 0x04 && row_pointers) {
            // Call png_write_image
            png_write_image(png_ptr, row_pointers);
        }
        
        if (selector & 0x08 && row_pointers) {
            // Call png_write_row for each row
            for (png_uint_32 y = 0; y < height; y++) {
                png_write_row(png_ptr, row_pointers[y]);
            }
        }
        
        if (selector & 0x10) {
            // Call png_write_end
            png_write_end(png_ptr, info_ptr);
        }
    } else {
        // Default sequence
        png_write_info(png_ptr, info_ptr);
        if (row_pointers) {
            png_write_image(png_ptr, row_pointers);
        }
        png_write_end(png_ptr, info_ptr);
    }

    // Clean up row data
    if (row_pointers) {
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }

    // Clean up PNG structures
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}