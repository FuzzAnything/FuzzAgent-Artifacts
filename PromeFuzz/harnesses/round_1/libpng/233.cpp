// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_sRGB at pngset.c:819:1 in png.h
// png_set_sBIT at pngset.c:804:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_write_info at pngwrite.c:228:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "code/png.h"

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy writer
}

static void dummy_flush_data(png_structp png_ptr) {
    // Do nothing - dummy writer
}

static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    // Error handler to prevent crashes on invalid input
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    // Warning handler
}

extern "C" int LLVMFuzzerTestOneInput_233(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data for meaningful fuzzing

    // Create PNG write structure
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, error_handler, warning_handler);
    if (!png_ptr) return 0;

    // Create PNG info structure
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

    // Set up dummy write functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);

    // Parse parameters from fuzz data
    size_t offset = 0;
    
    // Extract parameters for png_set_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    
    if (offset + 4 <= Size) {
        width = *reinterpret_cast<const png_uint_32*>(Data + offset);
        offset += 4;
    }
    if (offset + 4 <= Size) {
        height = *reinterpret_cast<const png_uint_32*>(Data + offset);
        offset += 4;
    }
    if (offset + 1 <= Size) {
        bit_depth = Data[offset] % 17; // Valid bit depths: 1,2,4,8,16
        offset += 1;
    }
    if (offset + 1 <= Size) {
        color_type = Data[offset] % 7; // Valid color types: 0,2,3,4,6
        offset += 1;
    }
    if (offset + 1 <= Size) {
        interlace_method = Data[offset] % 2; // 0 or 1
        offset += 1;
    }
    if (offset + 1 <= Size) {
        compression_method = Data[offset] % 2; // 0
        offset += 1;
    }
    if (offset + 1 <= Size) {
        filter_method = Data[offset] % 2; // 0 or 1
        offset += 1;
    }

    // Call png_set_IHDR
    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);

    // Extract and call png_set_sRGB
    if (offset + 1 <= Size) {
        int srgb_intent = Data[offset] % 4; // Valid intents: 0-3
        offset += 1;
        png_set_sRGB(png_ptr, info_ptr, srgb_intent);
    }

    // Extract and call png_set_sBIT
    if (offset + 4 <= Size) {
        png_color_8 sig_bit;
        sig_bit.red = Data[offset] % (bit_depth + 1);
        offset += 1;
        sig_bit.green = Data[offset] % (bit_depth + 1);
        offset += 1;
        sig_bit.blue = Data[offset] % (bit_depth + 1);
        offset += 1;
        sig_bit.alpha = Data[offset] % (bit_depth + 1);
        offset += 1;
        png_set_sBIT(png_ptr, info_ptr, &sig_bit);
    }

    // Extract and call png_set_iCCP
    if (offset + 20 <= Size) {
        const char* name = "dummy_profile";
        int compression_type = 0;
        if (offset + 1 <= Size) {
            compression_type = Data[offset] % 2; // 0 or 1
            offset += 1;
        }
        
        png_uint_32 proflen = 0;
        if (offset + 4 <= Size) {
            proflen = *reinterpret_cast<const png_uint_32*>(Data + offset);
            offset += 4;
            // Limit profile length to prevent excessive memory usage
            if (proflen > 1024) proflen = 1024;
        }
        
        if (proflen > 0 && offset + proflen <= Size) {
            const png_byte* profile = Data + offset;
            offset += proflen;
            png_set_iCCP(png_ptr, info_ptr, name, compression_type, profile, proflen);
        }
    }

    // Extract and call png_set_cHRM_fixed
    if (offset + 32 <= Size) {
        png_fixed_point int_white_x = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        png_fixed_point int_white_y = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        png_fixed_point int_red_x = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        png_fixed_point int_red_y = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        png_fixed_point int_green_x = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        png_fixed_point int_green_y = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        png_fixed_point int_blue_x = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        png_fixed_point int_blue_y = *reinterpret_cast<const png_fixed_point*>(Data + offset);
        offset += 4;
        
        png_set_cHRM_fixed(png_ptr, info_ptr, int_white_x, int_white_y,
                          int_red_x, int_red_y, int_green_x, int_green_y,
                          int_blue_x, int_blue_y);
    }

    // Call png_write_info
    png_write_info(png_ptr, info_ptr);

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}