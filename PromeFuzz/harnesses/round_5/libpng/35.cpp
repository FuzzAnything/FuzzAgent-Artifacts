// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_reset_zstream at png.c:951:1 in png.h
// png_set_compression_window_bits at pngwrite.c:1224:1 in png.h
// png_set_compression_strategy at pngwrite.c:1207:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, 
                                                  dummy_error_fn, 
                                                  dummy_warning_fn);
    if (!png_ptr) return 0;
    
    // Create info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up custom write functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);
    
    // Use fuzz data to generate parameter values
    // We'll use the first few bytes to determine parameter values
    int param1 = (Data[0] % 10);  // strategy/level parameters (0-9)
    int param2 = (Data[Size > 1 ? 1 : 0] % 10);  // mem_level/level parameters
    int param3 = (Data[Size > 2 ? 2 : 0] % 16);  // window_bits (0-15)
    int param4 = (Data[Size > 3 ? 3 : 0] % 10);  // additional parameter
    
    // Call target API functions with different combinations
    // Test png_set_compression_strategy
    png_set_compression_strategy(png_ptr, param1);
    
    // Test png_set_compression_mem_level (valid range 1-9)
    int mem_level = (param2 % 9) + 1;
    png_set_compression_mem_level(png_ptr, mem_level);
    
    // Test png_set_text_compression_strategy
    png_set_text_compression_strategy(png_ptr, param1);
    
    // Test png_set_compression_level (valid range 0-9)
    png_set_compression_level(png_ptr, param4 % 10);
    
    // Test png_reset_zstream (deprecated but we still test it)
    // This function returns int, so we capture the return value
    int reset_result = png_reset_zstream(png_ptr);
    (void)reset_result;  // Prevent unused variable warning
    
    // Test png_set_compression_window_bits (valid range 8-15)
    int window_bits = (param3 % 8) + 8;  // Ensure 8-15 range
    png_set_compression_window_bits(png_ptr, window_bits);
    
    // Try different combinations to explore more states
    if (Size > 4) {
        // Use different parameter values based on fuzz data
        int alt_param = Data[4] % 10;
        png_set_compression_strategy(png_ptr, alt_param);
        png_set_text_compression_strategy(png_ptr, alt_param);
    }
    
    if (Size > 5) {
        // Test with extreme values
        png_set_compression_level(png_ptr, Data[5] % 10);
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}