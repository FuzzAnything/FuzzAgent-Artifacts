// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_mDCV at pngset.c:297:1 in png.h
// png_get_mDCV at pngget.c:860:1 in png.h
// png_set_mDCV_fixed at pngset.c:225:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_set_cLLI at pngset.c:194:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
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
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy write function
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Do nothing - dummy flush function
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from fuzzer input
    png_voidp io_ptr = png_get_io_ptr(png_ptr);
    uint8_t** input_ptr = static_cast<uint8_t**>(io_ptr);
    
    if (length > 0 && *input_ptr) {
        size_t to_copy = length;
        memcpy(data, *input_ptr, to_copy);
        *input_ptr += to_copy;
    }
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }
    
    // Initialize png_struct for reading
    png_structp read_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!read_ptr) {
        return 0;
    }
    
    // Initialize png_info for reading
    png_infop read_info = png_create_info_struct(read_ptr);
    if (!read_info) {
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Initialize png_struct for writing
    png_structp write_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr,
                                                   dummy_error_fn, dummy_warning_fn);
    if (!write_ptr) {
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    // Initialize png_info for writing
    png_infop write_info = png_create_info_struct(write_ptr);
    if (!write_info) {
        png_destroy_write_struct(&write_ptr, nullptr);
        png_destroy_read_struct(&read_ptr, &read_info, nullptr);
        return 0;
    }
    
    // Set up custom read function to read from fuzzer input
    uint8_t* mutable_data = const_cast<uint8_t*>(Data);
    uint8_t* data_ptr = mutable_data;
    png_set_read_fn(read_ptr, &data_ptr, dummy_read_fn);
    
    // Set up dummy write function
    png_set_write_fn(write_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    
    // Use fuzzer data to seed values for API calls
    // We'll use the first bytes to determine which functions to call and with what values
    
    if (Size >= 64) {
        // Extract values from fuzzer data for mDCV functions
        double white_x = *reinterpret_cast<const double*>(Data);
        double white_y = *reinterpret_cast<const double*>(Data + 8);
        double red_x = *reinterpret_cast<const double*>(Data + 16);
        double red_y = *reinterpret_cast<const double*>(Data + 24);
        double green_x = *reinterpret_cast<const double*>(Data + 32);
        double green_y = *reinterpret_cast<const double*>(Data + 40);
        double blue_x = *reinterpret_cast<const double*>(Data + 48);
        double blue_y = *reinterpret_cast<const double*>(Data + 56);
        
        double max_lum = 100.0;  // Reasonable default
        double min_lum = 0.1;    // Reasonable default
        
        // Test png_set_mDCV with double values
        png_set_mDCV(write_ptr, write_info, 
                     white_x, white_y,
                     red_x, red_y,
                     green_x, green_y,
                     blue_x, blue_y,
                     max_lum, min_lum);
        
        // Test png_get_mDCV with double values
        double out_white_x, out_white_y, out_red_x, out_red_y;
        double out_green_x, out_green_y, out_blue_x, out_blue_y;
        double out_max_lum, out_min_lum;
        
        png_uint_32 mdcv_result = png_get_mDCV(read_ptr, read_info,
                                              &out_white_x, &out_white_y,
                                              &out_red_x, &out_red_y,
                                              &out_green_x, &out_green_y,
                                              &out_blue_x, &out_blue_y,
                                              &out_max_lum, &out_min_lum);
        
        // Test fixed-point versions if we have enough data
        if (Size >= 96) {
            png_fixed_point int_white_x = *reinterpret_cast<const png_int_32*>(Data + 64);
            png_fixed_point int_white_y = *reinterpret_cast<const png_int_32*>(Data + 68);
            png_fixed_point int_red_x = *reinterpret_cast<const png_int_32*>(Data + 72);
            png_fixed_point int_red_y = *reinterpret_cast<const png_int_32*>(Data + 76);
            png_fixed_point int_green_x = *reinterpret_cast<const png_int_32*>(Data + 80);
            png_fixed_point int_green_y = *reinterpret_cast<const png_int_32*>(Data + 84);
            png_fixed_point int_blue_x = *reinterpret_cast<const png_int_32*>(Data + 88);
            png_fixed_point int_blue_y = *reinterpret_cast<const png_int_32*>(Data + 92);
            
            png_uint_32 max_lum_scaled = 1000000;  // 100 nits * 10000
            png_uint_32 min_lum_scaled = 1000;     // 0.1 nits * 10000
            
            // Test png_set_mDCV_fixed
            png_set_mDCV_fixed(write_ptr, write_info,
                              int_white_x, int_white_y,
                              int_red_x, int_red_y,
                              int_green_x, int_green_y,
                              int_blue_x, int_blue_y,
                              max_lum_scaled, min_lum_scaled);
            
            // Test png_get_mDCV_fixed
            png_fixed_point out_int_white_x, out_int_white_y;
            png_fixed_point out_int_red_x, out_int_red_y;
            png_fixed_point out_int_green_x, out_int_green_y;
            png_fixed_point out_int_blue_x, out_int_blue_y;
            png_uint_32 out_max_lum_scaled, out_min_lum_scaled;
            
            png_get_mDCV_fixed(read_ptr, read_info,
                              &out_int_white_x, &out_int_white_y,
                              &out_int_red_x, &out_int_red_y,
                              &out_int_green_x, &out_int_green_y,
                              &out_int_blue_x, &out_int_blue_y,
                              &out_max_lum_scaled, &out_min_lum_scaled);
        }
    }
    
    // Test cLLI functions if we have enough data
    if (Size >= 80) {
        double max_cll = *reinterpret_cast<const double*>(Data);
        double max_fall = *reinterpret_cast<const double*>(Data + 8);
        
        // Test png_set_cLLI
        png_set_cLLI(write_ptr, write_info, max_cll, max_fall);
        
        // Test png_get_cLLI
        double out_max_cll, out_max_fall;
        png_uint_32 clli_result = png_get_cLLI(read_ptr, read_info, 
                                              &out_max_cll, &out_max_fall);
        
        // Test fixed-point version
        if (Size >= 88) {
            png_uint_32 max_cll_scaled = *reinterpret_cast<const png_uint_32*>(Data + 16);
            png_uint_32 max_fall_scaled = *reinterpret_cast<const png_uint_32*>(Data + 20);
            
            // Test png_set_cLLI_fixed
            png_set_cLLI_fixed(write_ptr, write_info, max_cll_scaled, max_fall_scaled);
            
            // Test png_get_cLLI_fixed
            png_uint_32 out_max_cll_scaled, out_max_fall_scaled;
            png_get_cLLI_fixed(read_ptr, read_info, 
                              &out_max_cll_scaled, &out_max_fall_scaled);
        }
    }
    
    // Test png_get_oFFs if we have enough data
    if (Size >= 12) {
        png_int_32 offset_x, offset_y;
        int unit_type;
        png_uint_32 offs_result = png_get_oFFs(read_ptr, read_info, 
                                              &offset_x, &offset_y, &unit_type);
    }
    
    // Clean up
    png_destroy_write_struct(&write_ptr, &write_info);
    png_destroy_read_struct(&read_ptr, &read_info, nullptr);
    
    return 0;
}