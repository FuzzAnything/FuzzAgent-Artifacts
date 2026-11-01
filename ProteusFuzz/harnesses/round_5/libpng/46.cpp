// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_read_info at pngread.c:102:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_init_io at png.c:705:1 in png.h
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
#include <csetjmp>

static jmp_buf fuzz_jmp_buf;
static int fuzz_jmp_triggered = 0;

static void fuzz_error_handler(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
    fuzz_jmp_triggered = 1;
    longjmp(fuzz_jmp_buf, 1);
}

static void fuzz_warning_handler(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Create read structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Set up error handling to allow recovery via longjmp
    if (setjmp(fuzz_jmp_buf) == 0) {
        png_set_error_fn(png_ptr, nullptr, fuzz_error_handler, fuzz_warning_handler);
    } else {
        // Error occurred, cleanup and return
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Initialize dummy file for reading
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    } else {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Reopen for reading
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize CRC action based on input
    int crit_action = (Data[0] % 4); // 0..3
    int ancil_action = (Size > 1) ? (Data[1] % 4) : 0;
    png_set_crc_action(png_ptr, crit_action, ancil_action);

    // Set benign errors behavior
    int allowed = (Size > 2) ? (Data[2] % 2) : 0;
    png_set_benign_errors(png_ptr, allowed);

    // Set RGB to gray fixed conversion
    if (Size > 4) {
        int error_action = Data[3] % 3; // 0..2
        png_fixed_point red = (png_fixed_point)(Data[4] * 100000);
        png_fixed_point green = (Size > 5) ? (png_fixed_point)((Data[5] % 256) * 100000) : 0;
        png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);
    }

    // Set check for invalid index
    if (Size > 6) {
        int index_allowed = (Data[6] % 2) ? 1 : 0;
        png_set_check_for_invalid_index(png_ptr, index_allowed);
    }

    // Attempt to read the PNG file
    png_init_io(png_ptr, fp);
    png_read_info(png_ptr, info_ptr);

    // Trigger png_longjmp manually if desired for testing
    if (Size > 7 && Data[7] > 127) {
        png_longjmp(png_ptr, 1);
    }

    // Cleanup
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}