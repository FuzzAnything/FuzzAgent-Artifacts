// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_y_pixels_per_inch at pngget.c:369:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    FILE* fp = static_cast<FILE*>(png_get_io_ptr(png_ptr));
    if (fp) {
        fread(data, 1, length, fp);
    }
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* Data, size_t Size) {
    static const char* dummy_filename = "./dummy_file";
    
    // Write fuzzer input to dummy file
    FILE* fp = fopen(dummy_filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open dummy file for reading
    fp = fopen(dummy_filename, "rb");
    if (!fp) {
        return 0;
    }
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up custom read function
    png_set_read_fn(png_ptr, fp, dummy_read_fn);
    
    // Call target API functions
    try {
        // Try to read PNG info
        png_read_info(png_ptr, info_ptr);
        
        // Get image dimensions
        png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
        png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
        
        // Get IHDR metadata
        png_uint_32 ihdr_width, ihdr_height;
        int bit_depth, color_type, interlace_method, compression_method, filter_method;
        png_get_IHDR(png_ptr, info_ptr, &ihdr_width, &ihdr_height, &bit_depth,
                    &color_type, &interlace_method, &compression_method, &filter_method);
        
        // Check for valid chunks - using PNG_INFO_IDAT instead of PNG_INFO_ALL
        png_uint_32 valid_flags = png_get_valid(png_ptr, info_ptr, PNG_INFO_IDAT);
        
        // Get vertical pixel density
        png_uint_32 y_pixels_per_inch = png_get_y_pixels_per_inch(png_ptr, info_ptr);
        
        // Try calling functions with NULL pointers to test edge cases
        png_get_image_height(nullptr, nullptr);
        png_get_image_width(nullptr, nullptr);
        png_get_IHDR(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        png_get_valid(nullptr, nullptr, 0);
        png_get_y_pixels_per_inch(nullptr, nullptr);
        
        // Try with mixed valid/invalid parameters
        png_get_image_height(png_ptr, nullptr);
        png_get_image_width(nullptr, info_ptr);
        
    } catch (...) {
        // Catch any exceptions
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}