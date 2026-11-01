// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_x_offset_inches_fixed at pngget.c:393:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_oFFs at pngset.c:465:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_x_offset_microns at pngget.c:244:1 in png.h
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
#include <cstring>
#include <cstdio>

static void dummy_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_set_read_fn(png_ptr, NULL, dummy_read_callback);
    png_set_error_fn(png_ptr, NULL, dummy_error_callback, dummy_warning_callback);

    // Initialize with minimal valid PNG header to allow parsing of chunks
    // We construct a minimal valid PNG in memory or rely on fuzzed data being parsed
    // For fuzzing, we will try to parse the input as a PNG file
    // However, since we don't have a real file, we'll simulate reading by setting up the structure
    // and then manually setting info fields to trigger the target functions

    // Attempt to read the input as a PNG file (it will likely fail, but that's okay for fuzzing)
    // We use a dummy file approach as per instructions
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Re-open for reading
    f = fopen(dummy_file, "rb");
    if (f) {
        png_set_read_fn(png_ptr, (png_voidp)f, dummy_read_callback);
        png_read_info(png_ptr, info_ptr);
        fclose(f);
    }

    // Manually set oFFs chunk data to test offset retrieval functions
    png_int_32 offset_x = 0;
    png_int_32 offset_y = 0;
    int unit_type = 0;

    if (Size >= 9) {
        offset_x = (png_int_32)Data[0] | ((png_int_32)Data[1] << 8) | ((png_int_32)Data[2] << 16) | ((png_int_32)Data[3] << 24);
        offset_y = (png_int_32)Data[4] | ((png_int_32)Data[5] << 8) | ((png_int_32)Data[6] << 16) | ((png_int_32)Data[7] << 24);
        unit_type = (int)Data[8];
    }

    png_set_oFFs(png_ptr, info_ptr, offset_x, offset_y, unit_type);

    // Test target API functions
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    png_int_32 x_offset_microns = png_get_x_offset_microns(png_ptr, info_ptr);
    (void)x_offset_microns;

    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;

    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    (void)y_offset_microns;

    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}