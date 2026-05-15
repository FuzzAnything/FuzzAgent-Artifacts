// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_acTL at pngset.c:1318:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
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
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - discard written data
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing
}

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing on error - let the fuzzer continue
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing on warning
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 20) {
        return 0;
    }

    // Initialize png_struct for reading
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!read_ptr) {
        return 0;
    }

    // Initialize png_info for reading
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        return 0;
    }

    // Initialize png_struct for writing
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }

    // Initialize png_info for writing
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }

    // Set up custom write functions to avoid file I/O
    png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);

    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // Ensure we have enough data for parameters
    if (Size - offset < 40) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }

    png_uint_32 width = *reinterpret_cast<const png_uint_32*>(Data + offset);
    offset += 4;
    png_uint_32 height = *reinterpret_cast<const png_uint_32*>(Data + offset);
    offset += 4;
    png_uint_32 x_offset = *reinterpret_cast<const png_uint_32*>(Data + offset);
    offset += 4;
    png_uint_32 y_offset = *reinterpret_cast<const png_uint_32*>(Data + offset);
    offset += 4;
    png_uint_32 num_frames = *reinterpret_cast<const png_uint_32*>(Data + offset);
    offset += 4;
    png_uint_32 num_plays = *reinterpret_cast<const png_uint_32*>(Data + offset);
    offset += 4;
    png_uint_16 delay_num = *reinterpret_cast<const png_uint_16*>(Data + offset);
    offset += 2;
    png_uint_16 delay_den = *reinterpret_cast<const png_uint_16*>(Data + offset);
    offset += 2;
    png_byte dispose_op = Data[offset++];
    png_byte blend_op = Data[offset++];
    png_byte is_hidden = Data[offset++] & 1;

    // Test png_set_acTL
    png_set_acTL(write_ptr, write_info, num_frames, num_plays);

    // Test png_set_first_frame_is_hidden
    png_set_first_frame_is_hidden(write_ptr, write_info, is_hidden);

    // Test png_set_next_frame_fcTL
    png_set_next_frame_fcTL(write_ptr, write_info, width, height, x_offset, y_offset,
                           delay_num, delay_den, dispose_op, blend_op);

    // Test png_get_acTL
    png_uint_32 retrieved_frames = 0;
    png_uint_32 retrieved_plays = 0;
    png_get_acTL(read_ptr, read_info, &retrieved_frames, &retrieved_plays);

    // Test png_get_next_frame_fcTL
    png_uint_32 ret_width = 0;
    png_uint_32 ret_height = 0;
    png_uint_32 ret_x_offset = 0;
    png_uint_32 ret_y_offset = 0;
    png_uint_16 ret_delay_num = 0;
    png_uint_16 ret_delay_den = 0;
    png_byte ret_dispose_op = 0;
    png_byte ret_blend_op = 0;
    
    png_get_next_frame_fcTL(read_ptr, read_info, &ret_width, &ret_height,
                           &ret_x_offset, &ret_y_offset, &ret_delay_num,
                           &ret_delay_den, &ret_dispose_op, &ret_blend_op);

    // Test png_write_frame_head - this requires row_pointers to be set up
    // We'll allocate dummy row pointers for the test
    if (width > 0 && height > 0 && width < 10000 && height < 10000) {
        png_byte** row_pointers = nullptr;
        png_size_t rowbytes = (width * 4 + 7) / 8; // Estimate rowbytes for 4 channels
        
        if (rowbytes > 0 && height < 1000) {
            row_pointers = static_cast<png_byte**>(png_malloc(write_ptr, height * sizeof(png_byte*)));
            if (row_pointers) {
                for (png_uint_32 y = 0; y < height; y++) {
                    row_pointers[y] = static_cast<png_byte*>(png_malloc(write_ptr, rowbytes));
                    if (row_pointers[y] && (Size - offset) >= rowbytes) {
                        memcpy(row_pointers[y], Data + offset, rowbytes);
                        offset += rowbytes;
                    }
                }
                
                png_write_frame_head(write_ptr, write_info, row_pointers, width, height,
                                    x_offset, y_offset, delay_num, delay_den,
                                    dispose_op, blend_op);
                
                // Clean up row pointers
                for (png_uint_32 y = 0; y < height; y++) {
                    if (row_pointers[y]) {
                        png_free(write_ptr, row_pointers[y]);
                    }
                }
                png_free(write_ptr, row_pointers);
            }
        }
    }

    // Clean up
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);

    return 0;
}