// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <setjmp.h>
#include <png.h>

static void custom_error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    png_longjmp(png_ptr, 1);
}

static void custom_warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 9) {
        return 0;
    }

    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, custom_error_handler, custom_warning_handler);
    
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    int allowed_index = Data[0] & 1;
    png_set_check_for_invalid_index(png_ptr, allowed_index);

    int allowed_benign = (Data[1] & 1) ? 1 : 0;
    png_set_benign_errors(png_ptr, allowed_benign);

    int crit_action = Data[2] % 3;
    int ancil_action = Data[3] % 3;
    png_set_crc_action(png_ptr, crit_action, ancil_action);

    int error_action = Data[4] % 3;
    png_fixed_point red = (png_fixed_point)((Data[5] << 8) | Data[6]);
    png_fixed_point green = (png_fixed_point)((Data[7] << 8) | Data[8]);
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);

    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status;

    png_read_info(png_ptr, info_ptr);

    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    remove(dummy_file);

    return 0;
}