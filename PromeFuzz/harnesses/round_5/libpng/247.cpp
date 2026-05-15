// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_frame_head at pngwrite.c:1501:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_next_frame_fcTL at pngget.c:1399:1 in png.h
// png_get_next_frame_blend_op at pngget.c:1499:1 in png.h
// png_get_first_frame_is_hidden at pngget.c:1509:1 in png.h
// png_get_next_frame_dispose_op at pngget.c:1489:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

extern "C" int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size) {
    // Initialize structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Setup error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Write fuzz data to file for reading
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Read PNG file
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    
    // Test reading functions
    png_uint_32 width, height, x_offset, y_offset;
    png_uint_16 delay_num, delay_den;
    png_byte dispose_op, blend_op;
    
    // Test png_get_next_frame_fcTL
    png_get_next_frame_fcTL(png_ptr, info_ptr, &width, &height, &x_offset, &y_offset,
                           &delay_num, &delay_den, &dispose_op, &blend_op);
    
    // Test png_get_next_frame_blend_op
    png_get_next_frame_blend_op(png_ptr, info_ptr);
    
    // Test png_get_first_frame_is_hidden
    png_get_first_frame_is_hidden(png_ptr, info_ptr);
    
    // Test png_get_next_frame_dispose_op
    png_get_next_frame_dispose_op(png_ptr, info_ptr);
    
    fclose(fp);
    
    // Clean up read structures
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    // Create write structures for writing tests
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!write_ptr) return 0;
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Setup error handling for write
    if (setjmp(png_jmpbuf(write_ptr))) {
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Test png_set_first_frame_is_hidden
    png_byte is_hidden = (Size > 0) ? Data[0] : 0;
    png_set_first_frame_is_hidden(write_ptr, write_info, is_hidden);
    
    // Prepare for png_write_frame_head
    // Create dummy row pointers
    png_uint_32 frame_width = 100;
    png_uint_32 frame_height = 100;
    png_byte **row_pointers = (png_byte **)malloc(frame_height * sizeof(png_byte *));
    if (row_pointers) {
        for (png_uint_32 i = 0; i < frame_height; i++) {
            row_pointers[i] = (png_byte *)malloc(frame_width * 4); // RGBA
            if (row_pointers[i]) {
                memset(row_pointers[i], 0, frame_width * 4);
            }
        }
        
        // Extract parameters from fuzz data with bounds checking
        png_uint_32 x_off = (Size > 4) ? *(png_uint_32 *)Data : 0;
        png_uint_32 y_off = (Size > 8) ? *(png_uint_32 *)(Data + 4) : 0;
        png_uint_16 d_num = (Size > 10) ? *(png_uint_16 *)(Data + 8) : 1;
        png_uint_16 d_den = (Size > 12) ? *(png_uint_16 *)(Data + 10) : 10;
        png_byte d_op = (Size > 13) ? Data[12] : 0;
        png_byte b_op = (Size > 14) ? Data[13] : 0;
        
        // Test png_write_frame_head
        png_write_frame_head(write_ptr, write_info, row_pointers, 
                            frame_width, frame_height, 
                            x_off, y_off, 
                            d_num, d_den, 
                            d_op, b_op);
        
        // Clean up row pointers
        for (png_uint_32 i = 0; i < frame_height; i++) {
            if (row_pointers[i]) free(row_pointers[i]);
        }
        free(row_pointers);
    }
    
    // Clean up write structures
    png_destroy_write_struct(&write_ptr, &write_info);
    
    return 0;
}