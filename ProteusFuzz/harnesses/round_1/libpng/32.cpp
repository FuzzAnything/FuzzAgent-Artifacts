// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_pHYs_dpi at pngget.c:435:1 in png.h
// png_get_y_pixels_per_meter at pngget.c:146:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_get_pHYs at pngget.c:1087:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_sRGB at pngget.c:699:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
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

static void png_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void png_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void read_png_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    if (fread(data, 1, length, static_cast<FILE*>(png_get_io_ptr(png_ptr))) != length) {
        png_error(png_ptr, "Read Error");
    }
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write input data to a dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    fp = fopen("./dummy_file", "rb");
    if (!fp) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, png_error_handler, png_warning_handler);
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

    png_set_read_fn(png_ptr, fp, read_png_data);

    // Read PNG header and info
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_get_sRGB
    int srgb_intent = 0;
    png_get_sRGB(png_ptr, info_ptr, &srgb_intent);
    png_get_sRGB(png_ptr, info_ptr, NULL);

    // Fuzz png_get_oFFs
    png_int_32 offset_x = 0, offset_y = 0;
    int unit_type = 0;
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
    png_get_oFFs(png_ptr, info_ptr, NULL, &offset_y, &unit_type);
    png_get_oFFs(png_ptr, info_ptr, &offset_x, NULL, &unit_type);
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, NULL);
    png_get_oFFs(png_ptr, info_ptr, NULL, NULL, NULL);

    // Fuzz png_get_cHRM_fixed
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y;
    png_fixed_point int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, NULL, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, NULL, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, NULL, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, NULL,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       NULL, &int_green_y, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, NULL, &int_blue_x, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, NULL, &int_blue_y);
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, NULL);
    png_get_cHRM_fixed(png_ptr, info_ptr, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    // Fuzz png_get_pHYs_dpi
    png_uint_32 res_x = 0, res_y = 0;
    int unit_type_dpi = 0;
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, &unit_type_dpi);
    png_get_pHYs_dpi(png_ptr, info_ptr, NULL, &res_y, &unit_type_dpi);
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, NULL, &unit_type_dpi);
    png_get_pHYs_dpi(png_ptr, info_ptr, &res_x, &res_y, NULL);
    png_get_pHYs_dpi(png_ptr, info_ptr, NULL, NULL, NULL);

    // Fuzz png_get_y_pixels_per_meter
    png_get_y_pixels_per_meter(png_ptr, info_ptr);

    // Fuzz png_get_pHYs
    png_uint_32 res_x_phys = 0, res_y_phys = 0;
    int unit_type_phys = 0;
    png_get_pHYs(png_ptr, info_ptr, &res_x_phys, &res_y_phys, &unit_type_phys);
    png_get_pHYs(png_ptr, info_ptr, NULL, &res_y_phys, &unit_type_phys);
    png_get_pHYs(png_ptr, info_ptr, &res_x_phys, NULL, &unit_type_phys);
    png_get_pHYs(png_ptr, info_ptr, &res_x_phys, &res_y_phys, NULL);
    png_get_pHYs(png_ptr, info_ptr, NULL, NULL, NULL);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);
    return 0;
}