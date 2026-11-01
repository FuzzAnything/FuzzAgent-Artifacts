// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_set_sig_bytes at png.c:53:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_offset_pixels at pngget.c:284:1 in png.h
// png_get_y_offset_pixels at pngget.c:304:1 in png.h
// png_get_y_offset_microns at pngget.c:264:1 in png.h
// png_get_pixel_aspect_ratio_fixed at pngget.c:213:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <vector>

static void
fuzz_read_png(const uint8_t *data, size_t size) {
    if (size == 0) return;

    // Create a temporary file for the fuzzer input
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file) return;
    file.write(reinterpret_cast<const char*>(data), size);
    file.close();

    // Open the file for reading
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return;

    // Initialize libpng read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return;
    }

    // Set up error handling to prevent crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);

    // Check for PNG signature
    unsigned char header[8];
    size_t header_size = fread(header, 1, 8, fp);
    if (header_size == 8 && png_sig_cmp(header, 0, 8) == 0) {
        png_set_sig_bytes(png_ptr, 8);
    } else {
        // If not a valid PNG, try to read anyway to test robustness, but reset file pointer
        rewind(fp);
    }

    // Read the header and info
    png_read_info(png_ptr, info_ptr);

    // Call target API functions
    // 1. png_get_x_offset_pixels
    png_int_32 x_offset_pixels = png_get_x_offset_pixels(png_ptr, info_ptr);
    (void)x_offset_pixels;

    // 2. png_get_y_offset_pixels
    png_int_32 y_offset_pixels = png_get_y_offset_pixels(png_ptr, info_ptr);
    (void)y_offset_pixels;

    // 4. png_get_y_offset_microns
    png_int_32 y_offset_microns = png_get_y_offset_microns(png_ptr, info_ptr);
    (void)y_offset_microns;

    // 5. png_get_pixel_aspect_ratio_fixed
    png_fixed_point aspect_ratio = png_get_pixel_aspect_ratio_fixed(png_ptr, info_ptr);
    (void)aspect_ratio;

    // 6. png_get_x_offset_inches_fixed
    png_fixed_point x_offset_inches_fixed = png_get_x_offset_inches_fixed(png_ptr, info_ptr);
    (void)x_offset_inches_fixed;

    // 3. png_get_y_offset_inches_fixed
    png_fixed_point y_offset_inches_fixed = png_get_y_offset_inches_fixed(png_ptr, info_ptr);
    (void)y_offset_inches_fixed;

    // Additional: png_get_pHYs_dpi
    png_uint_32 res_x, res_y;
    int unit_type;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)res_x;
    (void)res_y;
    (void)unit_type;

    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(fp);
}

extern "C" int
LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    fuzz_read_png(Data, Size);
    return 0;
}