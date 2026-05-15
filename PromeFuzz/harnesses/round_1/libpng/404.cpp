// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_expand at pngrtran.c:915:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_shift at pngtrans.c:83:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_user_transform_info at pngtrans.c:805:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
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

static void dummy_read_function(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
}

extern "C" int LLVMFuzzerTestOneInput_404(const uint8_t *Data, size_t Size) {
    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
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
    
    // Set up dummy I/O
    png_set_read_fn(png_ptr, nullptr, dummy_read_function);
    
    // Parse fuzzer input to extract parameters
    // We need at least 15 bytes for various parameters
    if (Size < 15) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For png_color_8 structures (5 bytes each)
    png_color_8 true_bits;
    true_bits.red = Data[offset++];
    true_bits.green = Data[offset++];
    true_bits.blue = Data[offset++];
    true_bits.gray = Data[offset++];
    true_bits.alpha = Data[offset++];
    
    png_color_8 sig_bit;
    sig_bit.red = Data[offset++];
    sig_bit.green = Data[offset++];
    sig_bit.blue = Data[offset++];
    sig_bit.gray = Data[offset++];
    sig_bit.alpha = Data[offset++];
    
    // For user transform parameters
    int user_transform_depth = Data[offset++] % 16;  // Limit to reasonable depth
    int user_transform_channels = Data[offset++] % 5; // Limit to reasonable channels
    
    // For RGB to gray parameters
    int error_action = Data[offset++] % 3;  // 0-2 valid values
    png_fixed_point red_coeff = 0;
    png_fixed_point green_coeff = 0;
    
    // Extract fixed point coefficients if we have enough data
    if (Size >= offset + 8) {
        memcpy(&red_coeff, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&green_coeff, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }
    
    // Call target API functions
    
    // 1. png_set_shift
    png_set_shift(png_ptr, &true_bits);
    
    // 2. png_set_sBIT
    png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    
    // 3. png_set_user_transform_info
    // Use nullptr for user_transform_ptr as we don't have actual transform data
    png_set_user_transform_info(png_ptr, nullptr, user_transform_depth, user_transform_channels);
    
    // 4. png_set_rgb_to_gray_fixed
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red_coeff, green_coeff);
    
    // 5. png_get_rgb_to_gray_status
    png_get_rgb_to_gray_status(png_ptr);
    
    // 6. png_set_expand
    png_set_expand(png_ptr);
    
    // 7. png_get_sBIT (additional function from the list)
    png_color_8p retrieved_sig_bit = nullptr;
    png_get_sBIT(png_ptr, info_ptr, &retrieved_sig_bit);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}