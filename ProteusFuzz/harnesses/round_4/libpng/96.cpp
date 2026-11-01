// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_IHDR at pngget.c:928:1 in png.h
// png_get_channels at pngget.c:481:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_valid at pngget.c:19:1 in png.h
// png_get_tRNS at pngget.c:1202:1 in png.h
// png_get_bKGD at pngget.c:502:1 in png.h
// png_get_num_plays at pngget.c:1389:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <setjmp.h>

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_read_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up dummy read function to avoid crashes during header reading simulation
    png_set_read_fn(png_ptr, NULL, dummy_read_data_fn);

    // We use setjmp/longjmp to catch potential libpng errors during internal operations
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Test png_get_IHDR
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_method, compression_method, filter_method;
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
                 &interlace_method, &compression_method, &filter_method);

    // Test png_get_channels
    png_byte channels = png_get_channels(png_ptr, info_ptr);
    (void)channels;

    // Test png_get_valid with various flags
    // Note: PNG_INFO_IHDR is not a standard flag for png_get_valid in all versions, 
    // so we stick to documented flags like tRNS and bKGD.
    png_uint_32 valid_tRNS = png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS);
    png_uint_32 valid_bKGD = png_get_valid(png_ptr, info_ptr, PNG_INFO_bKGD);
    (void)valid_tRNS;
    (void)valid_bKGD;

    // Test png_get_tRNS
    png_byte *trans_alpha = NULL;
    int num_trans = 0;
    png_color_16 *trans_color = NULL;
    png_uint_32 trns_result = png_get_tRNS(png_ptr, info_ptr, &trans_alpha, &num_trans, &trans_color);
    (void)trns_result;

    // Test png_get_bKGD
    png_color_16 *background = NULL;
    png_uint_32 bkdg_result = png_get_bKGD(png_ptr, info_ptr, &background);
    (void)bkdg_result;

    // Test png_get_num_plays (APNG specific)
    png_uint_32 num_plays = png_get_num_plays(png_ptr, info_ptr);
    (void)num_plays;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}