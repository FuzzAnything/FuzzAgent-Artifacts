// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_text at pngset.c:912:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
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

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_data(png_structp png_ptr) {
    // Dummy flush function that does nothing
    (void)png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_323(const uint8_t *Data, size_t Size) {
    // Initialize png_struct for writing
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    // Initialize png_info
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
    
    // Set up dummy I/O
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);
    
    // Use fuzzer data to generate parameters for API calls
    // We'll extract values from the input data, using modulo to keep them in valid ranges
    
    // For png_set_cICP - parameters are png_byte (0-255)
    if (Size >= 4) {
        png_byte colour_primaries = Data[0] % 256;
        png_byte transfer_function = Data[1] % 256;
        png_byte matrix_coefficients = Data[2] % 256;
        png_byte video_full_range_flag = Data[3] % 2; // 0 or 1
        
        png_set_cICP(png_ptr, info_ptr, colour_primaries, transfer_function,
                     matrix_coefficients, video_full_range_flag);
    }
    
    // For png_set_cLLI - parameters are doubles
    if (Size >= 16) {
        double maxCLL, maxFALL;
        // Extract doubles from bytes (simplified - just use integer part)
        uint32_t val1, val2;
        memcpy(&val1, Data + 4, sizeof(uint32_t));
        memcpy(&val2, Data + 8, sizeof(uint32_t));
        maxCLL = static_cast<double>(val1 % 1000000); // Reasonable range
        maxFALL = static_cast<double>(val2 % 1000000);
        
        png_set_cLLI(png_ptr, info_ptr, maxCLL, maxFALL);
    }
    
    // For png_set_mDCV - parameters are doubles
    if (Size >= 80) {
        double params[10];
        for (int i = 0; i < 10 && i * 8 + 16 < Size; i++) {
            uint32_t val;
            memcpy(&val, Data + 16 + i * 8, sizeof(uint32_t));
            params[i] = static_cast<double>(val % 10000) / 1000.0; // Reasonable range
        }
        
        png_set_mDCV(png_ptr, info_ptr,
                     params[0], params[1],   // white_x, white_y
                     params[2], params[3],   // red_x, red_y
                     params[4], params[5],   // green_x, green_y
                     params[6], params[7],   // blue_x, blue_y
                     params[8], params[9]);  // max_lum, min_lum
    }
    
    // Set up some text data for png_free_data to work with
    if (Size >= 100) {
        png_text text;
        text.compression = PNG_TEXT_COMPRESSION_NONE;
        text.key = (char*)"Test";
        text.text = (char*)"Test text";
        text.text_length = 9;
        text.itxt_length = 0;
        text.lang = nullptr;
        text.lang_key = nullptr;
        
        png_set_text(png_ptr, info_ptr, &text, 1);
    }
    
    // Call png_free_data with various masks
    if (Size >= 104) {
        png_uint_32 free_me;
        int num;
        memcpy(&free_me, Data + 80, sizeof(png_uint_32));
        memcpy(&num, Data + 84, sizeof(int));
        
        // Use only the free mask values that are defined in the libpng version
        free_me &= (PNG_FREE_PLTE | PNG_FREE_TRNS | PNG_FREE_HIST |
                   PNG_FREE_ICCP | PNG_FREE_SPLT | PNG_FREE_ROWS |
                   PNG_FREE_SCAL | PNG_FREE_UNKN | PNG_FREE_PCAL);
        
        png_free_data(png_ptr, info_ptr, free_me, num);
    }
    
    // Test png_free_default with allocated memory
    if (Size >= 108) {
        size_t alloc_size = Data[88] % 1024 + 1; // 1-1024 bytes
        void* ptr = png_malloc(png_ptr, alloc_size);
        if (ptr) {
            png_free_default(png_ptr, ptr);
        }
    }
    
    // Test png_destroy_info_struct
    // We need to create a new info struct since the current one might be partially destroyed
    png_infop info_ptr2 = png_create_info_struct(png_ptr);
    if (info_ptr2) {
        png_infopp info_ptr_ptr = &info_ptr2;
        png_destroy_info_struct(png_ptr, info_ptr_ptr);
        // info_ptr2 should now be NULL
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}