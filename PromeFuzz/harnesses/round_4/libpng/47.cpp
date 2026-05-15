// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_write_function(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_function(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    // Initialize libpng structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
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
    
    // Set up dummy write function
    png_set_write_fn(png_ptr, nullptr, dummy_write_function, dummy_flush_function);
    
    // Use fuzzer data to generate parameter values
    // We'll use the first few bytes to determine parameter values
    if (Size < 1) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Extract parameter values from fuzzer input
    // Use modulo to keep values in reasonable ranges
    int mem_level = (Data[0] % 9) + 1;  // 1-9 for zlib memory level
    int strategy = Data[0] % 10;        // 0-9 for compression strategy
    int window_bits = (Data[0] % 8) + 8; // 8-15 for window bits
    int level = Data[0] % 10;           // 0-9 for compression level
    
    // If we have more data, use it for additional parameter variations
    if (Size > 1) {
        strategy = Data[1] % 10;
    }
    if (Size > 2) {
        window_bits = (Data[2] % 8) + 8;
    }
    if (Size > 3) {
        level = Data[3] % 10;
    }
    
    // Call target API functions with fuzzed parameters
    png_set_compression_mem_level(png_ptr, mem_level);
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_compression_strategy(png_ptr, strategy);
    png_set_compression_window_bits(png_ptr, window_bits);
    png_set_compression_level(png_ptr, level);
    
    // Try png_reset_zstream (deprecated but still testable)
    // Note: This function requires a valid zstream, which may not be initialized
    // We'll call it but ignore the return value
    png_reset_zstream(png_ptr);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}