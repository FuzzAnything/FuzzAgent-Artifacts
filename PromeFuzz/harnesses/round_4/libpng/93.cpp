// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_info at pngread.c:102:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_get_next_frame_delay_den at pngget.c:1479:1 in png.h
// png_get_next_frame_delay_num at pngget.c:1469:1 in png.h
// png_get_next_frame_height at pngget.c:1439:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include "png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csetjmp>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from the fuzzer input
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    uint8_t** input_ptr = static_cast<uint8_t**>(io_ptr);
    if (*input_ptr && length > 0) {
        size_t to_copy = length;
        if (to_copy > 0) {
            memcpy(data, *input_ptr, to_copy);
            *input_ptr += to_copy;
        }
    }
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Write to dummy buffer (ignore)
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    // Create read structures
    png_structp png_ptr_read = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                      nullptr, nullptr, nullptr);
    if (!png_ptr_read) return 0;
    
    png_infop info_ptr_read = png_create_info_struct(png_ptr_read);
    if (!info_ptr_read) {
        png_destroy_read_struct(&png_ptr_read, nullptr, nullptr);
        return 0;
    }
    
    // Create write structures
    png_structp png_ptr_write = png_create_write_struct(PNG_LIBPNG_VER_STRING,
                                                        nullptr, nullptr, nullptr);
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
    
    // Set error handlers
    png_set_error_fn(png_ptr_read, nullptr, dummy_error_fn, dummy_warning_fn);
    png_set_error_fn(png_ptr_write, nullptr, dummy_error_fn, dummy_warning_fn);
    
    // Set up longjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr_read))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr_write))) {
        png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
        png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
        return 0;
    }
    
    // Set up custom read function
    const uint8_t* read_ptr = Data;
    png_set_read_fn(png_ptr_read, const_cast<uint8_t**>(&read_ptr), dummy_read_fn);
    
    // Set up custom write function
    png_set_write_fn(png_ptr_write, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Try to read PNG info
    png_read_info(png_ptr_read, info_ptr_read);
    
    // Test png_read_frame_head
    png_read_frame_head(png_ptr_read, info_ptr_read);
    
    // Test png_get_next_frame_fcTL
    png_uint_32 width, height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    
    png_get_next_frame_fcTL(png_ptr_read, info_ptr_read,
                           &width, &height, &x_offset, &y_offset,
                           &delay_num, &delay_den, &dispose_op, &blend_op);
    
    // Test png_get_next_frame_delay_den
    png_get_next_frame_delay_den(png_ptr_read, info_ptr_read);
    
    // Test png_get_next_frame_delay_num
    png_get_next_frame_delay_num(png_ptr_read, info_ptr_read);
    
    // Test png_get_next_frame_height
    png_get_next_frame_height(png_ptr_read, info_ptr_read);
    
    // Test png_set_next_frame_fcTL with values from fuzzer input
    if (Size >= 22) {
        // Extract values from fuzzer input
        width = *reinterpret_cast<const png_uint_32*>(Data);
        height = *reinterpret_cast<const png_uint_32*>(Data + 4);
        x_offset = *reinterpret_cast<const png_uint_32*>(Data + 8);
        y_offset = *reinterpret_cast<const png_uint_32*>(Data + 12);
        delay_num = *reinterpret_cast<const png_uint_16*>(Data + 16);
        delay_den = *reinterpret_cast<const png_uint_16*>(Data + 18);
        dispose_op = Data[20];
        blend_op = Data[21];
        
        png_set_next_frame_fcTL(png_ptr_write, info_ptr_write,
                               width, height, x_offset, y_offset,
                               delay_num, delay_den, dispose_op, blend_op);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr_read, &info_ptr_read, nullptr);
    png_destroy_write_struct(&png_ptr_write, &info_ptr_write);
    
    return 0;
}