// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
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

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Ignore warnings during fuzzing
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Handle errors by longjmp
    (void)error_msg;
    png_longjmp(png_ptr, 1);
}

extern "C" int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Try to read the PNG file
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Check PNG signature
    png_byte header[8];
    size_t bytes_read = fread(header, 1, 8, fp);
    if (bytes_read != 8 || png_sig_cmp(header, 0, 8) != 0) {
        fclose(fp);
        return 0;
    }
    
    // Create PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    // Create PNG info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Use standard file I/O instead of custom read function
    png_init_io(png_ptr, fp);
    
    // We already read the signature
    png_set_sig_bytes(png_ptr, 8);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Variables for output parameters
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_fixed_point fixed_result = 0;
    png_int_32 int_result = 0;
    
    // Test png_get_pHYs
    png_uint_32 phys_result = png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_result;
    
    // Test png_get_y_offset_inches_fixed (if supported)
    #ifdef PNG_FIXED_POINT_SUPPORTED
    fixed_result = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)fixed_result;
    #endif
    
    // Test png_get_pixel_aspect_ratio_fixed (if supported)
    #ifdef PNG_FIXED_POINT_SUPPORTED
    fixed_result = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)fixed_result;
    #endif
    
    // Test png_get_pHYs_dpi (if supported)
    #ifdef PNG_pHYs_SUPPORTED
    phys_result = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_result;
    #endif
    
    // Test png_get_x_offset_inches_fixed (if supported)
    #ifdef PNG_FIXED_POINT_SUPPORTED
    fixed_result = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)fixed_result;
    #endif
    
    // Test png_get_pixels_per_meter
    png_uint_32 ppm_result = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm_result;
    
    // Test png_get_x_offset_pixels
    int_result = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)int_result;
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}