// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_get_acTL at pngget.c:1361:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
#include <setjmp.h>

#include "png.h"

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy write function
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing - dummy flush function
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy read function
}

static void error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error messages during fuzzing
}

static void warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning messages during fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    if (Size < 50) return 0; // Need minimum data for meaningful fuzzing

    // Initialize png_struct for writing
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_fn, warning_fn);
    if (!write_ptr) return 0;
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Initialize png_struct for reading
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_fn, warning_fn);
    if (!read_ptr) {
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Set up dummy I/O functions
    png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    png_set_read_fn(read_ptr, nullptr, dummy_read_fn);
    
    // Set up error handling with setjmp to catch validation errors
    if (setjmp(png_jmpbuf(write_ptr))) {
        // Error occurred in libpng, clean up and return
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(read_ptr))) {
        // Error occurred in libpng, clean up and return
        png_destroy_write_struct(&write_ptr, &write_info);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // Use first bytes for various parameters
    png_uint_32 width = 0, height = 0, x_offset = 0, y_offset = 0;
    png_uint_32 num_frames = 0, num_plays = 0;
    png_uint_16 delay_num = 0, delay_den = 0;
    png_byte dispose_op = 0, blend_op = 0;
    png_byte is_hidden = 0;
    
    if (Size >= 4) { width = *reinterpret_cast<const png_uint_32*>(Data + offset); offset += 4; }
    if (Size >= offset + 4) { height = *reinterpret_cast<const png_uint_32*>(Data + offset); offset += 4; }
    if (Size >= offset + 4) { x_offset = *reinterpret_cast<const png_uint_32*>(Data + offset); offset += 4; }
    if (Size >= offset + 4) { y_offset = *reinterpret_cast<const png_uint_32*>(Data + offset); offset += 4; }
    if (Size >= offset + 4) { num_frames = *reinterpret_cast<const png_uint_32*>(Data + offset); offset += 4; }
    if (Size >= offset + 4) { num_plays = *reinterpret_cast<const png_uint_32*>(Data + offset); offset += 4; }
    if (Size >= offset + 2) { delay_num = *reinterpret_cast<const png_uint_16*>(Data + offset); offset += 2; }
    if (Size >= offset + 2) { delay_den = *reinterpret_cast<const png_uint_16*>(Data + offset); offset += 2; }
    if (Size >= offset + 1) { dispose_op = Data[offset++]; }
    if (Size >= offset + 1) { blend_op = Data[offset++]; }
    if (Size >= offset + 1) { is_hidden = Data[offset++]; }
    
    // Set IHDR first to provide valid image dimensions for validation
    png_set_IHDR(write_ptr, write_info, 1000, 1000, 8, 
                 PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, 
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    // Test png_set_acTL
    png_set_acTL(write_ptr, write_info, num_frames, num_plays);
    
    // Test png_set_first_frame_is_hidden
    png_set_first_frame_is_hidden(write_ptr, write_info, is_hidden);
    
    // Test png_set_next_frame_fcTL - wrap in try block since it validates parameters
    // Note: width and height must be > 0, and offsets must be within image bounds
    // We'll clamp values to avoid validation errors
    png_uint_32 clamped_width = (width == 0) ? 1 : (width > 1000 ? 1000 : width);
    png_uint_32 clamped_height = (height == 0) ? 1 : (height > 1000 ? 1000 : height);
    png_uint_32 clamped_x_offset = (x_offset > 1000 - clamped_width) ? (1000 - clamped_width) : x_offset;
    png_uint_32 clamped_y_offset = (y_offset > 1000 - clamped_height) ? (1000 - clamped_height) : y_offset;
    
    png_set_next_frame_fcTL(write_ptr, write_info, 
                           clamped_width, clamped_height, 
                           clamped_x_offset, clamped_y_offset,
                           delay_num, delay_den, 
                           dispose_op, blend_op);
    
    // Test png_write_frame_head
    // Create dummy row pointers
    png_bytep row_pointers[1] = {nullptr};
    if (clamped_height > 0 && clamped_width > 0) {
        png_write_frame_head(write_ptr, write_info, row_pointers, 
                            clamped_width, clamped_height,
                            clamped_x_offset, clamped_y_offset, 
                            delay_num, delay_den, dispose_op, blend_op);
    }
    
    // Test reading functions
    png_uint_32 out_width = 0, out_height = 0, out_x_offset = 0, out_y_offset = 0;
    png_uint_32 out_num_frames = 0, out_num_plays = 0;
    png_uint_16 out_delay_num = 0, out_delay_den = 0;
    png_byte out_dispose_op = 0, out_blend_op = 0;
    
    // Test png_get_acTL
    png_get_acTL(read_ptr, read_info, &out_num_frames, &out_num_plays);
    
    // Test png_get_next_frame_fcTL
    png_get_next_frame_fcTL(read_ptr, read_info, &out_width, &out_height,
                           &out_x_offset, &out_y_offset, &out_delay_num, &out_delay_den,
                           &out_dispose_op, &out_blend_op);
    
    // Cleanup
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    return 0;
}