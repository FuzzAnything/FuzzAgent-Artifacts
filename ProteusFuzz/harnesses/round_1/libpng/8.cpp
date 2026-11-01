// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
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

static void safe_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_read_struct(nullptr, &info_ptr, nullptr);
    }
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file to simulate a PNG file
    write_dummy_file(Data, Size);

    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        fclose(fp);
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        fclose(fp);
        return 0;
    }

    // Setup error handling to avoid crashes on invalid input
    if (setjmp(png_jmpbuf(png_ptr))) {
        safe_cleanup(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Attempt to read info; this may fail if data is not a valid PNG
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // Fuzz png_get_pixels_per_meter
    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    // Fuzz png_get_x_pixels_per_meter
    png_uint_32 xppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)xppm;

    // Fuzz png_get_valid with various flags
    png_uint_32 valid_hIST = png_get_valid(png_ptr, info_ptr, PNG_INFO_hIST);
    (void)valid_hIST;
    png_uint_32 valid_pHYs = png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs);
    (void)valid_pHYs;
    // PNG_INFO_IHDR is not a standard flag for png_get_valid, removed to fix compilation error.
    // IHDR is always present if png_read_info succeeded without error.

    // Fuzz png_get_hIST
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
    if (hist_result != 0 && hist != nullptr) {
        // Access histogram data safely if available
        for (int i = 0; i < 256; ++i) {
            (void)hist[i];
        }
    }

    // Cleanup
    safe_cleanup(png_ptr, info_ptr);
    fclose(fp);

    return 0;
}