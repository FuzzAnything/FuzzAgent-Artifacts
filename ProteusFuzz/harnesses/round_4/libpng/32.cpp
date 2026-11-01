// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_oFFs at pngget.c:973:1 in png.h
// png_get_sCAL at pngget.c:1049:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_get_sCAL_s at pngget.c:1067:1 in png.h
// png_get_cHRM at pngget.c:526:1 in png.h
// png_get_cHRM_XYZ at pngget.c:559:1 in png.h
// png_get_cHRM_fixed at pngget.c:628:1 in png.h
// png_get_cHRM_XYZ_fixed at pngget.c:598:1 in png.h
// png_get_cICP at pngget.c:762:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_cLLI at pngget.c:808:1 in png.h
// png_get_mDCV_fixed at pngget.c:829:1 in png.h
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

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush_fn(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file to simulate a PNG input
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) return 0;
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to avoid crashes
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize IO callbacks
    png_set_read_fn(png_ptr, NULL, dummy_read_fn);
    png_set_write_fn(png_ptr, NULL, dummy_write_fn, dummy_flush_fn);
    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    // Attempt to read the IHDR chunk (required for most get functions)
    png_read_info(png_ptr, info_ptr);

    // Variables to hold output data for the target APIs
    double max_content_light_level, max_frame_average_light_level;
    png_fixed_point int_white_x, int_white_y, int_red_x, int_red_y, int_green_x, int_green_y, int_blue_x, int_blue_y;
    png_uint_32 mastering_display_maximum_luminance_scaled_by_10000, mastering_display_minimum_luminance_scaled_by_10000;
    png_int_32 offset_x, offset_y;
    int unit_type;
    int unit;
    double width, height;
    png_fixed_point width_fixed, height_fixed;
    char *swidth = NULL, *sheight = NULL;
    double white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
    double red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z;
    png_fixed_point int_white_x_cHRM, int_white_y_cHRM, int_red_x_cHRM, int_red_y_cHRM, int_green_x_cHRM, int_green_y_cHRM, int_blue_x_cHRM, int_blue_y_cHRM;
    png_fixed_point int_red_X, int_red_Y, int_red_Z, int_green_X, int_green_Y, int_green_Z, int_blue_X, int_blue_Y, int_blue_Z;
    png_byte colour_primaries, transfer_function, matrix_coefficients, video_full_range_flag;
    png_uint_32 width_ihdr, height_ihdr;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;

    // 1. png_get_cLLI
    png_get_cLLI(png_ptr, info_ptr, &max_content_light_level, &max_frame_average_light_level);

    // 2. png_get_mDCV_fixed
    png_get_mDCV_fixed(png_ptr, info_ptr, &int_white_x, &int_white_y, &int_red_x, &int_red_y,
                       &int_green_x, &int_green_y, &int_blue_x, &int_blue_y,
                       &mastering_display_maximum_luminance_scaled_by_10000, &mastering_display_minimum_luminance_scaled_by_10000);

    // 3. png_get_oFFs
    png_get_oFFs(png_ptr, info_ptr, &offset_x, &offset_y, &unit_type);

    // 4. png_get_sCAL (double)
    png_get_sCAL(png_ptr, info_ptr, &unit, &width, &height);
    // 4. png_get_sCAL_fixed
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width_fixed, &height_fixed);
    // 4. png_get_sCAL_s
    png_get_sCAL_s(png_ptr, info_ptr, &unit, &swidth, &sheight);
    if (swidth) free(swidth);
    if (sheight) free(sheight);

    // 5. png_get_cHRM (double)
    png_get_cHRM(png_ptr, info_ptr, &white_x, &white_y, &red_x, &red_y, &green_x, &green_y, &blue_x, &blue_y);
    // 5. png_get_cHRM_XYZ (double)
    png_get_cHRM_XYZ(png_ptr, info_ptr, &red_X, &red_Y, &red_Z, &green_X, &green_Y, &green_Z, &blue_X, &blue_Y, &blue_Z);
    // 5. png_get_cHRM_fixed
    png_get_cHRM_fixed(png_ptr, info_ptr, &int_white_x_cHRM, &int_white_y_cHRM, &int_red_x_cHRM, &int_red_y_cHRM,
                       &int_green_x_cHRM, &int_green_y_cHRM, &int_blue_x_cHRM, &int_blue_y_cHRM);
    // 5. png_get_cHRM_XYZ_fixed
    png_get_cHRM_XYZ_fixed(png_ptr, info_ptr, &int_red_X, &int_red_Y, &int_red_Z, &int_green_X, &int_green_Y, &int_green_Z,
                           &int_blue_X, &int_blue_Y, &int_blue_Z);

    // 6. png_get_cICP
    png_get_cICP(png_ptr, info_ptr, &colour_primaries, &transfer_function, &matrix_coefficients, &video_full_range_flag);

    // 7. png_get_IHDR
    png_get_IHDR(png_ptr, info_ptr, &width_ihdr, &height_ihdr, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Clean up dummy file
    remove(filename);

    return 0;
}