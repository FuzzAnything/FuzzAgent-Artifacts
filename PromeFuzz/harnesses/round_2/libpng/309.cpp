// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
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
// png_read_info at pngread.c:102:1 in png.h
// png_set_swap_alpha at pngtrans.c:222:1 in png.h
// png_set_strip_alpha at pngrtran.c:198:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
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

struct FuzzData {
    const uint8_t* data;
    size_t size;
    size_t offset;
};

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that reads from fuzzer input
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    if (!io_ptr) return;
    
    FuzzData* fuzz_data = static_cast<FuzzData*>(io_ptr);
    if (fuzz_data->offset + length > fuzz_data->size) {
        png_error(png_ptr, "Insufficient fuzzer data");
        return;
    }
    
    memcpy(data, fuzz_data->data + fuzz_data->offset, length);
    fuzz_data->offset += length;
}

extern "C" int LLVMFuzzerTestOneInput_309(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;  // Need minimum data for PNG header
    
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
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
    
    // Set up custom read function using fuzzer data
    FuzzData fuzz_data = {Data, Size, 0};
    png_set_read_fn(png_ptr, &fuzz_data, dummy_read_fn);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Apply transformations based on fuzzer input
    if (Size > 8) {
        uint8_t transform_flags = Data[8] % 64;  // Use 6 bits for 6 transformations
        
        if (transform_flags & 0x01) {
            png_set_swap_alpha(png_ptr);
        }
        
        if (transform_flags & 0x02) {
            png_set_strip_alpha(png_ptr);
        }
        
        if (transform_flags & 0x04) {
            // Use fuzzer data for RGB to gray parameters
            int error_action = (Data[9] % 2) ? 1 : 2;  // PNG_ERROR_ACTION_NONE or PNG_ERROR_ACTION_WARN
            png_fixed_point red = 0;
            png_fixed_point green = 0;
            
            if (Size > 12) {
                // Use bytes 10-13 for fixed point values
                red = *reinterpret_cast<const png_int_32*>(&Data[10]);
                green = *reinterpret_cast<const png_int_32*>(&Data[14]);
            }
            
            png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);
            
            // Check status
            png_get_rgb_to_gray_status(png_ptr);
        }
        
        if (transform_flags & 0x08) {
            // Set user transform info
            int depth = (Data[18] % 16) + 1;  // 1-16 bits
            int channels = (Data[19] % 4) + 1;  // 1-4 channels
            png_set_user_transform_info(png_ptr, nullptr, depth, channels);
        }
        
        if (transform_flags & 0x10) {
            png_set_palette_to_rgb(png_ptr);
        }
    }
    
    // Read the PNG image with transformations
    png_read_png(png_ptr, info_ptr, 0, nullptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}