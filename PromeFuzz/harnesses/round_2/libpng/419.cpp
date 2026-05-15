// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_pHYs at pngset.c:723:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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
// png_set_user_limits at pngset.c:1937:1 in png.h
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
#include <cstring>
#include <setjmp.h>
#include "code/png.h"

static void user_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to avoid crashes on invalid input
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void user_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_419(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for parameter extraction
    if (Size < 50) return 0;
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // Extract png_uint_32 parameters (4 bytes each)
    png_uint_32 res_x = 0, res_y = 0;
    png_uint_32 user_width_max = 0, user_height_max = 0;
    png_uint_32 width = 0, height = 0;
    
    if (offset + 4 <= Size) {
        memcpy(&res_x, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&res_y, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&user_width_max, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&user_height_max, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&width, Data + offset, 4);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        memcpy(&height, Data + offset, 4);
        offset += 4;
    }
    
    // Extract int parameters (1 byte each, but we'll use full int)
    int unit_type = 0;
    int bit_depth = 0;
    int color_type = 0;
    int interlace_method = 0;
    int compression_method = 0;
    int filter_method = 0;
    int transforms = 0;
    
    if (offset + 1 <= Size) unit_type = Data[offset++] & 0x03; // 0-2 valid
    if (offset + 1 <= Size) bit_depth = Data[offset++] & 0x1F; // Mask to reasonable range
    if (offset + 1 <= Size) color_type = Data[offset++] & 0x07; // 0-6 valid
    if (offset + 1 <= Size) interlace_method = Data[offset++] & 0x01; // 0-1 valid
    if (offset + 1 <= Size) compression_method = Data[offset++] & 0x01; // 0 valid
    if (offset + 1 <= Size) filter_method = Data[offset++] & 0x01; // 0 valid
    if (offset + 1 <= Size) transforms = Data[offset++] & 0xFF;
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, user_error_fn, user_warning_fn);
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
    
    // Test png_set_user_limits
    png_set_user_limits(png_ptr, user_width_max, user_height_max);
    
    // Test png_get_user_width_max and png_get_user_height_max
    png_uint_32 retrieved_width_max = png_get_user_width_max(png_ptr);
    png_uint_32 retrieved_height_max = png_get_user_height_max(png_ptr);
    
    // Test png_set_IHDR with various parameters
    // Constrain parameters to valid ranges
    width = width % 10000;  // Reasonable max
    height = height % 10000;
    bit_depth = (bit_depth % 5) + 1;  // 1-5, then map to valid values
    if (bit_depth == 3) bit_depth = 2;
    if (bit_depth == 5) bit_depth = 4;
    if (bit_depth > 4) bit_depth = 8;
    
    color_type = color_type % 7;  // 0-6
    if (color_type == 1 || color_type == 5) color_type = 0;  // Remove invalid types
    
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Test png_set_pHYs
    png_set_pHYs(png_ptr, info_ptr, res_x, res_y, unit_type);
    
    // Write to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Test png_write_png with various transforms
    // Create dummy image data if dimensions are reasonable
    if (width > 0 && height > 0 && width < 1000 && height < 1000) {
        png_bytepp row_pointers = nullptr;
        int channels = 0;
        
        // Determine channels based on color_type
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
        
        // Calculate row bytes with safety margin
        png_size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
        
        // Calculate maximum possible rowbytes for worst-case transforms
        // Maximum: width * 4 channels * 2 bytes per channel (for 16-bit) = width * 8
        // Add extra margin for safety
        png_size_t max_row_bytes = (width * 8) + 16;
        
        if (row_bytes > 0 && row_bytes < 10000) {
            // Use the maximum of actual rowbytes and calculated maximum
            png_size_t allocated_row_bytes = (row_bytes > max_row_bytes) ? row_bytes : max_row_bytes;
            allocated_row_bytes += 4; // Additional safety margin
            
            // Ensure allocated_row_bytes is reasonable
            if (allocated_row_bytes > 10000) {
                allocated_row_bytes = 10000;
            }
            
            row_pointers = (png_bytepp)png_malloc(png_ptr, height * sizeof(png_bytep));
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = (png_bytep)png_malloc(png_ptr, allocated_row_bytes);
                // Fill with some data from fuzzer input
                for (png_size_t x = 0; x < row_bytes; x++) {
                    if (offset + 1 <= Size) {
                        row_pointers[y][x] = Data[offset++];
                    } else {
                        row_pointers[y][x] = (x + y) & 0xFF;
                    }
                }
                // Initialize the rest of the buffer to 0
                for (png_size_t x = row_bytes; x < allocated_row_bytes; x++) {
                    row_pointers[y][x] = 0;
                }
            }
            png_set_rows(png_ptr, info_ptr, row_pointers);
        }
        
        // Write the PNG
        png_write_png(png_ptr, info_ptr, transforms, nullptr);
        
        // Clean up row pointers if allocated
        if (row_pointers) {
            for (png_uint_32 y = 0; y < height; y++) {
                png_free(png_ptr, row_pointers[y]);
            }
            png_free(png_ptr, row_pointers);
        }
    }
    
    fclose(fp);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}