// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function that does nothing
}

static void error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid data
}

static void warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 50) return 0; // Need minimum data for parameters

    // Create read structures
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_fn, warning_fn);
    if (!png_ptr_read) return 0;
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Set up dummy read functions
    png_set_read_fn(png_ptr_read, nullptr, dummy_read_fn);
    
    // Create write structures
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_fn, warning_fn);
    if (!png_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        return 0;
    }
    
    png_infop info_ptr_write = png_create_info_struct(png_ptr_write);
    if (!info_ptr_write) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, nullptr);
        return 0;
    }
    
    // Set up dummy write functions
    png_set_write_fn(png_ptr_write, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr_read)) || setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // Extract parameters from fuzzing data
    png_uint_32 width = (png_uint_32)(Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    png_uint_32 height = (png_uint_32)(Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
    png_uint_32 x_offset = (png_uint_32)(Data[8] << 24) | (Data[9] << 16) | (Data[10] << 8) | Data[11];
    png_uint_32 y_offset = (png_uint_32)(Data[12] << 24) | (Data[13] << 16) | (Data[14] << 8) | Data[15];
    png_uint_16 delay_num = (png_uint_16)(Data[16] << 8) | Data[17];
    png_uint_16 delay_den = (png_uint_16)(Data[18] << 8) | Data[19];
    png_byte dispose_op = Data[20] % 4; // Valid values: 0-3
    png_byte blend_op = Data[21] % 2; // Valid values: 0-1
    png_byte is_hidden = Data[22] % 2; // Valid values: 0-1
    
    // Limit dimensions to prevent excessive memory allocation
    width = width % 1024 + 1;
    height = height % 1024 + 1;
    x_offset = x_offset % 1024;
    y_offset = y_offset % 1024;
    
    // Test png_get_first_frame_is_hidden (reading context)
    png_get_first_frame_is_hidden(png_ptr_read, info_ptr_read);
    
    // Test png_set_first_frame_is_hidden (writing context)
    png_set_first_frame_is_hidden(png_ptr_write, info_ptr_write, is_hidden);
    
    // Test png_set_next_frame_fcTL
    png_set_next_frame_fcTL(png_ptr_write, info_ptr_write, 
                           width, height, x_offset, y_offset,
                           delay_num, delay_den, dispose_op, blend_op);
    
    // Test png_get_next_frame_blend_op
    png_get_next_frame_blend_op(png_ptr_read, info_ptr_read);
    png_get_next_frame_blend_op(png_ptr_write, info_ptr_write);
    
    // Test png_get_next_frame_dispose_op
    png_get_next_frame_dispose_op(png_ptr_read, info_ptr_read);
    png_get_next_frame_dispose_op(png_ptr_write, info_ptr_write);
    
    // Prepare row pointers for png_write_frame_head
    png_byte **row_pointers = nullptr;
    if (width > 0 && height > 0) {
        // Allocate row pointers
        row_pointers = (png_byte **)png_malloc(png_ptr_write, height * sizeof(png_byte *));
        if (row_pointers) {
            size_t rowbytes = width * 4; // Assume RGBA for simplicity
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = (png_byte *)png_malloc(png_ptr_write, rowbytes);
                if (row_pointers[y] && Size > 23 + y * 4) {
                    // Fill with some data from the input
                    size_t copy_size = (rowbytes < Size - (23 + y * 4)) ? rowbytes : Size - (23 + y * 4);
                    memcpy(row_pointers[y], Data + 23 + y * 4, copy_size);
                }
            }
            
            // Test png_write_frame_head
            png_write_frame_head(png_ptr_write, info_ptr_write, row_pointers,
                                width, height, x_offset, y_offset,
                                delay_num, delay_den, dispose_op, blend_op);
            
            // Free row pointers
            for (png_uint_32 y = 0; y < height; y++) {
                if (row_pointers[y]) {
                    png_free(png_ptr_write, row_pointers[y]);
                }
            }
            png_free(png_ptr_write, row_pointers);
        }
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    return 0;
}