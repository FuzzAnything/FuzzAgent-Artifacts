// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up custom read function
    png_set_read_fn(png_ptr, NULL, dummy_read_fn);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Test target API functions
    png_get_channels(png_ptr, info_ptr);
    png_get_color_type(png_ptr, info_ptr);
    
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);
    
    png_get_interlace_type(png_ptr, info_ptr);
    png_get_filter_type(png_ptr, info_ptr);
    png_get_compression_type(png_ptr, info_ptr);
    
    // Test with NULL parameters
    png_get_channels(NULL, NULL);
    png_get_color_type(NULL, NULL);
    png_get_IHDR(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    png_get_interlace_type(NULL, NULL);
    png_get_filter_type(NULL, NULL);
    png_get_compression_type(NULL, NULL);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}