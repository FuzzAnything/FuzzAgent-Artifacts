// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_frame_head at pngread.c:195:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
// png_get_next_frame_delay_num at pngget.c:1469:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "png.h"

// Memory read structure for libpng
struct mem_read_data {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Memory write structure for libpng
struct mem_write_data {
    uint8_t* data;
    size_t size;
    size_t offset;
};

// Custom read function for libpng
static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    mem_read_data* mem = (mem_read_data*)png_get_io_ptr(png_ptr);
    if (mem->offset + length > mem->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    memcpy(data, mem->data + mem->offset, length);
    mem->offset += length;
}

// Custom write function for libpng
static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    mem_write_data* mem = (mem_write_data*)png_get_io_ptr(png_ptr);
    if (mem->offset + length > mem->size) {
        // Reallocate buffer if needed
        size_t new_size = mem->size * 2 + length;
        uint8_t* new_data = (uint8_t*)realloc(mem->data, new_size);
        if (!new_data) {
            png_error(png_ptr, "Memory allocation failed");
            return;
        }
        mem->data = new_data;
        mem->size = new_size;
    }
    memcpy(mem->data + mem->offset, data, length);
    mem->offset += length;
}

// Custom flush function for libpng
static void user_flush_data(png_structp png_ptr) {
    // Do nothing for memory writes
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 100) {
        return 0;
    }

    // Part 1: Test reading functions
    {
        // Create read structure
        png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            return 0;
        }

        // Create info structure
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_read_struct(&png_ptr, NULL, NULL);
            return 0;
        }

        // Set up error handling
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
            goto writing_part;
        }

        // Set up custom read function
        mem_read_data mem_data = {Data, Size, 0};
        png_set_read_fn(png_ptr, &mem_data, user_read_data);

        // Read PNG info
        png_read_info(png_ptr, info_ptr);

        // Try to read frame head if it's an APNG
        png_read_frame_head(png_ptr, info_ptr);

        // Test get functions
        png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
        png_uint_16 delay_num = 0, delay_den = 0;
        png_byte dispose_op = 0, blend_op = 0;

        png_get_next_frame_fcTL(png_ptr, info_ptr, &width, &height, &x_offset, &y_offset,
                               &delay_num, &delay_den, &dispose_op, &blend_op);

        png_get_next_frame_delay_den(png_ptr, info_ptr);
        png_get_next_frame_delay_num(png_ptr, info_ptr);
        png_get_next_frame_height(png_ptr, info_ptr);

        // Cleanup
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    }

writing_part:
    // Part 2: Test writing functions
    {
        // Create write structure
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            return 0;
        }

        // Create info structure
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, NULL);
            return 0;
        }

        // Set up error handling
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }

        // Set up custom write function
        mem_write_data mem_data;
        mem_data.size = 4096;
        mem_data.data = (uint8_t*)malloc(mem_data.size);
        mem_data.offset = 0;
        if (!mem_data.data) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }

        png_set_write_fn(png_ptr, &mem_data, user_write_data, user_flush_data);

        // Set IHDR first (required before setting fcTL)
        png_set_IHDR(png_ptr, info_ptr, 
                     (png_uint_32)Data[0] << 24 | (png_uint_32)Data[1] << 16 | (png_uint_32)Data[2] << 8 | Data[3],
                     (png_uint_32)Data[4] << 24 | (png_uint_32)Data[5] << 16 | (png_uint_32)Data[6] << 8 | Data[7],
                     8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                     PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

        // Set acTL for APNG
        png_set_acTL(png_ptr, info_ptr, 2, 0);

        // Test set_next_frame_fcTL with data from input
        size_t data_idx = 8;
        png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
        png_uint_16 delay_num = 0, delay_den = 0;
        png_byte dispose_op = 0, blend_op = 0;

        if (Size - data_idx >= 20) {
            width = (png_uint_32)Data[data_idx] << 24 | (png_uint_32)Data[data_idx+1] << 16 | 
                    (png_uint_32)Data[data_idx+2] << 8 | Data[data_idx+3];
            height = (png_uint_32)Data[data_idx+4] << 24 | (png_uint_32)Data[data_idx+5] << 16 | 
                     (png_uint_32)Data[data_idx+6] << 8 | Data[data_idx+7];
            x_offset = (png_uint_32)Data[data_idx+8] << 24 | (png_uint_32)Data[data_idx+9] << 16 | 
                       (png_uint_32)Data[data_idx+10] << 8 | Data[data_idx+11];
            y_offset = (png_uint_32)Data[data_idx+12] << 24 | (png_uint_32)Data[data_idx+13] << 16 | 
                       (png_uint_32)Data[data_idx+14] << 8 | Data[data_idx+15];
            delay_num = (png_uint_16)Data[data_idx+16] << 8 | Data[data_idx+17];
            delay_den = (png_uint_16)Data[data_idx+18] << 8 | Data[data_idx+19];
            dispose_op = Data[data_idx+20] % 4;  // Valid values: 0-3
            blend_op = Data[data_idx+21] % 2;    // Valid values: 0-1
        }

        png_set_next_frame_fcTL(png_ptr, info_ptr, width, height, x_offset, y_offset,
                               delay_num, delay_den, dispose_op, blend_op);

        // Write info
        png_write_info(png_ptr, info_ptr);

        // Cleanup
        free(mem_data.data);
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }

    return 0;
}