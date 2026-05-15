// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cLLI at pngset.c:194:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_set_mDCV at pngset.c:297:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_text at pngset.c:912:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
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
#include <cmath>

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output
}

static png_voidp dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_324(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create png_struct
    png_structp png_ptr = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        dummy_error_fn,
        dummy_warning_fn,
        nullptr,
        dummy_malloc_fn,
        dummy_free_fn
    );
    if (!png_ptr) return 0;

    // Create png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use first byte to determine which function to test
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;

    switch (selector) {
        case 0: { // png_free_data
            if (Size - data_offset < sizeof(png_uint_32) + sizeof(int)) break;
            
            png_uint_32 free_me;
            int num;
            
            // Extract parameters from fuzzer data
            memcpy(&free_me, Data + data_offset, sizeof(png_uint_32));
            data_offset += sizeof(png_uint_32);
            memcpy(&num, Data + data_offset, sizeof(int));
            
            // Allocate some dummy text data to free
            if (free_me & PNG_FREE_TEXT) {
                png_text text;
                text.compression = PNG_TEXT_COMPRESSION_NONE;
                text.key = (char*)"Test";
                text.text = (char*)"Test text";
                text.text_length = 9;
                png_set_text(png_ptr, info_ptr, &text, 1);
            }
            
            png_free_data(png_ptr, info_ptr, free_me, num);
            break;
        }
        
        case 1: { // png_set_cICP
            if (Size - data_offset < 4) break;
            
            png_byte colour_primaries = Data[data_offset++];
            png_byte transfer_function = Data[data_offset++];
            png_byte matrix_coefficients = Data[data_offset++];
            png_byte video_full_range_flag = Data[data_offset++];
            
            png_set_cICP(png_ptr, info_ptr, 
                        colour_primaries, transfer_function,
                        matrix_coefficients, video_full_range_flag);
            break;
        }
        
        case 2: { // png_set_cLLI
            if (Size - data_offset < 2 * sizeof(double)) break;
            
            double maxCLL, maxFALL;
            
            // Extract doubles from fuzzer data
            memcpy(&maxCLL, Data + data_offset, sizeof(double));
            data_offset += sizeof(double);
            memcpy(&maxFALL, Data + data_offset, sizeof(double));
            
            // Ensure reasonable values to avoid overflow
            maxCLL = fabs(fmod(maxCLL, 1000000.0));
            maxFALL = fabs(fmod(maxFALL, 1000000.0));
            
            png_set_cLLI(png_ptr, info_ptr, maxCLL, maxFALL);
            break;
        }
        
        case 3: { // png_free_default
            if (Size - data_offset < 4) break;
            
            // Allocate some memory using libpng's default allocator
            size_t alloc_size = Data[data_offset] % 256 + 1;
            void* ptr = png_malloc(png_ptr, alloc_size);
            
            if (ptr) {
                png_free_default(png_ptr, ptr);
            }
            break;
        }
        
        case 4: { // png_destroy_info_struct
            // Create a separate info struct to destroy
            png_infop info_ptr2 = png_create_info_struct(png_ptr);
            if (info_ptr2) {
                png_destroy_info_struct(png_ptr, &info_ptr2);
            }
            break;
        }
        
        case 5: { // png_set_mDCV
            if (Size - data_offset < 10 * sizeof(double)) break;
            
            double params[10];
            for (int i = 0; i < 10; i++) {
                memcpy(&params[i], Data + data_offset + i * sizeof(double), sizeof(double));
                // Ensure values are in reasonable range
                params[i] = fabs(fmod(params[i], 2.0));
            }
            
            png_set_mDCV(png_ptr, info_ptr,
                        params[0], params[1],   // white_x, white_y
                        params[2], params[3],   // red_x, red_y
                        params[4], params[5],   // green_x, green_y
                        params[6], params[7],   // blue_x, blue_y
                        params[8], params[9]);  // max_lum, min_lum
            break;
        }
    }

    // Cleanup
    png_destroy_info_struct(png_ptr, &info_ptr);
    png_destroy_write_struct(&png_ptr, nullptr);
    
    return 0;
}