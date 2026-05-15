// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_expand_gray_1_2_4_to_8 at pngrtran.c:957:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "png.h"

struct buffer_info {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from the fuzzing input buffer
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    buffer_info* buf = static_cast<buffer_info*>(io_ptr);
    if (buf->offset + length > buf->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(data, buf->data + buf->offset, length);
    buf->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0; // Need at least PNG signature size
    
    // Write data to dummy file for file-based API usage
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) return 0;
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                  nullptr, 
                                                  dummy_error_fn, 
                                                  dummy_warning_fn);
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
    
    // Set up custom read function to read from buffer
    buffer_info buf = {Data, Size, 0};
    png_set_read_fn(png_ptr, &buf, dummy_read_fn);
    
    // Skip PNG signature check for fuzzing
    png_set_sig_bytes(png_ptr, 0);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Apply transformations based on input data to explore different states
    // Use bits from input to decide which transformations to apply
    if (Size > 0) {
        uint8_t transform_flags = Data[0];
        
        // Apply transformations in different combinations
        if (transform_flags & 0x01) {
            png_set_expand(png_ptr);
        }
        
        if (transform_flags & 0x02) {
            // Use some bits from input for coefficients
            png_fixed_point red_coeff = 29900;   // Default ~0.299
            png_fixed_point green_coeff = 58700; // Default ~0.587
            int error_action = 1; // PNG_ERROR_ACTION_NONE
            
            if (Size > 1) {
                // Use input bytes to vary coefficients
                red_coeff = (Data[1] * 100000) / 255;
                if (Size > 2) {
                    green_coeff = (Data[2] * 100000) / 255;
                }
                if (Size > 3) {
                    error_action = Data[3] % 3 + 1; // 1, 2, or 3
                }
            }
            
            png_set_rgb_to_gray_fixed(png_ptr, error_action, red_coeff, green_coeff);
            
            // Check status
            png_get_rgb_to_gray_status(png_ptr);
        }
        
        if (transform_flags & 0x04) {
            png_set_expand_gray_1_2_4_to_8(png_ptr);
        }
        
        if (transform_flags & 0x08) {
            // Use some bits from input for user transform info
            int depth = 8;
            int channels = 4;
            void* user_ptr = nullptr;
            
            if (Size > 4) {
                depth = (Data[4] % 16) + 1; // 1-16
                channels = (Data[4] >> 4) % 4 + 1; // 1-4
                user_ptr = reinterpret_cast<void*>(const_cast<uint8_t*>(Data));
            }
            
            png_set_user_transform_info(png_ptr, user_ptr, depth, channels);
        }
        
        if (transform_flags & 0x10) {
            png_set_strip_alpha(png_ptr);
        }
    }
    
    // Read the PNG image with transformations
    png_read_png(png_ptr, info_ptr, 0, nullptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}