// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_background_fixed at pngrtran.c:125:1 in png.h
// png_set_gAMA_fixed at pngset.c:351:1 in png.h
// png_set_background at pngrtran.c:155:1 in png.h
// png_set_gamma_fixed at pngrtran.c:859:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_alpha_mode_fixed at pngrtran.c:344:1 in png.h
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

#include "png.h"

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

extern "C" int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Create dummy file if needed
    FILE* dummy_file = fopen("./dummy_file", "wb");
    if (!dummy_file) {
        return 0;
    }
    fwrite(Data, 1, Size, dummy_file);
    fclose(dummy_file);

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

    // Set error handlers
    png_set_error_fn(png_ptr, nullptr, dummy_error_fn, dummy_warning_fn);

    // Set up dummy I/O
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);

    // Parse some parameters from fuzzing input
    size_t offset = 0;
    
    // Extract parameters for png_set_rgb_to_gray_fixed
    int error_action = Data[offset++] % 3;
    png_fixed_point red_coeff = 0;
    png_fixed_point green_coeff = 0;
    
    if (offset + 8 <= Size) {
        memcpy(&red_coeff, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
        memcpy(&green_coeff, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }

    // Call png_set_rgb_to_gray_fixed
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red_coeff, green_coeff);
    
    // Call png_get_rgb_to_gray_status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Use result to avoid unused variable warning

    // Extract parameters for png_set_alpha_mode_fixed
    int alpha_mode = Data[offset++] % 6;
    png_fixed_point output_gamma = 0;
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&output_gamma, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }

    // Call png_set_alpha_mode_fixed
    png_set_alpha_mode_fixed(png_ptr, alpha_mode, output_gamma);

    // Prepare background color structure
    png_color_16 background_color;
    background_color.index = Data[offset++] % 256;
    background_color.red = (offset + 2 <= Size) ? (Data[offset] << 8) | Data[offset + 1] : 0;
    offset = (offset + 2 <= Size) ? offset + 2 : offset;
    background_color.green = (offset + 2 <= Size) ? (Data[offset] << 8) | Data[offset + 1] : 0;
    offset = (offset + 2 <= Size) ? offset + 2 : offset;
    background_color.blue = (offset + 2 <= Size) ? (Data[offset] << 8) | Data[offset + 1] : 0;
    offset = (offset + 2 <= Size) ? offset + 2 : offset;
    background_color.gray = (offset + 2 <= Size) ? (Data[offset] << 8) | Data[offset + 1] : 0;
    offset = (offset + 2 <= Size) ? offset + 2 : offset;

    // Extract parameters for png_set_background_fixed
    int background_gamma_code = Data[offset++] % 4;
    int need_expand = Data[offset++] % 2;
    png_fixed_point background_gamma = 0;
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&background_gamma, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }

    // Call png_set_background_fixed
    png_set_background_fixed(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma);

    // Extract parameters for png_set_gAMA_fixed
    png_fixed_point file_gamma = 0;
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&file_gamma, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }

    // Call png_set_gAMA_fixed
    png_set_gAMA_fixed(png_ptr, info_ptr, file_gamma);

    // Call png_set_background (floating point version)
    double background_gamma_double = 0.0;
    if (offset + sizeof(double) <= Size) {
        memcpy(&background_gamma_double, Data + offset, sizeof(double));
        offset += sizeof(double);
    }
    
    png_set_background(png_ptr, &background_color, background_gamma_code, need_expand, background_gamma_double);

    // Extract parameters for png_set_gamma_fixed
    png_fixed_point screen_gamma = 0;
    png_fixed_point override_file_gamma = 0;
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&screen_gamma, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }
    
    if (offset + sizeof(png_fixed_point) <= Size) {
        memcpy(&override_file_gamma, Data + offset, sizeof(png_fixed_point));
        offset += sizeof(png_fixed_point);
    }

    // Call png_set_gamma_fixed
    png_set_gamma_fixed(png_ptr, screen_gamma, override_file_gamma);

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    // Remove dummy file
    remove("./dummy_file");

    return 0;
}