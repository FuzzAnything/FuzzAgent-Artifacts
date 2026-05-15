// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_frame_tail at pngwrite.c:1529:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function - does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_data(png_structp png_ptr) {
    // Dummy flush function - does nothing
    (void)png_ptr;
}

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid input
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just prevent the default behavior of aborting
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_350(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!png_ptr) return 0;
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Set up dummy write functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);
    
    // Initialize basic image info based on fuzzer input
    png_uint_32 width = (Size > 4) ? (png_uint_32)(Data[0] * 256 + Data[1]) % 1000 + 1 : 100;
    png_uint_32 height = (Size > 4) ? (png_uint_32)(Data[2] * 256 + Data[3]) % 1000 + 1 : 100;
    int bit_depth = (Size > 4) ? (Data[4] % 16) + 1 : 8;
    
    // Set color type based on input
    int color_type = (Size > 5) ? (Data[5] % 7) : PNG_COLOR_TYPE_RGB;
    
    // Test png_set_acTL with fuzzer-derived parameters
    if (Size > 8) {
        png_uint_32 num_frames = (png_uint_32)(Data[6] * 256 + Data[7]) % 1000;
        png_uint_32 num_plays = (png_uint_32)(Data[8]) % 100;
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
    }
    
    // Test png_write_info
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    
    png_write_info(png_ptr, info_ptr);
    
    // Calculate rowbytes based on image parameters
    int channels;
    switch (color_type) {
        case PNG_COLOR_TYPE_GRAY:
            channels = 1;
            break;
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            channels = 2;
            break;
        case PNG_COLOR_TYPE_PALETTE:
            channels = 1;
            break;
        case PNG_COLOR_TYPE_RGB:
            channels = 3;
            break;
        case PNG_COLOR_TYPE_RGB_ALPHA:
            channels = 4;
            break;
        default:
            channels = 3;
    }
    
    png_size_t rowbytes = (width * channels * bit_depth + 7) / 8;
    
    // Test png_write_rows with dummy row data
    if (width > 0 && height > 0 && rowbytes > 0) {
        png_bytep row = (png_bytep)malloc(rowbytes);
        if (row) {
            // Initialize row with fuzzer data if available
            size_t row_data_size = (Size > 100) ? 100 : Size;
            for (size_t i = 0; i < rowbytes && i < row_data_size; i++) {
                row[i] = (i < Size) ? Data[i] : 0;
            }
            
            png_bytep rows[1] = {row};
            png_write_rows(png_ptr, rows, 1);
            
            free(row);
        }
    }
    
    // Test png_write_frame_tail (for APNG)
    png_write_frame_tail(png_ptr, info_ptr);
    
    // Test png_write_end
    png_write_end(png_ptr, info_ptr);
    
    // Test png_write_png with various transforms
    if (Size > 10) {
        int transforms = (int)(Data[9] % 256);
        
        // Reinitialize structures for png_write_png test
        png_destroy_write_struct(&png_ptr, &info_ptr);
        
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
        if (!png_ptr) return 0;
        
        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, nullptr);
            return 0;
        }
        
        if (setjmp(png_jmpbuf(png_ptr))) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
            return 0;
        }
        
        png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);
        
        // Set up image info again
        png_uint_32 new_width = 50;
        png_uint_32 new_height = 50;
        int new_bit_depth = 8;
        int new_color_type = PNG_COLOR_TYPE_RGB;
        
        png_set_IHDR(png_ptr, info_ptr, new_width, new_height, new_bit_depth,
                     new_color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
                     PNG_FILTER_TYPE_BASE);
        
        // Create dummy row pointers for png_write_png
        png_bytep row_pointers[50];
        int new_channels = 3;
        png_size_t new_rowbytes = (new_width * new_channels * new_bit_depth + 7) / 8;
        png_bytep image_data = (png_bytep)malloc(new_height * new_rowbytes);
        
        if (image_data) {
            // Initialize with fuzzer data
            for (size_t i = 0; i < new_height * new_rowbytes && i < Size; i++) {
                image_data[i] = Data[i % Size];
            }
            
            for (int i = 0; i < 50; i++) {
                row_pointers[i] = image_data + i * new_rowbytes;
            }
            
            // Set row pointers in info structure
            png_set_rows(png_ptr, info_ptr, row_pointers);
            
            png_write_png(png_ptr, info_ptr, transforms, nullptr);
            
            free(image_data);
        }
    }
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}