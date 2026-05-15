// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_start_read_image at pngread.c:291:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_set_invert_mono at pngtrans.c:249:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_set_packing at pngtrans.c:49:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_bgr at pngtrans.c:20:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
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
#include <setjmp.h>

#include "png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep out, png_size_t length) {
    // Dummy read function that does nothing
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp msg) {
    // Dummy error handler that doesn't longjmp
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp msg) {
    // Dummy warning handler
}

struct buffer_state {
    const uint8_t *data;
    size_t size;
    size_t index;
};

static void read_from_buffer(png_structp png_ptr, png_bytep out, png_size_t length) {
    buffer_state *state = (buffer_state*)png_get_io_ptr(png_ptr);
    if (state->index + length > state->size) {
        // Not enough data, fill with zeros
        memset(out, 0, length);
        state->index = state->size;
    } else {
        memcpy(out, state->data + state->index, length);
        state->index += length;
    }
}

static void call_transformation_functions(png_structp png_ptr, uint8_t selector) {
    if (selector & 0x01) {
        png_set_invert_mono(png_ptr);
    }
    
    if (selector & 0x02) {
        png_set_expand(png_ptr);
    }
    
    if (selector & 0x04) {
        png_set_packing(png_ptr);
    }
    
    if (selector & 0x08) {
        png_set_swap_alpha(png_ptr);
    }
    
    if (selector & 0x10) {
        png_set_bgr(png_ptr);
    }
}

// A simple valid 1x1 grayscale PNG (no CRC issues)
static const uint8_t valid_minimal_png[] = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x08, 0x00, 0x00, 0x00, 0x00, 0x3A, 0x7E, 0x9B, 0x55, 0x00, 0x00, 0x00,
    0x0A, 0x49, 0x44, 0x41, 0x54, 0x08, 0x1D, 0x63, 0x60, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x01, 0xE2, 0x21, 0xBC, 0x33, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Create first PNG structure for transformation functions without header
    png_structp png_ptr1 = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr1) {
        return 0;
    }
    
    png_infop info_ptr1 = png_create_info_struct(png_ptr1);
    if (!info_ptr1) {
        png_destroy_read_struct(&png_ptr1, nullptr, nullptr);
        return 0;
    }
    
    // Set up dummy read function for first structure
    png_set_read_fn(png_ptr1, nullptr, dummy_read_fn);
    
    // Use fuzz data to determine which functions to call
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        // Call transformation functions on first structure
        call_transformation_functions(png_ptr1, selector);
        
        // Create second structure with valid PNG for png_start_read_image
        if (selector & 0x20) {
            png_structp png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                                         nullptr, nullptr);
            if (png_ptr2) {
                png_infop info_ptr2 = png_create_info_struct(png_ptr2);
                if (info_ptr2) {
                    // Set up error handling with setjmp
                    if (setjmp(png_jmpbuf(png_ptr2))) {
                        // Error occurred, clean up and continue
                        png_destroy_read_struct(&png_ptr2, &info_ptr2, nullptr);
                    } else {
                        // Set up buffer state for valid minimal PNG
                        buffer_state state;
                        state.data = valid_minimal_png;
                        state.size = sizeof(valid_minimal_png);
                        state.index = 0;
                        
                        // Set read function for second structure
                        png_set_read_fn(png_ptr2, &state, read_from_buffer);
                        
                        // Read PNG header
                        png_read_info(png_ptr2, info_ptr2);
                        
                        // Call transformation functions on second structure
                        call_transformation_functions(png_ptr2, selector);
                        
                        // Try to start read image
                        png_start_read_image(png_ptr2);
                        
                        // Clean up second structure
                        png_destroy_read_struct(&png_ptr2, &info_ptr2, nullptr);
                    }
                } else {
                    png_destroy_read_struct(&png_ptr2, nullptr, nullptr);
                }
            }
        }
        
        // Try calling png_start_read_image on first structure (should error)
        if (selector & 0x40) {
            // This will trigger an error since we haven't read a PNG header
            // But our dummy error handler doesn't longjmp, so it's safe
            png_start_read_image(png_ptr1);
        }
    }
    
    // Clean up first structure
    png_destroy_read_struct(&png_ptr1, &info_ptr1, nullptr);
    
    return 0;
}