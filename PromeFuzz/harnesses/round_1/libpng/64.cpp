// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_next_frame_y_offset at pngget.c:1459:1 in png.h
// png_get_next_frame_width at pngget.c:1429:1 in png.h
// png_set_first_frame_is_hidden at pngset.c:1435:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_next_frame_fcTL at pngset.c:1357:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_write_frame_head at pngwrite.c:1501:1 in png.h
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
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>

#include "code/png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}
static void dummy_write_fn(png_structp, png_bytep, size_t) {}
static void dummy_flush_fn(png_structp) {}
static void dummy_read_fn(png_structp, png_bytep, size_t) {}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 50) return 0;

    // Create read structures
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!read_ptr) return 0;
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        return 0;
    }

    // Create write structures
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }

    // Set up write functions
    png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    png_set_read_fn(read_ptr, nullptr, dummy_read_fn);

    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    png_uint_32 width = 0;
    if (offset + sizeof(png_uint_32) <= Size) {
        memcpy(&width, Data + offset, sizeof(png_uint_32));
        offset += sizeof(png_uint_32);
    }
    
    png_uint_32 height = 0;
    if (offset + sizeof(png_uint_32) <= Size) {
        memcpy(&height, Data + offset, sizeof(png_uint_32));
        offset += sizeof(png_uint_32);
    }
    
    png_uint_32 x_offset = 0;
    if (offset + sizeof(png_uint_32) <= Size) {
        memcpy(&x_offset, Data + offset, sizeof(png_uint_32));
        offset += sizeof(png_uint_32);
    }
    
    png_uint_32 y_offset = 0;
    if (offset + sizeof(png_uint_32) <= Size) {
        memcpy(&y_offset, Data + offset, sizeof(png_uint_32));
        offset += sizeof(png_uint_32);
    }
    
    png_uint_16 delay_num = 0;
    if (offset + sizeof(png_uint_16) <= Size) {
        memcpy(&delay_num, Data + offset, sizeof(png_uint_16));
        offset += sizeof(png_uint_16);
    }
    
    png_uint_16 delay_den = 0;
    if (offset + sizeof(png_uint_16) <= Size) {
        memcpy(&delay_den, Data + offset, sizeof(png_uint_16));
        offset += sizeof(png_uint_16);
    }
    
    png_byte dispose_op = 0;
    if (offset + sizeof(png_byte) <= Size) {
        dispose_op = Data[offset];
        offset += sizeof(png_byte);
    }
    
    png_byte blend_op = 0;
    if (offset + sizeof(png_byte) <= Size) {
        blend_op = Data[offset];
        offset += sizeof(png_byte);
    }
    
    png_byte is_hidden = 0;
    if (offset + sizeof(png_byte) <= Size) {
        is_hidden = Data[offset];
        offset += sizeof(png_byte);
    }

    // Test API functions
    // 1. png_get_next_frame_y_offset
    png_uint_32 result1 = png_get_next_frame_y_offset(read_ptr, read_info);
    (void)result1;
    
    // 2. png_get_next_frame_width
    png_uint_32 result2 = png_get_next_frame_width(read_ptr, read_info);
    (void)result2;
    
    // 3. png_set_first_frame_is_hidden
    png_uint_32 result3 = png_set_first_frame_is_hidden(write_ptr, write_info, is_hidden);
    (void)result3;
    
    // 4. png_set_next_frame_fcTL - with validation to prevent crashes
    // First set up image dimensions to ensure frame fits within bounds
    png_uint_32 image_width = 1000;  // Reasonable image dimensions
    png_uint_32 image_height = 1000;
    
    // Ensure width and height are reasonable (non-zero and within image bounds)
    png_uint_32 valid_width = (width == 0 || width > 10000) ? 100 : (width % (image_width + 1));
    png_uint_32 valid_height = (height == 0 || height > 10000) ? 100 : (height % (image_height + 1));
    
    // Ensure offsets are within image bounds
    png_uint_32 valid_x_offset = (x_offset > 10000) ? 0 : (x_offset % (image_width - valid_width + 1));
    png_uint_32 valid_y_offset = (y_offset > 10000) ? 0 : (y_offset % (image_height - valid_height + 1));
    
    // Ensure delay denominator is not zero
    png_uint_16 valid_delay_den = (delay_den == 0) ? 1 : delay_den;
    
    // Ensure dispose_op and blend_op are within valid ranges
    png_byte valid_dispose_op = dispose_op % 4; // Valid values: 0-3
    png_byte valid_blend_op = blend_op % 2;     // Valid values: 0-1
    
    // Set image dimensions first
    png_set_IHDR(write_ptr, write_info, image_width, image_height, 
                  8, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, 
                  PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    
    // Now set frame control with validated parameters
    png_uint_32 result4 = png_set_next_frame_fcTL(write_ptr, write_info, 
                                                   valid_width, valid_height, 
                                                   valid_x_offset, valid_y_offset, 
                                                   delay_num, valid_delay_den, 
                                                   valid_dispose_op, valid_blend_op);
    (void)result4;
    
    // 5. png_get_next_frame_fcTL
    png_uint_32 width_out = 0;
    png_uint_32 height_out = 0;
    png_uint_32 x_offset_out = 0;
    png_uint_32 y_offset_out = 0;
    png_uint_16 delay_num_out = 0;
    png_uint_16 delay_den_out = 0;
    png_byte dispose_op_out = 0;
    png_byte blend_op_out = 0;
    
    png_uint_32 result5 = png_get_next_frame_fcTL(read_ptr, read_info, &width_out, &height_out,
                                                   &x_offset_out, &y_offset_out, &delay_num_out,
                                                   &delay_den_out, &dispose_op_out, &blend_op_out);
    (void)result5;
    
    // 6. png_write_frame_head
    // Write PNG info first
    png_write_info(write_ptr, write_info);
    
    // Now write frame head with validated parameters
    // Use the same validated parameters that were used for png_set_next_frame_fcTL
    png_write_frame_head(write_ptr, write_info, nullptr, 
                         valid_width, valid_height, 
                         valid_x_offset, valid_y_offset, 
                         delay_num, valid_delay_den, 
                         valid_dispose_op, valid_blend_op);

    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    return 0;
}