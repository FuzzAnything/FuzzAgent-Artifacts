// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_alpha_mode at pngrtran.c:444:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_set_scale_16 at pngrtran.c:171:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "code/png.h"
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy read function that does nothing
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Dummy write function that does nothing
}

static void dummy_flush_fn(png_structp png_ptr) {
    // Dummy flush function that does nothing
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Dummy error handler
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Dummy warning handler
}

extern "C" int LLVMFuzzerTestOneInput_423(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Initialize IO
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    png_set_write_fn(png_ptr, nullptr, dummy_write_fn, dummy_flush_fn);
    png_set_error_fn(png_ptr, nullptr, dummy_error_fn, dummy_warning_fn);
    
    // Read PNG header from dummy file
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);
    fclose(fp);
    
    // Extract parameters from fuzzer input
    size_t offset = 0;
    
    // For png_color_16
    png_color_16 background_color;
    if (Size - offset >= 9) {
        background_color.index = Data[offset++];
        background_color.red = (Data[offset] << 8) | Data[offset + 1]; offset += 2;
        background_color.green = (Data[offset] << 8) | Data[offset + 1]; offset += 2;
        background_color.blue = (Data[offset] << 8) | Data[offset + 1]; offset += 2;
        background_color.gray = (Data[offset] << 8) | Data[offset + 1]; offset += 2;
    } else {
        background_color.index = 0;
        background_color.red = background_color.green = background_color.blue = background_color.gray = 0;
    }
    
    // For gamma codes and modes
    int background_gamma_code = 0;
    int need_expand = 0;
    int mode = 0;
    
    if (Size - offset >= 3) {
        background_gamma_code = Data[offset++] % 4; // 0-3
        need_expand = Data[offset++] % 2; // 0 or 1
        mode = Data[offset++] % 5; // 0-4 for different alpha modes
    }
    
    // For gamma values (double and fixed-point)
    double background_gamma_double = 1.0;
    double screen_gamma_double = 1.0;
    double override_file_gamma_double = 1.0;
    double output_gamma_double = 1.0;
    
    png_fixed_point background_gamma_fixed = 100000; // PNG_FP_1
    png_fixed_point screen_gamma_fixed = 100000;
    png_fixed_point override_file_gamma_fixed = 100000;
    png_fixed_point output_gamma_fixed = 100000;
    
    if (Size - offset >= 32) {
        // Extract 4 doubles (8 bytes each) from input
        uint64_t temp;
        memcpy(&temp, Data + offset, 8);
        background_gamma_double = *reinterpret_cast<double*>(&temp);
        offset += 8;
        
        memcpy(&temp, Data + offset, 8);
        screen_gamma_double = *reinterpret_cast<double*>(&temp);
        offset += 8;
        
        memcpy(&temp, Data + offset, 8);
        override_file_gamma_double = *reinterpret_cast<double*>(&temp);
        offset += 8;
        
        memcpy(&temp, Data + offset, 8);
        output_gamma_double = *reinterpret_cast<double*>(&temp);
        offset += 8;
        
        // Extract 4 fixed-point values (4 bytes each)
        memcpy(&background_gamma_fixed, Data + offset, 4);
        offset += 4;
        memcpy(&screen_gamma_fixed, Data + offset, 4);
        offset += 4;
        memcpy(&override_file_gamma_fixed, Data + offset, 4);
        offset += 4;
        memcpy(&output_gamma_fixed, Data + offset, 4);
        offset += 4;
    }
    
    // Call target API functions
    // 1. png_set_background
    png_set_background(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma_double);
    
    // 2. png_set_background_fixed
    png_set_background_fixed(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma_fixed);
    
    // 3. png_set_alpha_mode
    png_set_alpha_mode(png_ptr, mode, output_gamma_double);
    
    // 4. png_set_alpha_mode_fixed
    png_set_alpha_mode_fixed(png_ptr, mode, output_gamma_fixed);
    
    // 5. png_set_gamma
    png_set_gamma(png_ptr, screen_gamma_double, override_file_gamma_double);
    
    // 6. png_set_gamma_fixed
    png_set_gamma_fixed(png_ptr, screen_gamma_fixed, override_file_gamma_fixed);
    
    // Additional function mentioned in the defines
    png_set_scale_16(png_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}