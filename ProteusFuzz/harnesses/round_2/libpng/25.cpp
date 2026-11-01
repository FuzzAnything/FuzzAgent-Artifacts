// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
// png_get_oFFs at pngget.c:973:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_pCAL at pngget.c:994:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
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
#include <vector>

static void write_dummy_png(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static void cleanup_png(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

static void fuzz_png_get_cHRM(png_structp png_ptr, png_infop info_ptr, const uint8_t *data, size_t size) {
    double white_x = 0.0, white_y = 0.0;
    double red_x = 0.0, red_y = 0.0;
    double green_x = 0.0, green_y = 0.0;
    double blue_x = 0.0, blue_y = 0.0;
    double red_X = 0.0, red_Y = 0.0, red_Z = 0.0;
    double green_X = 0.0, green_Y = 0.0, green_Z = 0.0;
    double blue_X = 0.0, blue_Y = 0.0, blue_Z = 0.0;
    png_fixed_point int_white_x = 0, int_white_y = 0;
    png_fixed_point int_red_x = 0, int_red_y = 0;
    png_fixed_point int_green_x = 0, int_green_y = 0;
    png_fixed_point int_blue_x = 0, int_blue_y = 0;
    png_fixed_point int_red_X = 0, int_red_Y = 0, int_red_Z = 0;
    png_fixed_point int_green_X = 0, int_green_Y = 0, int_green_Z = 0;
    png_fixed_point int_blue_X = 0, int_blue_Y = 0, int_blue_Z = 0;

    if (png_ptr && info_ptr) {
        png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
        png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);
        png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y, &int_green_x, &int_green_y, &int_blue_x, &int_blue_y);
        png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z, &int_green_X, &int_green_Y, &int_green_Z, &int_blue_X, &int_blue_Y, &int_blue_Z);
    }
}

static void fuzz_png_get_pCAL(png_structp png_ptr, png_infop info_ptr) {
    char *purpose = nullptr;
    png_int_32 X0 = 0, X1 = 0;
    int type = 0, nparams = 0;
    char *units = nullptr;
    char **params = nullptr;

    if (png_ptr && info_ptr) {
        png_get_pCAL(png_ptr, info_ptr, &purpose, &X0, &X1, &type, &nparams, &units, &params);
    }
}

static void fuzz_png_get_IHDR(png_structp png_ptr, png_infop info_ptr) {
    png_uint_32 width = 0, height = 0;
    int bit_depth = 0, color_type = 0, interlace_method = 0, compression_method = 0, filter_method = 0;

    if (png_ptr && info_ptr) {
        png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type, &interlace_method, &compression_method, &filter_method);
    }
}

static void fuzz_png_get_cLLI(png_structp png_ptr, png_infop info_ptr) {
    double maximum_content_light_level = 0.0;
    double maximum_frame_average_light_level = 0.0;
    png_uint_32 fixed_max_content = 0, fixed_max_frame_avg = 0;

    if (png_ptr && info_ptr) {
        png_get_cLLI(png_ptr, info_ptr, &maximum_content_light_level, &maximum_frame_average_light_level);
        png_get_cLLI_fixed(png_ptr, info_ptr, &fixed_max_content, &fixed_max_frame_avg);
    }
}

static void fuzz_png_get_sCAL(png_structp png_ptr, png_infop info_ptr) {
    int unit = 0;
    double width = 0.0, height = 0.0;
    png_fixed_point fixed_width = 0, fixed_height = 0;
    char *swidth = nullptr, *sheight = nullptr;

    if (png_ptr && info_ptr) {
        png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);
        png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &fixed_width, &fixed_height);
        png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    }
}

static void fuzz_png_get_mDCV_fixed(png_structp png_ptr, png_infop info_ptr) {
    png_fixed_point int_white_x = 0, int_white_y = 0;
    png_fixed_point int_red_x = 0, int_red_y = 0;
    png_fixed_point int_green_x = 0, int_green_y = 0;
    png_fixed_point int_blue_x = 0, int_blue_y = 0;
    png_uint_32 mastering_display_maximum_luminance_scaled_by_10000 = 0;
    png_uint_32 mastering_display_minimum_luminance_scaled_by_10000 = 0;

    if (png_ptr && info_ptr) {
        png_get_mDCV_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y, &int_green_x, &int_green_y, &int_blue_x, &int_blue_y, &mastering_display_maximum_luminance_scaled_by_10000, &mastering_display_minimum_luminance_scaled_by_10000);
    }
}

static void fuzz_png_get_oFFs(png_structp png_ptr, png_infop info_ptr) {
    png_int_32 offset_x = 0, offset_y = 0;
    int unit_type = 0;

    if (png_ptr && info_ptr) {
        png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);
    }
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_png(Data, Size);

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

    if (setjmp(png_jmpbuf(png_ptr))) {
        cleanup_png(png_ptr, info_ptr);
        fclose(fp);
        return 0;
    }

    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    fuzz_png_get_cHRM(png_ptr, info_ptr, Data, Size);
    fuzz_png_get_pCAL(png_ptr, info_ptr);
    fuzz_png_get_IHDR(png_ptr, info_ptr);
    fuzz_png_get_cLLI(png_ptr, info_ptr);
    fuzz_png_get_sCAL(png_ptr, info_ptr);
    fuzz_png_get_mDCV_fixed(png_ptr, info_ptr);
    fuzz_png_get_oFFs(png_ptr, info_ptr);

    cleanup_png(png_ptr, info_ptr);
    fclose(fp);
    return 0;
}