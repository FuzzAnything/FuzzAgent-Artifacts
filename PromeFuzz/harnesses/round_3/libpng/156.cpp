// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_iCCP at pngset.c:859:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_iCCP at pngget.c:718:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "code/png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

static png_structp create_png_struct(bool for_write) {
    png_structp png_ptr;
    
    if (for_write) {
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                          nullptr, dummy_error_fn, dummy_warning_fn);
    } else {
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                         nullptr, dummy_error_fn, dummy_warning_fn);
    }
    
    return png_ptr;
}

static png_infop create_info_struct(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    return png_create_info_struct(png_ptr);
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        png_destroy_write_struct(&png_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_156(const uint8_t *Data, size_t Size) {
    if (Size < 50) return 0;
    
    // Create write structure for setting operations
    png_structp write_png = create_png_struct(true);
    if (!write_png) return 0;
    
    png_infop write_info = create_info_struct(write_png);
    if (!write_info) {
        cleanup_png(write_png, nullptr);
        return 0;
    }
    
    // Create read structure for getting operations
    png_structp read_png = create_png_struct(false);
    if (!read_png) {
        cleanup_png(write_png, write_info);
        return 0;
    }
    
    png_infop read_info = create_info_struct(read_png);
    if (!read_info) {
        cleanup_png(write_png, write_info);
        cleanup_png(read_png, nullptr);
        return 0;
    }
    
    // Extract parameters from fuzz data
    png_uint_32 width = *reinterpret_cast<const png_uint_32*>(Data);
    png_uint_32 height = *reinterpret_cast<const png_uint_32*>(Data + 4);
    int bit_depth = Data[8] % 17; // 0-16, but valid values are 1,2,4,8,16
    int color_type = Data[9] % 6; // 0-5, valid: 0,2,3,4,6
    int interlace = Data[10] % 2; // 0 or 1
    int compression = 0; // Must be 0
    int filter = 0; // Must be 0
    
    // Adjust to valid values
    if (bit_depth == 0) bit_depth = 8;
    if (color_type == 1) color_type = PNG_COLOR_TYPE_RGB;
    if (color_type == 5) color_type = PNG_COLOR_TYPE_RGBA;
    
    // Test png_set_IHDR
    png_set_IHDR(write_png, write_info, 
                 width & 0x7FFFFFFF, height & 0x7FFFFFFF, // Ensure reasonable sizes
                 bit_depth, color_type,
                 interlace, compression, filter);
    
    // Test png_set_iCCP
    const char* icc_name = "test_profile";
    int comp_type = Data[11] % 2; // 0 or 1
    png_uint_32 profile_len = Size > 100 ? 50 : Size - 50;
    if (profile_len > 0) {
        const png_byte* profile_data = Data + 50;
        png_set_iCCP(write_png, write_info, icc_name, comp_type, 
                     profile_data, profile_len);
    }
    
    // Test png_set_sRGB_gAMA_and_cHRM
    int srgb_intent = Data[12] % 4; // 0-3
    png_set_sRGB_gAMA_and_cHRM(write_png, write_info, srgb_intent);
    
    // Test png_get_valid
    png_uint_32 valid_flags = png_get_valid(read_png, read_info, PNG_INFO_iCCP);
    valid_flags = png_get_valid(read_png, read_info, PNG_INFO_cICP);
    valid_flags = png_get_valid(read_png, read_info, PNG_INFO_sRGB);
    
    // Test png_get_iCCP
    char* name_ptr = nullptr;
    int out_comp_type;
    png_byte* profile_ptr = nullptr;
    png_uint_32 proflen_out;
    png_uint_32 iccp_result = png_get_iCCP(read_png, read_info, 
                                           &name_ptr, &out_comp_type,
                                           &profile_ptr, &proflen_out);
    
    // Test png_get_cICP
    png_byte colour_primaries;
    png_byte transfer_function;
    png_byte matrix_coefficients;
    png_byte video_full_range_flag;
    png_uint_32 cicp_result = png_get_cICP(read_png, read_info,
                                           &colour_primaries, &transfer_function,
                                           &matrix_coefficients, &video_full_range_flag);
    
    // Cleanup
    cleanup_png(write_png, write_info);
    cleanup_png(read_png, read_info);
    
    return 0;
}