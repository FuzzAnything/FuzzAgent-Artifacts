// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_read_frame_head at pngread.c:195:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include "code/png.h"

struct mem_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    mem_buffer* buf = (mem_buffer*)png_get_io_ptr(png_ptr);
    if (buf->offset + length > buf->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(data, buf->data + buf->offset, length);
    buf->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;
    
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    mem_buffer buf = {Data, Size, 0};
    png_set_read_fn(png_ptr, &buf, user_read_data);
    
    int choice = Size % 7;
    
    switch (choice) {
        case 0:
            png_read_info(png_ptr, info_ptr);
            png_set_packing(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            {
                png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                
                if (width > 0 && height > 0 && rowbytes > 0) {
                    png_byte* row = (png_byte*)malloc(rowbytes);
                    if (row) {
                        png_read_row(png_ptr, row, nullptr);
                        free(row);
                    }
                }
            }
            
            png_read_end(png_ptr, info_ptr);
            break;
            
        case 1:
            png_read_info(png_ptr, info_ptr);
            png_read_frame_head(png_ptr, info_ptr);
            break;
            
        case 2:
            png_read_info(png_ptr, info_ptr);
            png_set_packing(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            png_read_update_info(png_ptr, info_ptr);
            break;
            
        case 3:
            png_read_info(png_ptr, info_ptr);
            png_set_packing(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            {
                png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                
                if (width > 0 && height > 0 && rowbytes > 0) {
                    png_byte* row = (png_byte*)malloc(rowbytes);
                    if (row) {
                        for (int i = 0; i < 3 && i < (int)height; i++) {
                            png_read_row(png_ptr, row, nullptr);
                        }
                        free(row);
                    }
                }
            }
            
            png_read_end(png_ptr, info_ptr);
            break;
            
        case 4:
            png_read_info(png_ptr, info_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            {
                png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                
                if (width > 0 && height > 0 && rowbytes > 0) {
                    png_byte* row = (png_byte*)malloc(rowbytes);
                    if (row) {
                        png_read_row(png_ptr, row, nullptr);
                        free(row);
                    }
                }
            }
            break;
            
        case 5:
            png_read_info(png_ptr, info_ptr);
            png_read_end(png_ptr, info_ptr);
            break;
            
        case 6:
            png_read_info(png_ptr, info_ptr);
            png_set_packing(png_ptr);
            png_read_update_info(png_ptr, info_ptr);
            
            {
                png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
                png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
                png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
                
                if (width > 0 && height > 0 && rowbytes > 0) {
                    png_byte* row = (png_byte*)malloc(rowbytes);
                    png_byte* display_row = (png_byte*)malloc(rowbytes);
                    if (row && display_row) {
                        png_read_row(png_ptr, row, display_row);
                        free(row);
                        free(display_row);
                    } else {
                        if (row) free(row);
                        if (display_row) free(display_row);
                    }
                }
            }
            break;
    }
    
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}