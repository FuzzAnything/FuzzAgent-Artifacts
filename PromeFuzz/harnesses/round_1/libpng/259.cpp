// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_cHRM_XYZ_fixed at pngset.c:62:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
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
#include <cstdlib>

extern "C" {
#include "code/png.h"
}

static void dummy_error_fn(png_structp, png_const_charp) {}
static void dummy_warning_fn(png_structp, png_const_charp) {}
static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Read from a dummy source - just fill with zeros
    memset(data, 0, length);
}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return nullptr;
    
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    return png_ptr;
}

static png_infop create_png_info(png_structp png_ptr) {
    if (!png_ptr) return nullptr;
    return png_create_info_struct(png_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_259(const uint8_t *Data, size_t Size) {
    // Initialize png structures
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) return 0;
    
    png_infop info_ptr = create_png_info(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Extract values from fuzz input for fixed-point parameters
    // We need at least 10 * 4 bytes for the 10 png_fixed_point values
    const size_t min_size = 40; // 10 * sizeof(png_fixed_point)
    png_fixed_point fixed_points[10] = {0};
    
    if (Size >= min_size) {
        for (int i = 0; i < 10 && (i * 4 + 3) < Size; i++) {
            fixed_points[i] = (Data[i * 4] << 24) | (Data[i * 4 + 1] << 16) | 
                             (Data[i * 4 + 2] << 8) | Data[i * 4 + 3];
        }
    } else {
        // If not enough data, use what we have
        for (size_t i = 0; i < Size / 4 && i < 10; i++) {
            fixed_points[i] = (Data[i * 4] << 24) | (Data[i * 4 + 1] << 16) | 
                             (Data[i * 4 + 2] << 8) | Data[i * 4 + 3];
        }
    }
    
    // Test png_set_cHRM_XYZ_fixed
    png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
                          fixed_points[0], fixed_points[1], fixed_points[2],
                          fixed_points[3], fixed_points[4], fixed_points[5],
                          fixed_points[6], fixed_points[7], fixed_points[8]);
    
    // Test png_get_cHRM_XYZ_fixed
    png_fixed_point out_fixed[10] = {0};
    png_uint_32 result = png_get_cHRM_XYZ_fixed(png_ptr, info_ptr,
                                               &out_fixed[0], &out_fixed[1], &out_fixed[2],
                                               &out_fixed[3], &out_fixed[4], &out_fixed[5],
                                               &out_fixed[6], &out_fixed[7], &out_fixed[8]);
    
    // Test png_get_cHRM_fixed
    png_fixed_point cHRM_fixed[8] = {0};
    png_get_cHRM_fixed(png_ptr, info_ptr,
                      &cHRM_fixed[0], &cHRM_fixed[1],
                      &cHRM_fixed[2], &cHRM_fixed[3],
                      &cHRM_fixed[4], &cHRM_fixed[5],
                      &cHRM_fixed[6], &cHRM_fixed[7]);
    
    // Test png_get_IHDR
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0;
    int interlace_method = 0, compression_method = 0, filter_method = 0;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                &interlace_method, &compression_method, &filter_method);
    
    // Test png_get_cHRM
    double cHRM_double[8] = {0};
    png_get_cHRM(png_ptr, info_ptr,
                &cHRM_double[0], &cHRM_double[1],
                &cHRM_double[2], &cHRM_double[3],
                &cHRM_double[4], &cHRM_double[5],
                &cHRM_double[6], &cHRM_double[7]);
    
    // Test png_get_sRGB
    int file_srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &file_srgb_intent);
    
    // Test png_get_cICP if supported
    #ifdef PNG_cICP_SUPPORTED
    png_byte colour_primaries = 0;
    png_byte transfer_function = 0;
    png_byte matrix_coefficients = 0;
    png_byte video_full_range_flag = 0;
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function,
                &matrix_coefficients, &video_full_range_flag);
    #endif
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}