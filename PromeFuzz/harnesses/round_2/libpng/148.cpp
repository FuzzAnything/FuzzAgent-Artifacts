// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
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
    // Dummy read function that does nothing
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp message) {
    (void)png_ptr;
    (void)message;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp message) {
    (void)png_ptr;
    (void)message;
    // Don't actually longjmp to avoid complexity
}

extern "C" int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Open the dummy file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) {
        return 0;
    }
    
    // Initialize png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    // Initialize png_info
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
    
    // Set up reading
    png_init_io(png_ptr, fp);
    
    // Try to read PNG header
    png_read_info(png_ptr, info_ptr);
    
    // Now test the target functions
    // 1. png_get_x_offset_inches_fixed
    png_fixed_point x_offset_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_fixed;
    
    // 2. png_get_y_offset_inches_fixed
    png_fixed_point y_offset_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_fixed;
    
    // 3. png_get_pHYs_dpi
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_uint_32 phys_dpi_result = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_dpi_result;
    (void)res_x;
    (void)res_y;
    (void)unit_type;
    
    // 4. png_get_signature
    const png_byte* signature = png_get_signature(png_ptr, info_ptr);
    (void)signature;
    
    // 5. png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio_fixed;
    
    // 6. png_get_pHYs
    png_uint_32 res_x2 = 0, res_y2 = 0;
    int unit_type2 = 0;
    png_uint_32 phys_result = png_get_pHYs(png_ptr, info_ptr, &res_x2, &res_y2, &unit_type2);
    (void)phys_result;
    (void)res_x2;
    (void)res_y2;
    (void)unit_type2;
    
    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
    
    return 0;
}