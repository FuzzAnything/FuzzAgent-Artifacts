// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_filler at pngtrans.c:119:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_set_gray_to_rgb at pngrtran.c:998:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
// png_read_update_info at pngread.c:260:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_color_type at pngget.c:89:1 in png.h
// png_get_bit_depth at pngget.c:80:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_read_row at pngread.c:376:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_invert_alpha at pngtrans.c:236:1 in png.h
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
#include <cstddef>
#include <cstring>
#include <cstdlib>

#include "png.h"

// Structure to hold our memory buffer for PNG reading
struct png_memory_buffer {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

// Custom read function for libpng to read from memory buffer
static void png_read_memory(png_structp png_ptr, png_bytep out, png_size_t length) {
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (io_ptr == NULL) return;
    
    png_memory_buffer* buf = (png_memory_buffer*)io_ptr;
    
    // Check if we have enough data to read
    if (buf->offset + length > buf->size) {
        png_error(png_ptr, "Read beyond buffer");
        return;
    }
    
    memcpy(out, buf->data + buf->offset, length);
    buf->offset += length;
}

// Error handling callback
static void png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - just prevent libpng from exiting
}

// Warning handling callback  
static void png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // Need at least 8 bytes for PNG signature
    if (Size < 8) return 0;
    
    // Check PNG signature
    if (png_sig_cmp(Data, 0, 8) != 0) {
        // Not a PNG file, but we can still try to process it
        // or return early. Let's try to process anyway.
    }
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, 
                                                  png_error_handler, png_warning_handler);
    if (!png_ptr) return 0;
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }
    
    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    
    // Set up custom read function
    png_memory_buffer buf = {Data, Size, 0};
    png_set_read_fn(png_ptr, &buf, png_read_memory);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Now we can call the target API functions
    
    // Use some bytes from the input to decide which functions to call
    // and with what parameters
    if (Size > 8) {
        uint8_t selector = Data[0];
        
        // Call png_set_invert_alpha based on input
        if (selector & 0x01) {
            png_set_invert_alpha(png_ptr);
        }
        
        // Call png_set_alpha_mode_fixed with parameters from input
        if (Size > 12 && (selector & 0x02)) {
            int mode = Data[1] % 4; // 0-3 for different modes
            png_fixed_point output_gamma;
            
            // Extract gamma value from input bytes
            if (Size > 16) {
                memcpy(&output_gamma, Data + 8, sizeof(png_fixed_point));
            } else {
                output_gamma = PNG_DEFAULT_sRGB;
            }
            
            png_set_alpha_mode_fixed(png_ptr, mode, output_gamma);
        }
        
        // Call png_set_filler with parameters from input
        if (Size > 16 && (selector & 0x04)) {
            png_uint_32 filler;
            int flags = Data[2] % 2; // 0 or 1 for PNG_FILLER_BEFORE or PNG_FILLER_AFTER
            
            // Extract filler value from input
            memcpy(&filler, Data + 12, sizeof(png_uint_32));
            
            png_set_filler(png_ptr, filler, flags);
        }
        
        // Call png_set_bgr based on input
        if (selector & 0x08) {
            png_set_bgr(png_ptr);
        }
        
        // Call png_set_gray_to_rgb based on input
        if (selector & 0x10) {
            png_set_gray_to_rgb(png_ptr);
        }
        
        // Call png_set_interlace_handling based on input
        if (selector & 0x20) {
            png_set_interlace_handling(png_ptr);
        }
    }
    
    // Update the info structure after transformations
    png_read_update_info(png_ptr, info_ptr);
    
    // Try to read some image data to exercise the transformations
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    png_byte color_type = png_get_color_type(png_ptr, info_ptr);
    png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    
    // Calculate row bytes
    png_size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    
    // Allocate memory for row pointers
    png_bytep row_pointers = (png_bytep)malloc(rowbytes);
    if (row_pointers) {
        // Read a few rows to exercise the transformations
        for (png_uint_32 y = 0; y < height && y < 10; y++) {
            png_read_row(png_ptr, row_pointers, NULL);
        }
        free(row_pointers);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    
    return 0;
}