// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_set_add_alpha at pngtrans.c:204:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_filler at pngtrans.c:119:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
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

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function - doesn't actually read anything
    memset(data, 0, length);
}

extern "C" int LLVMFuzzerTestOneInput_215(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up dummy read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    
    // Use input data to determine which functions to call and with what parameters
    // We'll use the first few bytes to seed our decisions
    
    // Extract parameters from input data
    size_t offset = 0;
    
    // Get function selector (0-5 for the 6 functions)
    uint8_t func_selector = 0;
    if (offset < Size) {
        func_selector = Data[offset] % 6;
        offset++;
    }
    
    // Get parameters for the functions
    int error_action = 0;
    double red_coeff = 0.0;
    double green_coeff = 0.0;
    png_fixed_point red_fixed = 0;
    png_fixed_point green_fixed = 0;
    png_uint_32 filler = 0;
    int flags = 0;
    int mode = 0;
    double output_gamma = 0.0;
    png_fixed_point output_gamma_fixed = 0;
    
    // Extract parameters from input if available
    if (offset + sizeof(int) <= Size) {
        memcpy(&error_action, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    if (offset + sizeof(double) <= Size) {
        memcpy(&red_coeff, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    
    if (offset + sizeof(double) <= Size) {
        memcpy(&green_coeff, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&red_fixed, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&green_fixed, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }
    
    if (offset + sizeof(png_uint_32) <= Size) {
        memcpy(&filler, Data + offset, sizeof(png_uint_32));
        offset += sizeof(png_uint_32);
    }
    
    if (offset + sizeof(int) <= Size) {
        memcpy(&flags, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    if (offset + sizeof(int) <= Size) {
        memcpy(&mode, Data + offset, sizeof(int));
        offset += sizeof(int);
    }
    
    if (offset + sizeof(double) <= Size) {
        memcpy(&output_gamma, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&output_gamma_fixed, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }
    
    // Call the selected function
    switch (func_selector) {
        case 0:
            png_set_rgb_to_gray(png_ptr, error_action, red_coeff, green_coeff);
            break;
        case 1:
            png_set_add_alpha(png_ptr, filler, flags);
            break;
        case 2:
            png_set_rgb_to_gray_fixed(png_ptr, error_action, red_fixed, green_fixed);
            break;
        case 3:
            png_set_bgr(png_ptr);
            break;
        case 4:
            png_set_alpha_mode(png_ptr, mode, output_gamma);
            break;
        case 5:
            png_set_filler(png_ptr, filler, flags);
            break;
    }
    
    // Try to get RGB to gray status if applicable
    if (func_selector == 0 || func_selector == 2) {
        png_byte status = png_get_rgb_to_gray_status(png_ptr);
        (void)status; // Avoid unused variable warning
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}