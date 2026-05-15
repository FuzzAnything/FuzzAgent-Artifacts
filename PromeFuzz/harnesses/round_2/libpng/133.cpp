// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up dummy error handling to avoid longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Test png_set_option with various options
    // Use fuzz data to determine option and onoff values
    if (Size >= 2) {
        int option = Data[0] % 10;  // Map to valid option range
        int onoff = Data[1] % 2;    // 0 or 1
        png_set_option(png_ptr, option, onoff);
    }
    
    // Test png_set_sCAL_s with string parameters
    if (Size >= 10) {
        // Extract unit (1 or 2 as per PNG_SCALE_UNIT_METER/RADIAN)
        int unit = (Data[0] % 2) + 1;
        
        // Create null-terminated strings from fuzz data
        // Use small buffers to avoid overflows
        char swidth[32] = {0};
        char sheight[32] = {0};
        
        size_t width_len = Size/4;
        if (width_len > 31) width_len = 31;
        size_t height_len = Size/4;
        if (height_len > 31) height_len = 31;
        
        memcpy(swidth, Data + 1, width_len);
        swidth[width_len] = '\0';
        
        memcpy(sheight, Data + 1 + width_len, height_len);
        sheight[height_len] = '\0';
        
        png_set_sCAL_s(png_ptr, info_ptr, unit, swidth, sheight);
    }
    
    // Test png_set_sCAL_fixed with fixed-point values
    if (Size >= 12) {
        int unit = (Data[0] % 2) + 1;
        png_fixed_point width, height;
        
        // Convert bytes to fixed-point values
        memcpy(&width, Data + 4, 4);
        memcpy(&height, Data + 8, 4);
        
        png_set_sCAL_fixed(png_ptr, info_ptr, unit, width, height);
    }
    
    // Test png_set_sCAL with floating-point values (if supported)
    #if defined(PNG_FLOATING_ARITHMETIC_SUPPORTED) || defined(PNG_FLOATING_POINT_SUPPORTED)
    if (Size >= 20) {
        int unit = (Data[0] % 2) + 1;
        double width, height;
        
        // Convert bytes to double values
        memcpy(&width, Data + 4, 8);
        memcpy(&height, Data + 12, 8);
        
        png_set_sCAL(png_ptr, info_ptr, unit, width, height);
    }
    #endif
    
    // Test retrieval functions
    int unit = 0;
    char *swidth = nullptr;
    char *sheight = nullptr;
    
    // Test png_get_sCAL_s
    png_uint_32 result = png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    if (result == PNG_INFO_sCAL) {
        // Valid data was retrieved
        if (swidth && sheight) {
            // Access the strings to ensure they're valid
            volatile char c1 = swidth[0];
            volatile char c2 = sheight[0];
            (void)c1;
            (void)c2;
        }
    }
    
    // Test png_get_sCAL (if supported)
    #if defined(PNG_FLOATING_ARITHMETIC_SUPPORTED) || defined(PNG_FLOATING_POINT_SUPPORTED)
    {
        int get_unit = 0;
        double width = 0.0, height = 0.0;
        png_uint_32 get_result = png_get_sCAL(png_ptr, info_ptr, &get_unit, &width, &height);
        (void)get_result;
        (void)width;
        (void)height;
    }
    #endif
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}