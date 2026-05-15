// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_rgb_to_gray at pngrtran.c:1085:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling to avoid longjmp crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Use fuzzer input to determine which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0] % 7; // 7 different operations
        
        switch (selector) {
            case 0:
                png_set_strip_alpha(png_ptr);
                break;
            case 1:
                png_get_rgb_to_gray_status(png_ptr);
                break;
            case 2: {
                int error_action = (Size > 1) ? Data[1] % 2 : 0; // Only 0 or 1 for valid error actions
                double red = (Size > 2) ? (Data[2] / 255.0) : 0.2126;
                double green = (Size > 3) ? (Data[3] / 255.0) : 0.7152;
                // Ensure coefficients are valid
                if (red >= 0.0 && green >= 0.0 && (red + green) <= 1.0) {
                    png_set_rgb_to_gray(png_ptr, error_action, red, green);
                }
                break;
            }
            case 3:
                png_set_gray_to_rgb(png_ptr);
                break;
            case 4: {
                int error_action = (Size > 1) ? Data[1] % 2 : 0; // Only 0 or 1 for valid error actions
                png_fixed_point red = (Size > 2) ? (Data[2] * 1000) : 21260;
                png_fixed_point green = (Size > 3) ? (Data[3] * 1000) : 71520;
                // Ensure coefficients are valid
                if (red >= 0 && green >= 0 && (red + green) <= 100000) {
                    png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);
                }
                break;
            }
            case 5:
                png_set_expand(png_ptr);
                break;
            case 6:
                // Call multiple functions to explore interactions
                png_set_strip_alpha(png_ptr);
                png_set_expand(png_ptr);
                png_set_gray_to_rgb(png_ptr);
                break;
        }
        
        // Always check status after potential conversions
        png_get_rgb_to_gray_status(png_ptr);
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}