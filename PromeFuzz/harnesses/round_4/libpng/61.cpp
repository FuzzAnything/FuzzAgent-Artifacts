// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_gamma at pngrtran.c:901:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_set_sRGB_gAMA_and_cHRM at pngset.c:831:1 in png.h
// png_get_gAMA at pngget.c:677:1 in png.h
// png_set_gAMA at pngset.c:365:1 in png.h
// png_set_cHRM at pngset.c:99:1 in png.h
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
#include <cstddef>
#include <cstring>

#include "png.h"

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}
static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from a dummy buffer
    static const uint8_t dummy_data[] = {0};
    memcpy(data, dummy_data, length < sizeof(dummy_data) ? length : sizeof(dummy_data));
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    // Create dummy file if needed
    const char* dummy_filename = "./dummy_file";
    FILE* dummy_file = fopen(dummy_filename, "wb");
    if (!dummy_file) return 0;
    if (Size > 0) {
        fwrite(Data, 1, Size, dummy_file);
    }
    fclose(dummy_file);
    
    // Initialize png_struct for reading
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;
    
    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up custom read function
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    
    // Try to read PNG header
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    png_read_info(png_ptr, info_ptr);
    
    // Fuzz png_get_gAMA_fixed
    png_fixed_point int_file_gamma;
    png_get_gAMA_fixed(png_ptr, info_ptr, &int_file_gamma);
    
    // Fuzz png_set_sRGB_gAMA_and_cHRM
    if (Size > 0) {
        int srgb_intent = Data[0] % 4;  // Valid intents are 0-3
        png_set_sRGB_gAMA_and_cHRM(png_ptr, info_ptr, srgb_intent);
    }
    
    // Fuzz png_get_gAMA
    double file_gamma;
    png_get_gAMA(png_ptr, info_ptr, &file_gamma);
    
    // Fuzz png_set_gAMA
    if (Size > 4) {
        double gamma_value;
        memcpy(&gamma_value, Data, sizeof(double));
        png_set_gAMA(png_ptr, info_ptr, gamma_value);
    }
    
    // Fuzz png_set_cHRM
    if (Size >= 8 * sizeof(double)) {
        double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
        size_t offset = 0;
        memcpy(&white_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&white_y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&red_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&red_y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&green_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&green_y, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&blue_x, Data + offset, sizeof(double)); offset += sizeof(double);
        memcpy(&blue_y, Data + offset, sizeof(double));
        
        png_set_cHRM(png_ptr, info_ptr, white_x, white_y, red_x, red_y, 
                     green_x, green_y, blue_x, blue_y);
    }
    
    // Fuzz png_set_gamma
    if (Size >= 2 * sizeof(double)) {
        double screen_gamma, override_file_gamma;
        memcpy(&screen_gamma, Data, sizeof(double));
        memcpy(&override_file_gamma, Data + sizeof(double), sizeof(double));
        png_set_gamma(png_ptr, screen_gamma, override_file_gamma);
    }
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}