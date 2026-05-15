// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
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
#include <cstdlib>
#include <cstring>

#include "code/png.h"

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    // Initialize PNG structures
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
    
    // Extract integer parameters from fuzzer input
    int strategy = 0;
    int mem_level = 0;
    int level = 0;
    int window_bits = 0;
    
    if (Size >= 4) {
        strategy = Data[0] % 10;          // ZLIB strategies are typically 0-4, but we'll use mod 10 for broader coverage
        mem_level = Data[1] % 10;         // Memory level typically 1-9
        level = Data[2] % 10;             // Compression level typically 0-9
        window_bits = Data[3] % 16;       // Window bits typically 8-15
    }
    
    // Call target API functions with fuzzer-derived parameters
    png_set_compression_strategy(png_ptr, strategy);
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_compression_mem_level(png_ptr, mem_level);
    png_set_compression_level(png_ptr, level);
    png_set_compression_window_bits(png_ptr, window_bits);
    
    // Call deprecated function (ignore return value)
    png_reset_zstream(png_ptr);
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}