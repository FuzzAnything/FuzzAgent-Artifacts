// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
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
#include <cstring>
#include <cstdlib>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function - just fill with zeros
    memset(data, 0, length);
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler - do nothing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    // Create dummy PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                 dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up dummy read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    
    // Use fuzzer data to determine which functions to call and with what parameters
    // We'll use the first few bytes to make decisions
    if (Size < 1) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Call various API functions based on fuzzer input
    if (selector & 0x01) {
        png_set_swap_alpha(png_ptr);
    }
    
    if (selector & 0x02) {
        png_set_invert_alpha(png_ptr);
    }
    
    if (selector & 0x04) {
        png_set_strip_16(png_ptr);
    }
    
    if (selector & 0x08) {
        png_set_strip_alpha(png_ptr);
    }
    
    // Try to call png_set_background or png_set_background_fixed if we have enough data
    if ((selector & 0x10) && (Size - data_offset >= sizeof(png_color_16) + 4)) {
        png_color_16 background_color;
        memcpy(&background_color, Data + data_offset, sizeof(png_color_16));
        data_offset += sizeof(png_color_16);
        
        int background_gamma_code = static_cast<int>(Data[data_offset] & 0x03);
        data_offset++;
        
        int need_expand = static_cast<int>(Data[data_offset] & 0x01);
        data_offset++;
        
        // Try floating point version
        if (Size - data_offset >= sizeof(double)) {
            double background_gamma;
            memcpy(&background_gamma, Data + data_offset, sizeof(double));
            data_offset += sizeof(double);
            
            png_set_background(png_ptr, &background_color, background_gamma_code, 
                              need_expand, background_gamma);
        }
    }
    
    // Try png_set_background_fixed if we have enough data
    if ((selector & 0x20) && (Size - data_offset >= sizeof(png_color_16) + 9)) {
        png_color_16 background_color;
        memcpy(&background_color, Data + data_offset, sizeof(png_color_16));
        data_offset += sizeof(png_color_16);
        
        int background_gamma_code = static_cast<int>(Data[data_offset] & 0x03);
        data_offset++;
        
        int need_expand = static_cast<int>(Data[data_offset] & 0x01);
        data_offset++;
        
        if (Size - data_offset >= sizeof(png_fixed_point)) {
            png_fixed_point background_gamma;
            memcpy(&background_gamma, Data + data_offset, sizeof(png_fixed_point));
            data_offset += sizeof(png_fixed_point);
            
            png_set_background_fixed(png_ptr, &background_color, background_gamma_code,
                                    need_expand, background_gamma);
        }
    }
    
    // Try png_set_scale_16 if we have enough data
    if ((selector & 0x40) && Size >= 2) {
        png_set_scale_16(png_ptr);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}