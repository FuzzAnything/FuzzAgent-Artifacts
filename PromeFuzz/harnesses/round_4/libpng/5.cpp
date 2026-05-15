// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_text_compression_level at pngwrite.c:1273:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_mem_level at pngwrite.c:1284:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_get_text at pngget.c:1158:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

extern "C" int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    // Create PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }
    
    // Use fuzz data to determine parameters
    if (Size > 0) {
        // Test png_set_text_compression_method with different values
        int method = Data[0] % 10; // Values 0-9
        png_set_text_compression_method(png_ptr, method);
        
        // Test png_set_text_compression_level with different levels
        int level = Data[0] % 10; // Values 0-9
        png_set_text_compression_level(png_ptr, level);
        
        // Test png_set_text_compression_window_bits with different values
        int window_bits = 8 + (Data[0] % 8); // Values 8-15
        png_set_text_compression_window_bits(png_ptr, window_bits);
        
        // Test png_set_text_compression_mem_level with different values
        int mem_level = 1 + (Data[0] % 9); // Values 1-9
        png_set_text_compression_mem_level(png_ptr, mem_level);
    }
    
    // Test png_set_text with fuzz data
    if (Size > 1) {
        // Create text structure
        png_text text_chunk;
        text_chunk.compression = -1; // tEXt, none
        
        // Use fuzz data for key and text
        size_t key_len = Size > 10 ? 10 : Size;
        char* key = static_cast<char*>(malloc(key_len + 1));
        if (key) {
            memcpy(key, Data, key_len);
            key[key_len] = '\0';
            text_chunk.key = key;
            
            // Set text if we have enough data
            char* text = nullptr;
            if (Size > 20) {
                size_t text_len = Size - 20 > 100 ? 100 : Size - 20;
                text = static_cast<char*>(malloc(text_len + 1));
                if (text) {
                    memcpy(text, Data + 10, text_len);
                    text[text_len] = '\0';
                    text_chunk.text = text;
                    text_chunk.text_length = text_len;
                } else {
                    text_chunk.text = "";
                    text_chunk.text_length = 0;
                }
            } else {
                text_chunk.text = "";
                text_chunk.text_length = 0;
            }
            
            // Call png_set_text
            png_set_text(png_ptr, info_ptr, &text_chunk, 1);
            
            // Free allocated memory
            free(key);
            if (text) {
                free(text);
            }
        }
    }
    
    // Test png_get_text (even though we haven't written anything)
    png_textp text_ptr = nullptr;
    int num_text = 0;
    int ret = png_get_text(png_ptr, info_ptr, &text_ptr, &num_text);
    
    // Try different method values
    if (Size > 2) {
        int method2 = Data[1] % 20; // Try wider range
        png_set_text_compression_method(png_ptr, method2);
    }
    
    // Try different window bits values
    if (Size > 3) {
        int window_bits2 = Data[2] % 20; // Some invalid values
        png_set_text_compression_window_bits(png_ptr, window_bits2);
    }
    
    // Try different mem level values
    if (Size > 4) {
        int mem_level2 = Data[3] % 15; // Some invalid values
        png_set_text_compression_mem_level(png_ptr, mem_level2);
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}