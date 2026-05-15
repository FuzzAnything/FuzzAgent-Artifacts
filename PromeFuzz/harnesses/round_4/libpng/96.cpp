// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
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

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - prevent libpng from exiting on error
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return nullptr;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return nullptr;
    }
    
    return png_ptr;
}

static void destroy_png_struct(png_structp png_ptr) {
    if (png_ptr) {
        png_infop info_ptr = nullptr;
        png_destroy_write_struct(&png_ptr, &info_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Create PNG structure
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        destroy_png_struct(png_ptr);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Extract size parameter (if available)
    size_t buffer_size = 1024; // default
    if (data_offset + sizeof(size_t) <= Size) {
        memcpy(&buffer_size, Data + data_offset, sizeof(size_t));
        data_offset += sizeof(size_t);
    }
    
    // Extract integer parameters (if available)
    int mem_level = 8; // default
    int method = 8; // default
    int level = 6; // default
    
    if (data_offset + sizeof(int) <= Size) {
        memcpy(&mem_level, Data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    if (data_offset + sizeof(int) <= Size) {
        memcpy(&method, Data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    if (data_offset + sizeof(int) <= Size) {
        memcpy(&level, Data + data_offset, sizeof(int));
        data_offset += sizeof(int);
    }
    
    // Call target functions based on selector
    // Use different combinations to explore more states
    
    // Always call png_set_compression_buffer_size first (as required)
    png_set_compression_buffer_size(png_ptr, buffer_size);
    
    // Call png_get_compression_buffer_size
    size_t current_size = png_get_compression_buffer_size(png_ptr);
    (void)current_size; // Use result to avoid unused variable warning
    
    // Call other functions based on selector bits
    if (selector & 0x01) {
        png_set_compression_mem_level(png_ptr, mem_level);
    }
    
    if (selector & 0x02) {
        png_set_text_compression_mem_level(png_ptr, mem_level);
    }
    
    if (selector & 0x04) {
        png_set_compression_method(png_ptr, method);
    }
    
    if (selector & 0x08) {
        png_set_compression_level(png_ptr, level);
    }
    
    // Try calling png_set_compression_buffer_size again with different size
    if (selector & 0x10 && data_offset < Size) {
        size_t new_size = static_cast<size_t>(Data[data_offset]) + 1;
        png_set_compression_buffer_size(png_ptr, new_size);
    }
    
    // Call png_get_compression_buffer_size again to check if it changed
    size_t final_size = png_get_compression_buffer_size(png_ptr);
    (void)final_size; // Use result to avoid unused variable warning
    
    destroy_png_struct(png_ptr);
    return 0;
}