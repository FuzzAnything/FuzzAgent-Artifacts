// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
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

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

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

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 23) return 0;

    // Create PNG structures for reading
    png_structp png_ptr_r = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_r) return 0;
    
    png_infop info_ptr_r = png_create_info_struct(png_ptr_r);
    if (!info_ptr_r) {
        png_destroy_read_struct(&png_ptr_r, nullptr, nullptr);
        return 0;
    }

    // Create PNG structures for writing
    png_structp png_ptr_w = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr_w) {
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        return 0;
    }
    
    png_infop info_ptr_w = png_create_info_struct(png_ptr_w);
    if (!info_ptr_w) {
        png_destroy_write_struct(&png_ptr_w, nullptr);
        png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr_r)) || setjmp(png_jmpbuf(png_ptr_w))) {
        png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
        png_destroy_read_struct(&png_ptr_r, &info_ptr_w, nullptr);
        return 0;
    }

    // Set up dummy I/O functions
    png_set_read_fn(png_ptr_r, nullptr, dummy_read_fn);
    png_set_write_fn(png_ptr_w, nullptr, dummy_write_fn, dummy_flush_fn);

    // Extract parameters from fuzzer input safely
    png_uint_32 width = 100, height = 100, x_offset = 0, y_offset = 0;
    png_uint_16 delay_num = 100, delay_den = 100;
    png_byte dispose_op = 0, blend_op = 0, is_hidden = 0;
    
    // Use memcpy to safely extract values without risking buffer overflow
    if (Size >= 4) memcpy(&width, Data, 4);
    if (Size >= 8) memcpy(&height, Data + 4, 4);
    if (Size >= 12) memcpy(&x_offset, Data + 8, 4);
    if (Size >= 16) memcpy(&y_offset, Data + 12, 4);
    if (Size >= 18) memcpy(&delay_num, Data + 16, 2);
    if (Size >= 20) memcpy(&delay_den, Data + 18, 2);
    if (Size >= 21) dispose_op = Data[20];
    if (Size >= 22) blend_op = Data[21];
    if (Size >= 23) is_hidden = Data[22];

    // Set some reasonable limits to avoid excessive memory allocation
    if (width > 1000) width = 1000;
    if (height > 1000) height = 1000;
    if (x_offset > 1000) x_offset = 1000;
    if (y_offset > 1000) y_offset = 1000;
    if (delay_den == 0) delay_den = 100;
    if (dispose_op > 3) dispose_op = 0;
    if (blend_op > 1) blend_op = 0;
    if (is_hidden > 1) is_hidden = 0;

    // Test png_set_first_frame_is_hidden (writing API)
    png_set_first_frame_is_hidden(png_ptr_w, info_ptr_w, is_hidden);

    // Test png_get_first_frame_is_hidden (reading API)
    // We need to simulate that we've read a PNG header
    png_get_first_frame_is_hidden(png_ptr_r, info_ptr_r);

    // Test png_set_next_frame_fcTL (writing API)
    png_set_next_frame_fcTL(png_ptr_w, info_ptr_w, width, height, 
                           x_offset, y_offset, delay_num, delay_den,
                           dispose_op, blend_op);

    // Test png_get_next_frame_blend_op (reading API)
    png_get_next_frame_blend_op(png_ptr_r, info_ptr_r);

    // Test png_get_next_frame_dispose_op (reading API)
    png_get_next_frame_dispose_op(png_ptr_r, info_ptr_r);

    // Test png_write_frame_head (writing API)
    // First set up minimal PNG info for writing
    png_set_IHDR(png_ptr_w, info_ptr_w, width, height, 8, 
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Set up row pointers for png_write_frame_head
    png_byte** row_pointers = nullptr;
    if (width > 0 && height > 0) {
        row_pointers = static_cast<png_byte**>(png_malloc(png_ptr_w, height * sizeof(png_byte*)));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = static_cast<png_byte*>(png_malloc(png_ptr_w, width * 3));
            // Initialize with some data
            memset(row_pointers[y], y % 256, width * 3);
        }
    }

    // Write PNG info
    png_write_info(png_ptr_w, info_ptr_w);

    // Now test png_write_frame_head
    if (row_pointers) {
        png_write_frame_head(png_ptr_w, info_ptr_w, row_pointers,
                            width, height, x_offset, y_offset,
                            delay_num, delay_den, dispose_op, blend_op);
        
        // Clean up row pointers
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr_w, row_pointers[y]);
        }
        png_free(png_ptr_w, row_pointers);
    }

    // Clean up
    png_destroy_write_struct(&png_ptr_w, &info_ptr_w);
    png_destroy_read_struct(&png_ptr_r, &info_ptr_r, nullptr);

    return 0;
}