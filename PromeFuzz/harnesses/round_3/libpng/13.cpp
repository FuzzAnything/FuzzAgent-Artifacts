// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_strip_16 at pngrtran.c:185:1 in png.h
// png_set_expand_16 at pngrtran.c:985:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void write_data_to_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void dummy_read_function(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function - does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_warning_function(png_structp png_ptr, png_const_charp message) {
    // Dummy warning function - does nothing
    (void)png_ptr;
    (void)message;
}

static void dummy_error_function(png_structp png_ptr, png_const_charp message) {
    // Dummy error function - does nothing
    (void)png_ptr;
    (void)message;
}

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to file for libpng to read
    write_data_to_file(Data, Size);
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                 dummy_error_function, dummy_warning_function);
    if (!png_ptr) return 0;
    
    // Create PNG info structure
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
    png_set_read_fn(png_ptr, nullptr, dummy_read_function);
    
    // Call target API functions based on input data
    // Use first 6 bits to determine which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        if (selector & 0x01) png_set_expand_gray_1_2_4_to_8(png_ptr);
        if (selector & 0x02) png_set_strip_16(png_ptr);
        if (selector & 0x04) png_set_expand_16(png_ptr);
        if (selector & 0x08) png_set_scale_16(png_ptr);
        if (selector & 0x10) png_set_expand(png_ptr);
        if (selector & 0x20) png_set_gray_to_rgb(png_ptr);
    }
    
    // Try to read PNG info to trigger the transformations
    // This will fail for non-PNG data, but that's expected
    png_read_info(png_ptr, info_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}