// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_pixels_per_inch at pngget.c:357:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_pixels_per_meter at pngget.c:167:1 in png.h
// png_get_x_pixels_per_inch at pngget.c:363:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
#include <png.h>
#include <cstdio>
#include <cstddef>
#include <cstdint>

static void fuzz_png_resolution(const uint8_t *Data, size_t Size) {
    if (Size < 8) return;

    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return;

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

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    png_uint_32 ppi = png_get_pixels_per_inch(png_ptr, info_ptr);
    (void)ppi;

    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    png_uint_32 y_ppm = png_get_y_pixels_per_meter(png_ptr, info_ptr);
    (void)y_ppm;

    png_uint_32 ppm = png_get_pixels_per_meter(png_ptr, info_ptr);
    (void)ppm;

    png_uint_32 x_ppi = png_get_x_pixels_per_inch(png_ptr, info_ptr);
    (void)x_ppi;

    png_uint_32 valid_pHYs = png_get_valid(png_ptr, info_ptr, PNG_INFO_pHYs);
    (void)valid_pHYs;

    png_uint_32 valid_tRNS = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    (void)valid_tRNS;

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    fuzz_png_resolution(Data, Size);
    return 0;
}