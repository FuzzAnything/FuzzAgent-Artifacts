// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
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
#include <setjmp.h>

static jmp_buf fuzz_jmp_buf;
static int fuzz_jmp_status = 0;

static void fuzz_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    fuzz_jmp_status = 1;
    longjmp(fuzz_jmp_buf, 1);
}

static void fuzz_warning_handler(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    if (setjmp(fuzz_jmp_buf) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        fuzz_jmp_status = 0;
        return 0;
    }

    png_set_error_fn(png_ptr, nullptr, fuzz_error_handler, fuzz_warning_handler);

    int crit_action = Data[0] % 4;
    int ancil_action = (Data[0] >> 1) % 4;
    png_set_crc_action(png_ptr, crit_action, ancil_action);

    int benign_allowed = (Data[0] >> 2) % 2;
    png_set_benign_errors(png_ptr, benign_allowed);

    int error_action = (Data[0] >> 3) % 3;
    png_fixed_point red = ((png_fixed_point)(Data[0] << 8 | (Data[1] & 0xFF))) * 1000;
    png_fixed_point green = ((png_fixed_point)(Data[1] << 8 | (Data[2] & 0xFF))) * 1000;
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);

    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status;

    int check_invalid = (Data[0] >> 4) % 2;
    png_set_check_for_invalid_index(png_ptr, check_invalid);

    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fuzz_jmp_status = 0;

    return 0;
}