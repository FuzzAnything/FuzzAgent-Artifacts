// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sCAL_fixed at pngset.c:694:1 in png.h
// png_set_sCAL_s at pngset.c:598:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_set_sCAL at pngset.c:664:1 in png.h
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
#include <cstring>
#include <cstdlib>
#include <setjmp.h>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Don't call longjmp - just return
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Don't call longjmp - just return
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling to avoid longjmp crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        // If we get here, libpng encountered an error
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up dummy file for reading
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Read PNG header - this may fail for non-PNG data
    png_init_io(png_ptr, fp);
    
    // Try to read PNG info, but don't crash if it fails
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_sCAL_s
    {
        int unit = 0;
        char *swidth = nullptr;
        char *sheight = nullptr;
        png_uint_32 ret = png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
        if (ret == PNG_INFO_sCAL) {
            // Valid data returned
        }
    }
    
    // Test png_get_sCAL_fixed
    {
        int unit = 0;
        png_fixed_point width = 0;
        png_fixed_point height = 0;
        png_uint_32 ret = png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);
        if (ret == PNG_INFO_sCAL) {
            // Valid data returned
        }
    }
    
    // Test png_set_sCAL functions
    {
        // Use first few bytes for parameters
        int unit = (Size > 0) ? (Data[0] % 2) : 0;
        
        // Test png_set_sCAL with floating point
        if (Size >= 17) {
            double width, height;
            memcpy(&width, Data + 1, 8);
            memcpy(&height, Data + 9, 8);
            png_set_sCAL(png_ptr, info_ptr, unit, width, height);
        }
        
        // Test png_set_sCAL_fixed
        if (Size >= 9) {
            png_fixed_point width, height;
            memcpy(&width, Data + 1, 4);
            memcpy(&height, Data + 5, 4);
            png_set_sCAL_fixed(png_ptr, info_ptr, unit, width, height);
        }
        
        // Test png_set_sCAL_s with string parameters
        if (Size >= 3) {
            // Create null-terminated strings from data
            char swidth_buf[256] = {0};
            char sheight_buf[256] = {0};
            size_t swidth_len = (Size - 1 < 255) ? Size - 1 : 255;
            size_t sheight_len = (Size - 1 - swidth_len < 255) ? Size - 1 - swidth_len : 255;
            
            if (swidth_len > 0) memcpy(swidth_buf, Data + 1, swidth_len);
            if (sheight_len > 0) memcpy(sheight_buf, Data + 1 + swidth_len, sheight_len);
            
            swidth_buf[swidth_len] = '\0';
            sheight_buf[sheight_len] = '\0';
            
            png_set_sCAL_s(png_ptr, info_ptr, unit, swidth_buf, sheight_buf);
        }
    }
    
    // Test png_get_sCAL (floating point version)
    {
        int unit = 0;
        double width = 0.0;
        double height = 0.0;
        png_uint_32 ret = png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);
        if (ret == PNG_INFO_sCAL) {
            // Valid data returned
        }
    }
    
    // Test png_set_invalid
    {
        // Try different mask values based on input data
        int mask = 0;
        if (Size > 0) {
            mask = Data[0] % 32; // Use first byte for mask value
        }
        png_set_invalid(png_ptr, info_ptr, mask);
    }
    
    // Clean up
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}