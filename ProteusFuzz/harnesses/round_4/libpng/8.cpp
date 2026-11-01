// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_y_offset_inches_fixed at pngget.c:403:1 in png.h
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
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
#include <cstddef>
#include <cstring>
#include <cstdio>
#include <vector>

static void fuzz_png_offset_and_aspect(const uint8_t *Data, size_t Size) {
    if (Size < 8) return; // Minimal size check for PNG header

    // Create a temporary file to hold the fuzzed PNG data
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Reopen for reading
    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    // Initialize libpng structures
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

    // Setup error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG signature and header
    // We do not call png_set_sig_bytes because the input might not be a valid PNG
    // and we want to test how libpng handles invalid data.
    // However, to reach the offset functions, we need to parse at least the IHDR and potentially oFFs/pHYs.
    // png_read_info reads up to the first IDAT.
    png_read_info(png_ptr, info_ptr);

    // Invoke the target API functions
    // These functions are safe to call even if the specific chunks are missing; they return 0.
    
    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns; // Suppress unused variable warning

    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches_fixed;

    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;

    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    fuzz_png_offset_and_aspect(Data, Size);
    return 0;
}