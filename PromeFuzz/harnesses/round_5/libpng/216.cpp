// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static png_voidp dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_216(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Initialize png_struct for reading
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        dummy_error_fn, 
        dummy_warning_fn,
        nullptr, 
        dummy_malloc_fn, 
        dummy_free_fn);
    
    if (!png_ptr) {
        return 0;
    }
    
    // Initialize png_info
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
    
    // Open dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    
    // Read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Extract fuzzed parameters from input data
    // Use first part of data for parameters, rest for PNG content
    size_t param_offset = 0;
    
    // Prepare parameters for API calls
    png_color_16 background_color;
    if (Size >= sizeof(png_color_16) + 1) {
        memcpy(&background_color, Data, sizeof(png_color_16));
        param_offset += sizeof(png_color_16);
    } else {
        background_color.index = 0;
        background_color.red = 0;
        background_color.green = 0;
        background_color.blue = 0;
        background_color.gray = 0;
    }
    
    int background_gamma_code = 0;
    int need_expand = 0;
    png_fixed_point background_gamma_fixed = 0;
    double background_gamma = 1.0;
    
    if (Size >= param_offset + 4) {
        background_gamma_code = Data[param_offset] % 4;
        need_expand = Data[param_offset + 1] % 2;
        memcpy(&background_gamma_fixed, Data + param_offset + 2, sizeof(png_fixed_point));
        memcpy(&background_gamma, Data + param_offset + 6, sizeof(double));
        param_offset += 14;
    }
    
    png_fixed_point screen_gamma_fixed = 0;
    png_fixed_point override_file_gamma_fixed = 0;
    double screen_gamma = 1.0;
    double override_file_gamma = 1.0;
    
    if (Size >= param_offset + 8) {
        memcpy(&screen_gamma_fixed, Data + param_offset, sizeof(png_fixed_point));
        memcpy(&override_file_gamma_fixed, Data + param_offset + 4, sizeof(png_fixed_point));
        memcpy(&screen_gamma, Data + param_offset + 8, sizeof(double));
        memcpy(&override_file_gamma, Data + param_offset + 16, sizeof(double));
        param_offset += 24;
    }
    
    png_fixed_point file_gamma_fixed = 0;
    double file_gamma = 1.0;
    
    if (Size >= param_offset + 4) {
        memcpy(&file_gamma_fixed, Data + param_offset, sizeof(png_fixed_point));
        memcpy(&file_gamma, Data + param_offset + 4, sizeof(double));
        param_offset += 12;
    }
    
    int mode = 0;
    png_fixed_point output_gamma_fixed = 0;
    
    if (Size >= param_offset + 4) {
        mode = Data[param_offset] % 10;  // Use modulo to get valid mode values
        memcpy(&output_gamma_fixed, Data + param_offset + 1, sizeof(png_fixed_point));
    }
    
    // Call target API functions with fuzzed parameters
    // Try different combinations and orders
    
    // Option 1: Call png_set_gamma before reading (as recommended)
    png_set_gamma_fixed(png_ptr, screen_gamma_fixed, override_file_gamma_fixed);
    png_set_gamma(png_ptr, screen_gamma, override_file_gamma);
    
    // Option 2: Call png_set_background after reading header
    png_set_background_fixed(png_ptr, &background_color, background_gamma_code, 
                           need_expand, background_gamma_fixed);
    png_set_background(png_ptr, &background_color, background_gamma_code,
                      need_expand, background_gamma);
    
    // Option 3: Call png_set_gAMA
    png_set_gAMA_fixed(png_ptr, info_ptr, file_gamma_fixed);
    png_set_gAMA(png_ptr, info_ptr, file_gamma);
    
    // Option 4: Call png_set_alpha_mode_fixed
    png_set_alpha_mode_fixed(png_ptr, mode, output_gamma_fixed);
    
    // Try png_set_scale_16 if supported
    #ifdef PNG_READ_SCALE_16_TO_8_SUPPORTED
    png_set_scale_16(png_ptr);
    #endif
    
    // Try png_set_flush if supported
    #ifdef PNG_WRITE_FLUSH_SUPPORTED
    int nrows = 0;
    if (Size > 0) {
        nrows = Data[0] % 100;  // Reasonable number of rows
    }
    png_set_flush(png_ptr, nrows);
    #endif
    
    // Clean up
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}