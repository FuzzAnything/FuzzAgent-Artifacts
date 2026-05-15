// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
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

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function that reads from static buffer
    static const uint8_t* buffer = nullptr;
    static png_size_t buffer_size = 0;
    static png_size_t offset = 0;
    
    if (length == 0) return;
    
    // Initialize on first call
    if (buffer == nullptr) {
        buffer = (const uint8_t*)png_get_io_ptr(png_ptr);
        buffer_size = png_get_io_ptr(png_ptr) ? *((png_size_t*)png_get_io_ptr(png_ptr) + 1) : 0;
        offset = 0;
    }
    
    if (offset + length <= buffer_size) {
        memcpy(data, buffer + offset, length);
        offset += length;
    } else {
        png_error(png_ptr, "Read error");
    }
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - just prevent exit on error
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    if (Size < 100) return 0; // Need minimum data
    
    // Write data to dummy file
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
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
    
    // Initialize IO
    png_init_io(png_ptr, fp);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Test various API functions
    // 1. png_get_x_offset_inches_fixed
    #ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;
    #endif
    
    // 2. png_get_pixel_aspect_ratio
    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)pixel_aspect_ratio;
    
    #ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point pixel_aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)pixel_aspect_ratio_fixed;
    #endif
    
    // 3. png_set_oFFs
    // Use some data from input for parameters
    png_int_32 offset_x = (png_int_32)(Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
    png_int_32 offset_y = (png_int_32)(Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
    int unit_type = Data[8] & 1; // 0 or 1
    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);
    
    // 4. png_get_x_offset_microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns;
    
    // 5. png_get_y_offset_inches
    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)y_offset_inches;
    
    #ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches_fixed;
    #endif
    
    // 6. png_get_x_offset_inches
    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    (void)x_offset_inches;
    
    #ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_inches_fixed_again = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed_again;
    #endif
    
    // Test png_get_pHYs_dpi if supported
    #ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x, res_y;
    int unit_type_phys;
    png_uint_32 ret = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type_phys);
    (void)ret;
    #endif
    
    // Test png_get_x_offset_pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    // Try another approach: create PNG structures without file
    png_structp png_ptr2 = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    if (png_ptr2) {
        png_infop info_ptr2 = png_create_info_struct(png_ptr2);
        if (info_ptr2) {
            // Set up custom IO
            struct buffer_info {
                const uint8_t* data;
                size_t size;
            };
            buffer_info buf_info = {Data, Size};
            png_set_read_fn(png_ptr2, &buf_info, dummy_read_fn);
            
            // Try to read with error handling
            if (setjmp(png_jmpbuf(png_ptr2))) {
                // Error occurred, just clean up
            } else {
                png_read_info(png_ptr2, info_ptr2);
                
                // Test functions again with different state
                float pixel_aspect_ratio2 = png_get_pixel_aspect_ratio(png_ptr2, info_ptr2);
                (void)pixel_aspect_ratio2;
                
                png_set_oFFs(png_ptr2, info_ptr2, offset_y, offset_x, unit_type ^ 1);
                
                png_int_32 x_offset_microns2 = png_get_x_offset_microns(png_ptr2, info_ptr2);
                (void)x_offset_microns2;
            }
            
            png_destroy_read_struct(&png_ptr2, &info_ptr2, nullptr);
        } else {
            png_destroy_read_struct(&png_ptr2, nullptr, nullptr);
        }
    }
    
    return 0;
}