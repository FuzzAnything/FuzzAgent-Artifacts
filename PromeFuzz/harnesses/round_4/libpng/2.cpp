// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <setjmp.h>

#include "png.h"

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Custom error handler to prevent crashes on invalid input
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - we're not actually reading PNG data
    // This is needed to satisfy libpng's read function pointer
}

extern "C" int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up a dummy read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_data);
    
    // Call different API functions based on selector
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    switch (selector % 6) {
        case 0:
            png_set_expand(png_ptr);
            break;
        case 1:
            if (Size - data_offset >= 9) {
                int error_action = Data[data_offset] % 3;
                png_fixed_point red, green;
                memcpy(&red, Data + data_offset + 1, 4);
                memcpy(&green, Data + data_offset + 5, 4);
                png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);
            }
            break;
        case 2:
            png_set_palette_to_rgb(png_ptr);
            break;
        case 3:
            png_get_rgb_to_gray_status(png_ptr);
            break;
        case 4:
            png_set_gray_to_rgb(png_ptr);
            break;
        case 5:
            if (Size - data_offset >= 17) {
                int error_action = Data[data_offset] % 3;
                double red, green;
                memcpy(&red, Data + data_offset + 1, 8);
                memcpy(&green, Data + data_offset + 9, 8);
                png_set_rgb_to_gray(png_ptr, error_action, red, green);
            }
            break;
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}