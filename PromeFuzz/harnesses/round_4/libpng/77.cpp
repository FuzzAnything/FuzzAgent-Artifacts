// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_signature at pngget.c:491:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "png.h"
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *file = fopen("./dummy_file", "wb");
    if (!file) return;
    fwrite(data, 1, size, file);
    fclose(file);
}

static void cleanup_structures(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr && info_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (png_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    // Write input data to dummy file
    write_dummy_file(Data, Size);
    
    // Open the dummy file for reading
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;
    
    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_structures(png_ptr, nullptr);
        fclose(fp);
        return 0;
    }
    
    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_structures(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }
    
    // Initialize I/O
    png_init_io(png_ptr, fp);
    
    // Read PNG info
    png_read_info(png_ptr, info_ptr);
    
    // Test png_get_signature
    const png_byte* signature = png_get_signature(png_ptr, info_ptr);
    (void)signature; // Avoid unused variable warning
    
    // Test png_get_pHYs
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_uint_32 pHYs_result = png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)pHYs_result;
    
    // Test png_get_x_offset_inches_fixed
    png_fixed_point x_offset_inches = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches;
    
    // Test png_get_y_offset_inches_fixed
    png_fixed_point y_offset_inches = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches;
    
    // Test png_get_pixel_aspect_ratio_fixed
    png_fixed_point pixel_aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)pixel_aspect_ratio;
    
    // Test png_get_pHYs_dpi
    png_uint_32 res_x_dpi = 0, res_y_dpi = 0;
    int unit_type_dpi = 0;
    png_uint_32 pHYs_dpi_result = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x_dpi, &res_y_dpi, &unit_type_dpi);
    (void)pHYs_dpi_result;
    
    // Cleanup
    cleanup_structures(png_ptr, info_ptr);
    fclose(fp);
    
    return 0;
}