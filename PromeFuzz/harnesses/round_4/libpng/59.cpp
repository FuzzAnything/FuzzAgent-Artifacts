// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_channels at pngget.c:481:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_interlace_type at pngget.c:107:1 in png.h
// png_get_filter_type at pngget.c:98:1 in png.h
// png_get_compression_type at pngget.c:116:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <csetjmp>

#include "png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    
    // Set up error handling via setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return nullptr;
    }
    
    return png_ptr;
}

static png_infop create_info_struct(png_structp png_ptr) {
    if (!png_ptr) {
        return nullptr;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return nullptr;
    }
    
    return info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) {
        return 0;
    }
    
    fwrite(Data, 1, Size, file);
    fclose(file);
    
    // Open dummy file for reading
    file = fopen("./dummy_file", "rb");
    if (!file) {
        return 0;
    }
    
    // Create PNG read structure
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        fclose(file);
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(file);
        return 0;
    }
    
    // Set up error handling again after creating info struct
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(file);
        return 0;
    }
    
    // Initialize PNG I/O
    png_init_io(png_ptr, file);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_channels
    volatile png_byte channels = png_get_channels(png_ptr, info_ptr);
    
    // Test png_get_color_type
    volatile png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    
    // Test png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type_int, interlace_method, compression_method, filter_method;
    volatile png_uint_32 ihdr_result = png_get_IHDR(png_ptr, info_ptr, &width, &height,
                                                   &bit_depth, &color_type_int,
                                                   &interlace_method, &compression_method,
                                                   &filter_method);
    
    // Test png_get_interlace_type
    volatile png_byte interlace_type = png_get_interlace_type(png_ptr, info_ptr);
    
    // Test png_get_filter_type
    volatile png_byte filter_type = png_get_filter_type(png_ptr, info_ptr);
    
    // Test png_get_compression_type
    volatile png_byte compression_type = png_get_compression_type(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(file);
    
    return 0;
}