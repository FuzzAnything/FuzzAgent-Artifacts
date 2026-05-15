// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_cHRM_fixed at pngset.c:39:1 in png.h
// png_set_cICP at pngset.c:136:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_cHRM_XYZ at pngset.c:115:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_set_iCCP at pngset.c:859:1 in png.h
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
#include <cstring>
#include <cstdlib>

static void dummy_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    // Create PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_png_error, dummy_png_warning);
    if (!png_ptr) {
        return 0;
    }
    
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
    
    // Use fuzzer data to generate parameters for API calls
    // We'll use the data to seed different parameter values
    
    // For png_set_cHRM_XYZ - need 9 double values
    if (Size >= 72) { // 9 doubles * 8 bytes each
        const uint8_t* data_ptr = Data;
        double params[9];
        for (int i = 0; i < 9 && (data_ptr + 8) <= Data + Size; i++) {
            memcpy(&params[i], data_ptr, sizeof(double));
            data_ptr += 8;
        }
        
        png_set_cHRM_XYZ(png_ptr, info_ptr,
                        params[0], params[1], params[2],
                        params[3], params[4], params[5],
                        params[6], params[7], params[8]);
    }
    
    // For png_set_sRGB_gAMA_and_cHRM - need 1 int
    if (Size >= 4) {
        int srgb_intent;
        memcpy(&srgb_intent, Data, sizeof(int));
        // Ensure valid intent value (0-3 per PNG spec)
        srgb_intent = srgb_intent & 3;
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    }
    
    // For png_set_iCCP - need name, compression_type, profile data
    if (Size >= 20) { // Minimum for name + compression + proflen
        const char* name = "dummy_profile";
        int compression_type = Data[0] & 1; // 0 or 1 per PNG spec
        png_uint_32 proflen = Size > 1000 ? 1000 : Size; // Limit profile size
        
        // Use remaining data as profile (skip first few bytes for other params)
        size_t profile_offset = 4;
        if (profile_offset < Size) {
            const png_byte* profile = Data + profile_offset;
            png_uint_32 actual_proflen = (png_uint_32)(Size - profile_offset);
            if (actual_proflen > 10000) actual_proflen = 10000; // Reasonable limit
            
            png_set_iCCP(png_ptr, info_ptr, name, compression_type, 
                        profile, actual_proflen);
        }
    }
    
    // For png_set_cHRM_fixed - need 8 png_fixed_point values
    if (Size >= 32) { // 8 * 4 bytes
        const uint8_t* data_ptr = Data;
        png_fixed_point params[8];
        for (int i = 0; i < 8 && (data_ptr + 4) <= Data + Size; i++) {
            memcpy(&params[i], data_ptr, sizeof(png_fixed_point));
            data_ptr += 4;
        }
        
        png_set_cHRM_fixed(png_ptr, info_ptr,
                          params[0], params[1],
                          params[2], params[3],
                          params[4], params[5],
                          params[6], params[7]);
    }
    
    // For png_set_cICP - need 4 png_byte values
    if (Size >= 4) {
        png_byte colour_primaries = Data[0];
        png_byte transfer_function = Data[1 % Size];
        png_byte matrix_coefficients = Data[2 % Size];
        png_byte video_full_range_flag = Data[3 % Size];
        
        png_set_cICP(png_ptr, info_ptr,
                    colour_primaries, transfer_function,
                    matrix_coefficients, video_full_range_flag);
    }
    
    // For png_get_cICP - retrieve if set
    {
        png_byte colour_primaries;
        png_byte transfer_function;
        png_byte matrix_coefficients;
        png_byte video_full_range_flag;
        
        png_uint_32 result = png_get_cICP(png_ptr, info_ptr,
                                         &colour_primaries, &transfer_function,
                                         &matrix_coefficients, &video_full_range_flag);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Also test png_set_cHRM_XYZ_fixed if enough data
    if (Size >= 36) { // 9 * 4 bytes
        const uint8_t* data_ptr = Data;
        png_fixed_point params[9];
        for (int i = 0; i < 9 && (data_ptr + 4) <= Data + Size; i++) {
            memcpy(&params[i], data_ptr, sizeof(png_fixed_point));
            data_ptr += 4;
        }
        
        // Use the fixed-point version if available
        #ifdef PNG_cHRM_SUPPORTED
        png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                              params[0], params[1], params[2],
                              params[3], params[4], params[5],
                              params[6], params[7], params[8]);
        #endif
    }
    
    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}