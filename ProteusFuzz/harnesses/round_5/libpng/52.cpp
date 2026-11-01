// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_sBIT at pngget.c:1140:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
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
#include <cstdio>
#include <cstring>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
}

static void safe_read_png(png_structp png_ptr, png_infop info_ptr, FILE *fp) {
    png_set_read_fn(png_ptr, fp, NULL);
    png_read_info(png_ptr, info_ptr);
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
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

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return 0;
    }

    png_set_read_fn(png_ptr, fp, NULL);
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_pixels_per_inch
    png_uint_32 ppi = png_get_pixels_per_inch(png_ptr, info_ptr);
    (void)ppi;

    // Fuzz png_get_x_pixels_per_meter
    png_uint_32 xppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)xppm;

    // Fuzz png_get_y_pixels_per_meter
    png_uint_32 yppm = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    (void)yppm;

    // Fuzz png_get_pHYs_dpi
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type = 0;
    png_uint_32 phy_result = png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type);
    (void)phy_result;
    (void)res_x;
    (void)res_y;
    (void)unit_type;

    // Fuzz png_get_sBIT
    png_color_8 sig_bit;
    png_color_8p sig_bit_ptr = &sig_bit;
    png_uint_32 sbit_result = png_get_sBIT(png_ptr, info_ptr, &sig_bit_ptr);
    (void)sbit_result;

    // Fuzz png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
}