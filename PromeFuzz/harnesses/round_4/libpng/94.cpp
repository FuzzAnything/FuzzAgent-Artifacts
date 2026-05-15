// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler to avoid crashes
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    return png_ptr;
}

static void cleanup_png_struct(png_structp png_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create PNG structure
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        return 0;
    }

    // Test png_get_compression_buffer_size
    size_t buffer_size = png_get_compression_buffer_size(png_ptr);

    // Test png_set_compression_buffer_size with various sizes
    // Use first 4 bytes to determine size (mod to keep in valid range)
    size_t new_buffer_size = 1;
    if (Size >= 4) {
        uint32_t size_val;
        memcpy(&size_val, Data, 4);
        // Keep size in range 1 to 2^31-1
        new_buffer_size = 1 + (size_val % 0x7FFFFFFF);
    } else {
        new_buffer_size = 1 + (Data[0] % 1000);
    }
    png_set_compression_buffer_size(png_ptr, new_buffer_size);

    // Test png_set_compression_mem_level
    // Use next byte for mem_level (1-9)
    int mem_level = 1 + (Data[0] % 9);
    png_set_compression_mem_level(png_ptr, mem_level);

    // Test png_set_text_compression_mem_level
    // Use next byte for text mem_level (1-9)
    int text_mem_level = 1 + ((Data[0] + 1) % 9);
    png_set_text_compression_mem_level(png_ptr, text_mem_level);

    // Test png_set_compression_method
    // Method 8 is valid, but test others too
    int method = Data[0] % 16;
    png_set_compression_method(png_ptr, method);

    // Test png_set_compression_level
    // Compression level typically 0-9
    int level = Data[0] % 10;
    png_set_compression_level(png_ptr, level);

    // Get buffer size again to see if it changed
    buffer_size = png_get_compression_buffer_size(png_ptr);

    // Test with different combinations
    if (Size >= 2) {
        // Alternate buffer size
        size_t alt_buffer_size = 1 + (Data[1] % 1000);
        png_set_compression_buffer_size(png_ptr, alt_buffer_size);
        
        // Alternate compression settings
        if (Size >= 3) {
            int alt_mem_level = 1 + (Data[2] % 9);
            png_set_compression_mem_level(png_ptr, alt_mem_level);
            
            if (Size >= 4) {
                int alt_text_mem_level = 1 + (Data[3] % 9);
                png_set_text_compression_mem_level(png_ptr, alt_text_mem_level);
            }
        }
    }

    // Clean up
    cleanup_png_struct(png_ptr);
    
    return 0;
}