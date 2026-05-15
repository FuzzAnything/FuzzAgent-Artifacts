// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_image_height at pngget.c:71:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <csetjmp>

#include "png.h"

struct buffer_state {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from fuzzer input buffer
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    buffer_state* state = static_cast<buffer_state*>(io_ptr);
    size_t to_read = length;
    if (state->offset + to_read > state->size) {
        to_read = state->size - state->offset;
    }
    
    if (to_read > 0) {
        memcpy(data, state->data + state->offset, to_read);
        state->offset += to_read;
    }
    
    // Fill remaining with zeros if needed
    if (to_read < length) {
        memset(data + to_read, 0, length - to_read);
    }
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler - longjmp to recover from errors
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler - do nothing
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }
    
    // Create png_struct for reading with error handler that does longjmp
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }
    
    // Create png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up setjmp for error recovery
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up custom read function
    buffer_state state = {Data, Size, 0};
    png_set_read_fn(png_ptr, &state, dummy_read_fn);
    
    // Test 1: Try to read PNG info (may fail for invalid PNGs, caught by setjmp)
    png_read_info(png_ptr, info_ptr);
    
    // Test 2: Get image width and height
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    png_uint_32 height = png_get_image_height(png_ptr, info_ptr);
    
    // Test 3: Set user limits using values derived from fuzzer input
    png_uint_32 user_width_max = 0;
    png_uint_32 user_height_max = 0;
    
    if (Size >= 8) {
        // Use first 4 bytes for width, next 4 for height
        memcpy(&user_width_max, Data, sizeof(png_uint_32));
        memcpy(&user_height_max, Data + 4, sizeof(png_uint_32));
        
        // Ensure reasonable values to avoid overflow
        user_width_max = user_width_max % 100000;
        user_height_max = user_height_max % 100000;
        
        png_set_user_limits(png_ptr, user_width_max, user_height_max);
    }
    
    // Test 4: Get user limits back
    png_uint_32 retrieved_width_max = png_get_user_width_max(png_ptr);
    png_uint_32 retrieved_height_max = png_get_user_height_max(png_ptr);
    
    // Test 5: Try reading info again after setting limits
    // Reset read position
    state.offset = 0;
    
    // Need to reset error handling for second read attempt
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred during second PNG processing
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_read_info(png_ptr, info_ptr);
    
    // Get dimensions again
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    
    // Test 6: Test with NULL parameters (should return 0)
    // Create separate structs for NULL tests to avoid affecting main struct
    png_structp null_png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                       dummy_error_fn, dummy_warning_fn);
    if (null_png_ptr) {
        png_uint_32 null_width = png_get_image_width(nullptr, nullptr);
        png_uint_32 null_height = png_get_image_height(nullptr, nullptr);
        png_uint_32 null_user_width = png_get_user_width_max(nullptr);
        png_uint_32 null_user_height = png_get_user_height_max(nullptr);
        png_destroy_read_struct(&null_png_ptr, nullptr, nullptr);
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}