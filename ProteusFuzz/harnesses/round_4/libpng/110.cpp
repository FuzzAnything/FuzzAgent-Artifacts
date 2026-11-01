// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_png at pngread.c:957:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_pixel_aspect_ratio at pngget.c:189:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_y_offset_inches at pngget.c:424:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_x_offset_inches at pngget.c:413:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
#include <fstream>

static void fuzz_png_offset_functions(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    // Create a temporary file to hold the fuzzed PNG data
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) return;

    // Initialize PNG read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        fclose(fp);
        return;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG signature and header
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    // Call the target API functions
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);

    float pixel_aspect_ratio = png_get_pixel_aspect_ratio(png_ptr, info_ptr);
    png_fixed_point pixel_aspect_ratio_fixed = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);

    float y_offset_inches = png_get_y_offset_inches(png_ptr, info_ptr);
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);

    float x_offset_inches = png_get_x_offset_inches(png_ptr, info_ptr);
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);

    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    fuzz_png_offset_functions(Data, Size);
    return 0;
}