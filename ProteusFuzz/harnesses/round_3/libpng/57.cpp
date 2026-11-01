// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void dummy_error_callback(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_callback(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void cleanup_png(png_structp *png_ptr, png_infop *info_ptr) {
    if (*png_ptr) {
        png_destroy_read_struct(png_ptr, info_ptr, nullptr);
        *png_ptr = nullptr;
    }
    if (*info_ptr) {
        *info_ptr = nullptr;
    }
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a temporary file to simulate reading a PNG
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize PNG read structure
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_callback, dummy_warning_callback);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    // Set up error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    // Open the file for reading
    fp = fopen(filename, "rb");
    if (!fp) {
        cleanup_png(&png_ptr, &info_ptr);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read the PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Close the file
    fclose(fp);

    // Fuzz target API functions

    // 1. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // 2. png_get_pixels_per_meter
    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    // 3. png_get_pHYs
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_uint_32 phys_flag = png_get_pHYs(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phys_flag;

    // 4. png_get_cLLI_fixed
    png_uint_32 max_cll = 0, max_fall = 0;
    png_uint_32 clli_flag = png_get_cLLI_fixed(png_ptr, info_ptr, &max_cll, &max_fall);
    (void)clli_flag;

    // 5. png_get_pHYs_dpi
    png_uint_32 dpi_x = 0, dpi_y = 0;
    int dpi_unit_type = 0;
    png_uint_32 dpi_flag = png_get_pHYs_dpi(png_ptr, info_ptr, &dpi_x, &dpi_y, &dpi_unit_type);
    (void)dpi_flag;

    // 6. png_get_y_pixels_per_meter
    png_uint_32 y_ppm = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    (void)y_ppm;

    // Cleanup
    cleanup_png(&png_ptr, &info_ptr);

    // Clean up dummy file
    remove(filename);

    return 0;
}