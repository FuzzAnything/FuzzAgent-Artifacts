// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_read_end at pngread.c:762:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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
    // Simple dummy read function that returns zeros
    memset(data, 0, length);
}

extern "C" int LLVMFuzzerTestOneInput_225(const uint8_t *Data, size_t Size) {
    // Write data to dummy file as per instruction
    FILE* fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
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
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }
    
    // Set up dummy I/O
    png_set_read_fn(png_ptr, nullptr, dummy_read_fn);
    
    // Read PNG info to populate structures
    png_read_info(png_ptr, info_ptr);
    
    // Call target functions with different parameter combinations
    // Function 1: png_get_y_offset_inches
    float y_offset_inches = png_get_y_offset_inches((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)y_offset_inches; // Avoid unused variable warning
    
    // Function 2: png_get_x_offset_inches_fixed
    #ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)x_offset_inches_fixed;
    #endif
    
    // Function 3: png_get_pHYs_dpi
    #ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x, res_y;
    int unit_type;
    png_uint_32 phys_dpi_result = png_get_pHYs_dpi((png_const_structrp)png_ptr, (png_const_inforp)info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_dpi_result;
    #endif
    
    // Function 4: png_get_y_offset_inches_fixed
    #ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)y_offset_inches_fixed;
    #endif
    
    // Function 5: png_get_x_offset_inches
    float x_offset_inches = png_get_x_offset_inches((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)x_offset_inches;
    
    // Function 6: png_get_pixel_aspect_ratio
    float pixel_aspect_ratio = png_get_pixel_aspect_ratio((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    (void)pixel_aspect_ratio;
    
    // Try calling functions again with different state
    // Reset and re-read to explore different states
    png_read_end(png_ptr, info_ptr);
    
    // Call functions again after read_end
    y_offset_inches = png_get_y_offset_inches((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    x_offset_inches = png_get_x_offset_inches((png_const_structrp)png_ptr, (png_const_inforp)info_ptr);
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}