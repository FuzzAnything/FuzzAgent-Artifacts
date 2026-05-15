// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_acTL at pngset.c:1318:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_write_rows at pngwrite.c:637:1 in png.h
// png_write_frame_tail at pngwrite.c:1529:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_write_end at pngwrite.c:397:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_rows at pngset.c:1845:1 in png.h
// png_write_png at pngwrite.c:1376:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <csetjmp>
#include "png.h"

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_data(png_structp png_ptr) {
    // Dummy flush function
    (void)png_ptr;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data for meaningful fuzzing

    // Create write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    // Create info structure
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

    // Use fuzzer data to set image parameters
    png_uint_32 width = 64;
    png_uint_32 height = 64;
    int bit_depth = 8;
    int color_type = PNG_COLOR_TYPE_RGB;
    
    // Parse some parameters from fuzzer input
    if (Size >= 8) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        bit_depth = Data[4] % 3 + 1; // 1, 2, or 3 (will be scaled to valid values)
        color_type = Data[5] % 7; // 0-6 for different color types
        
        // Scale bit_depth to valid values
        if (bit_depth == 1) bit_depth = 1;
        else if (bit_depth == 2) bit_depth = 2;
        else if (bit_depth == 3) bit_depth = 8;
    }
    
    // Clamp dimensions to reasonable values
    if (width > 1024) width = 1024;
    if (height > 1024) height = 1024;
    if (width == 0) width = 1;
    if (height == 0) height = 1;
    
    // Set valid bit_depth values
    if (bit_depth != 1 && bit_depth != 2 && bit_depth != 4 && 
        bit_depth != 8 && bit_depth != 16) {
        bit_depth = 8;
    }
    
    // Set valid color_type values
    if (color_type != PNG_COLOR_TYPE_GRAY &&
        color_type != PNG_COLOR_TYPE_PALETTE &&
        color_type != PNG_COLOR_TYPE_RGB &&
        color_type != PNG_COLOR_TYPE_RGB_ALPHA &&
        color_type != PNG_COLOR_TYPE_GRAY_ALPHA) {
        color_type = PNG_COLOR_TYPE_RGB;
    }

    // Set image parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, 
                 PNG_FILTER_TYPE_BASE);

    // Test png_set_acTL (for APNG)
    if (Size >= 12) {
        png_uint_32 num_frames = (Data[6] << 8) | Data[7];
        png_uint_32 num_plays = (Data[8] << 8) | Data[9];
        png_set_acTL(png_ptr, info_ptr, num_frames, num_plays);
    }

    // Test png_write_info
    png_write_info(png_ptr, info_ptr);

    // Prepare row data for png_write_rows
    png_bytepp row_pointers = nullptr;
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    if (rowbytes > 0 && height > 0) {
        row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
            // Fill with fuzzer data (wrap around if needed)
            for (size_t x = 0; x < rowbytes; x++) {
                size_t data_idx = (y * rowbytes + x) % Size;
                row_pointers[y][x] = Data[data_idx];
            }
        }
        
        // Test png_write_rows
        png_write_rows(png_ptr, row_pointers, height);
        
        // Test png_write_frame_tail (for APNG)
        png_write_frame_tail(png_ptr, info_ptr);
        
        // Free row data
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }

    // Test png_write_end
    png_write_end(png_ptr, info_ptr);

    // Test png_write_png with different transforms
    // First reset info structure
    png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    
    // Re-set image parameters
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, 
                 PNG_FILTER_TYPE_BASE);
    
    // Prepare row pointers again for png_write_png
    if (rowbytes > 0 && height > 0) {
        row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
        for (png_uint_32 y = 0; y < height; y++) {
            row_pointers[y] = (png_bytep)png_malloc(png_ptr, rowbytes);
            for (size_t x = 0; x < rowbytes; x++) {
                size_t data_idx = (y * rowbytes + x + 50) % Size; // Different offset
                row_pointers[y][x] = Data[data_idx];
            }
        }
        
        png_set_rows(png_ptr, info_ptr, row_pointers);
        
        // Try different transform combinations
        int transforms = 0;
        if (Size >= 13) {
            transforms = Data[10] % 256; // Use fuzzer data to determine transforms
        }
        
        png_write_png(png_ptr, info_ptr, transforms, nullptr);
        
        // Free row data
        for (png_uint_32 y = 0; y < height; y++) {
            png_free(png_ptr, row_pointers[y]);
        }
        png_free(png_ptr, row_pointers);
    }

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}