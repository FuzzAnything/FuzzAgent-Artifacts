// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
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

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy writer
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing - dummy flusher
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress errors
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warnings
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for reading operations
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Initialize write structures
    png_structp write_png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!write_png_ptr) {
        return 0;
    }
    
    png_infop write_info_ptr = png_create_info_struct(write_png_ptr);
    if (!write_info_ptr) {
        png_destroy_write_struct(&write_png_ptr, nullptr);
        return 0;
    }
    
    // Set up dummy write functions
    png_set_write_fn(write_png_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Set up minimal PNG info for writing
    png_set_IHDR(write_png_ptr, write_info_ptr, 
                 64, 64, 8, PNG_COLOR_TYPE_RGB, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Set acTL chunk for APNG (required before png_write_frame_head)
    png_set_acTL(write_png_ptr, write_info_ptr, 2, 0);
    
    // Write PNG info
    png_write_info(write_png_ptr, write_info_ptr);
    
    // Extract parameters from fuzzed data
    png_uint_32 width = 64;
    png_uint_32 height = 64;
    png_uint_32 x_offset = 0;
    png_uint_32 y_offset = 0;
    png_uint_16 delay_num = 1;
    png_uint_16 delay_den = 10;
    png_byte dispose_op = 0;  // PNG_DISPOSE_OP_NONE
    png_byte blend_op = 0;    // PNG_BLEND_OP_SOURCE
    
    if (Size >= 16) {
        width = *reinterpret_cast<const png_uint_32*>(Data) % 1000;
        height = *reinterpret_cast<const png_uint_32*>(Data + 4) % 1000;
        x_offset = *reinterpret_cast<const png_uint_32*>(Data + 8) % 1000;
        y_offset = *reinterpret_cast<const png_uint_32*>(Data + 12) % 1000;
    }
    
    if (Size >= 20) {
        delay_num = *reinterpret_cast<const png_uint_16*>(Data + 16) % 1000;
        delay_den = *reinterpret_cast<const png_uint_16*>(Data + 18) % 1000;
    }
    
    if (Size >= 22) {
        dispose_op = Data[20] % 4;  // PNG_DISPOSE_OP values: 0-3
        blend_op = Data[21] % 2;    // PNG_BLEND_OP values: 0-1
    }
    
    // Allocate dummy row pointers
    png_bytep row_pointers[64];
    for (int i = 0; i < 64; i++) {
        row_pointers[i] = static_cast<png_bytep>(png_malloc(write_png_ptr, 64 * 3));
        memset(row_pointers[i], 0, 64 * 3);
    }
    
    // Test png_set_first_frame_is_hidden
    png_byte is_hidden = (Size > 0) ? Data[0] % 2 : 0;
    png_set_first_frame_is_hidden(write_png_ptr, write_info_ptr, is_hidden);
    
    // Test png_write_frame_head (now with acTL set)
    png_write_frame_head(write_png_ptr, write_info_ptr, 
                         reinterpret_cast<png_bytepp>(row_pointers),
                         width, height, x_offset, y_offset,
                         delay_num, delay_den, dispose_op, blend_op);
    
    // Clean up write structures
    for (int i = 0; i < 64; i++) {
        png_free(write_png_ptr, row_pointers[i]);
    }
    png_destroy_write_struct(&write_png_ptr, &write_info_ptr);
    
    // Initialize read structures - only if we have enough data for a PNG header
    if (Size < 8) {
        return 0;
    }
    
    FILE* fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }
    
    png_structp read_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!read_png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop read_info_ptr = png_create_info_struct(read_png_ptr);
    if (!read_info_ptr) {
        png_destroy_read_struct(&read_png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up read from file
    png_init_io(read_png_ptr, fp);
    
    // Set up error handling to prevent longjmp on invalid PNG data
    if (setjmp(png_jmpbuf(read_png_ptr))) {
        // If we get here, libpng encountered an error
        png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Try to read PNG info - this may fail for invalid data
    png_read_info(read_png_ptr, read_info_ptr);
    
    // Test read functions
    png_get_next_frame_dispose_op(read_png_ptr, read_info_ptr);
    png_get_next_frame_blend_op(read_png_ptr, read_info_ptr);
    png_get_first_frame_is_hidden(read_png_ptr, read_info_ptr);
    
    // Test png_get_next_frame_fcTL
    png_uint_32 out_width, out_height, out_x_offset, out_y_offset;
    png_uint_16 out_delay_num, out_delay_den;
    png_byte out_dispose_op, out_blend_op;
    
    png_get_next_frame_fcTL(read_png_ptr, read_info_ptr,
                            &out_width, &out_height,
                            &out_x_offset, &out_y_offset,
                            &out_delay_num, &out_delay_den,
                            &out_dispose_op, &out_blend_op);
    
    // Clean up read structures
    png_destroy_read_struct(&read_png_ptr, &read_info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}