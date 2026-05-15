// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_user_width_max at pngget.c:1305:1 in png.h
// png_get_user_height_max at pngget.c:1311:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}

extern "C" int LLVMFuzzerTestOneInput_390(const uint8_t *Data, size_t Size) {
    // Initialize write structures for png_set_IHDR and png_set_iCCP
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, 
                                                    nullptr, 
                                                    dummy_error_fn, 
                                                    dummy_warning_fn);
    if (!write_ptr) return 0;
    
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        return 0;
    }
    
    // Initialize read structures for png_read_info and limit functions
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
                                                  nullptr,
                                                  dummy_error_fn,
                                                  dummy_warning_fn);
    if (!read_ptr) {
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        png_destroy_write_struct(&write_ptr, &write_info);
        return 0;
    }
    
    // Use fuzzer data to generate parameters
    // Extract values from Data, using safe bounds checking
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    
    if (Size >= 28) {  // Enough for 7 * 4 bytes
        width = *reinterpret_cast<const png_uint_32*>(Data);
        height = *reinterpret_cast<const png_uint_32*>(Data + 4);
        bit_depth = *reinterpret_cast<const int*>(Data + 8);
        color_type = *reinterpret_cast<const int*>(Data + 12);
        interlace_method = *reinterpret_cast<const int*>(Data + 16);
        compression_method = *reinterpret_cast<const int*>(Data + 20);
        filter_method = *reinterpret_cast<const int*>(Data + 24);
    } else {
        // Use some default values if not enough data
        width = height = 100;
        bit_depth = 8;
        color_type = PNG_COLOR_TYPE_RGB;
        interlace_method = PNG_INTERLACE_NONE;
        compression_method = PNG_COMPRESSION_TYPE_DEFAULT;
        filter_method = PNG_FILTER_TYPE_DEFAULT;
    }
    
    // Test png_set_IHDR
    png_set_IHDR(write_ptr, write_info, width, height, bit_depth, color_type,
                 interlace_method, compression_method, filter_method);
    
    // Test png_set_user_limits and get functions
    png_set_user_limits(read_ptr, width, height);
    png_uint_32 max_width = png_get_user_width_max(read_ptr);
    png_uint_32 max_height = png_get_user_height_max(read_ptr);
    
    // Test png_set_iCCP
    if (Size > 44) {  // Need some data for name and profile
        const char* name = "dummy_profile";
        int compression_type = PNG_COMPRESSION_TYPE_BASE;
        png_uint_32 proflen = static_cast<png_uint_32>(Size > 100 ? 100 : Size);
        const png_byte* profile = Data;
        
        png_set_iCCP(write_ptr, write_info, name, compression_type, 
                     profile, proflen);
    }
    
    // Test png_read_info with dummy file
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
        
        dummy_file = fopen("./dummy_file", "rb");
        if (dummy_file) {
            png_init_io(read_ptr, dummy_file);
            // Try to read info - may fail if data isn't valid PNG
            png_read_info(read_ptr, read_info);
            fclose(dummy_file);
            remove("./dummy_file");
        }
    }
    
    // Clean up
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    png_destroy_write_struct(&write_ptr, &write_info);
    
    return 0;
}