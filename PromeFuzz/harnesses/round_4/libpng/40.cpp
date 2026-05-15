// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_option at png.c:3740:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
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
#include <cstring>
#include <cstdlib>
#include <string>

#include "png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing to avoid crashes on errors
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    png_infop info_ptr = png_create_info_struct(png_ptr);
    return info_ptr;
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create PNG structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Test png_set_option
    if (offset + 2 <= Size) {
        int option = Data[offset] % 10;  // Limited range for valid options
        int onoff = Data[offset + 1] % 2;
        png_set_option(png_ptr, option, onoff);
        offset += 2;
    }
    
    // Test png_set_sCAL_s with string parameters
    if (offset + 3 <= Size) {
        int unit = (Data[offset] % 2) + 1;  // 1 or 2 as required
        offset++;
        
        // Create dummy strings from remaining data
        size_t str_len = Size - offset;
        if (str_len > 100) str_len = 100;
        
        char* swidth = static_cast<char*>(malloc(str_len + 1));
        char* sheight = static_cast<char*>(malloc(str_len + 1));
        
        if (swidth && sheight) {
            memcpy(swidth, Data + offset, str_len);
            memcpy(sheight, Data + offset, str_len);
            swidth[str_len] = '\0';
            sheight[str_len] = '\0';
            
            png_set_sCAL_s(png_ptr, info_ptr, unit, swidth, sheight);
            
            free(swidth);
            free(sheight);
        }
    }
    
    // Test png_set_sCAL_fixed
    if (offset + 9 <= Size) {
        int unit = Data[offset] % 3;
        offset++;
        
        png_fixed_point width, height;
        memcpy(&width, Data + offset, 4);
        memcpy(&height, Data + offset + 4, 4);
        offset += 8;
        
        png_set_sCAL_fixed(png_ptr, info_ptr, unit, width, height);
    }
    
    // Test png_set_sCAL (floating point version)
    if (offset + 17 <= Size) {
        int unit = Data[offset] % 3;
        offset++;
        
        double width, height;
        memcpy(&width, Data + offset, 8);
        memcpy(&height, Data + offset + 8, 8);
        offset += 16;
        
        png_set_sCAL(png_ptr, info_ptr, unit, width, height);
    }
    
    // Test png_get_sCAL_s
    {
        int unit;
        char* swidth = nullptr;
        char* sheight = nullptr;
        
        png_uint_32 result = png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
        // Result is ignored as we're just testing the function call
    }
    
    // Test png_get_sCAL (floating point version)
    {
        int unit;
        double width, height;
        
        png_uint_32 result = png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);
        // Result is ignored
    }
    
    // Cleanup
    cleanup_png(png_ptr, info_ptr);
    
    return 0;
}