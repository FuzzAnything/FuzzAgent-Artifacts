// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
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
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "code/png.h"

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Simple dummy read function that returns zeros
    for (png_size_t i = 0; i < length; ++i) {
        data[i] = 0;
    }
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_message) {
    // Ignore warnings
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_message) {
    // Longjmp on error
    longjmp(png_jmpbuf(png_ptr), 1);
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    // Write input to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open file for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    // Check PNG signature
    png_byte header[8];
    if (fread(header, 1, 8, fp) != 8) {
        fclose(fp);
        return 0;
    }
    if (png_sig_cmp(header, 0, 8)) {
        fclose(fp);
        return 0;
    }

    // Create PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    // Initialize PNG reading
    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    // Read PNG info
    png_read_info(png_ptr, info_ptr);

    // Test target functions
    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)x_offset_inches;
    (void)y_offset_inches;

#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;
    (void)y_offset_inches_fixed;
#endif

#ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x, res_y;
    int unit_type;
    png_uint_32 phys_result = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_result;
#endif

    png_int_32 offset_x, offset_y;
    int unit_type_offs;
    png_uint_32 offs_result = png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type_offs);
    (void)offs_result;

    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)pixel_aspect_ratio;

#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point pixel_aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)pixel_aspect_ratio_fixed;
#endif

    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}