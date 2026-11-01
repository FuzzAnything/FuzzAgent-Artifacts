// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void fuzzer_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort in fuzzing context
}

static void fuzzer_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Ignore warnings
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file for the fuzzer input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Initialize libpng info structure
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set error and warning handlers to prevent abort
    png_set_error_fn(png_ptr, NULL, fuzzer_error_fn, fuzzer_warning_fn);

    // Open the file
    fp = fopen(filename, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize I/O
    png_init_io(png_ptr, fp);

    // Use setjmp to catch errors
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        fclose(fp);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Read header information
    png_read_info(png_ptr, info_ptr);

    // Fuzz target APIs
    // 1. png_get_y_offset_inches
    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    (void)y_offset_inches;

#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches_fixed;
#endif

    // 2. png_get_pixel_aspect_ratio
    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    (void)pixel_aspect_ratio;

    // 3. png_get_x_offset_microns
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns;

    // 4. png_get_pixel_aspect_ratio_fixed
#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point pixel_aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)pixel_aspect_ratio_fixed;
#endif

    // 5. png_read_info (already called above, but we can call again if safe, though typically not)
    // Note: png_read_info should only be called once per read session.

    // 6. png_get_x_offset_inches
    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    (void)x_offset_inches;

#ifdef PNG_FIXED_POINT_SUPPORTED
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;
#endif

    // Additional related API: png_get_pHYs_dpi
#ifdef PNG_pHYs_SUPPORTED
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)res_x;
    (void)res_y;
    (void)unit_type;
#endif

    // Additional related API: png_get_x_offset_pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Remove temporary file
    remove(filename);

    return 0;
}