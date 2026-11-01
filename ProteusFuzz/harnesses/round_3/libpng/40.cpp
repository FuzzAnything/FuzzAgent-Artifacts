// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_longjmp at pngerror.c:663:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_get_rgb_to_gray_status at pngget.c:1264:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
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
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// Custom error handler to prevent abort during fuzzing
// Note: The signature must match png_error_ptr: void (*)(png_structp, png_const_charp)
// png_structp is a pointer to png_struct, not const.
static void
fuzz_png_error_handler(png_structp png_ptr, png_const_charp error_message) {
    (void)error_message;
    // Trigger longjmp to recover from error
    png_longjmp(png_ptr, 1);
}

// Custom warning handler
static void
fuzz_png_warning_handler(png_structp png_ptr, png_const_charp warning_message) {
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 7) {
        return 0;
    }

    // Create a dummy file to simulate reading a PNG
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set custom error and warning handlers to prevent abort
    // The function pointers must match the expected signatures exactly
    png_set_error_fn(png_ptr, NULL, fuzz_png_error_handler, fuzz_png_warning_handler);

    // Use setjmp to catch errors
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred, clean up and return
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Initialize I/O
    fp = fopen(dummy_file, "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    png_init_io(png_ptr, fp);

    // Extract data for fuzzing parameters
    uint8_t error_action = Data[0] % 3; // 0: NONE, 1: WARN, 2: ERROR
    png_fixed_point red = (png_fixed_point)(Data[1] * 1000); // Scale to fixed point
    png_fixed_point green = (png_fixed_point)(Data[2] * 1000);
    int allowed_index = (Data[3] % 2) ? 1 : 0; // 1: allow invalid, 0: strict
    int benign_errors = (Data[4] % 2) ? 1 : 0; // 1: warn, 0: error
    int crit_action = (Data[5] % 3); // 0: error, 1: warn, 2: use
    int ancil_action = (Data[6] % 3); // 0: error, 1: warn, 2: use

    // Call target API functions
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);
    png_set_check_for_invalid_index(png_ptr, allowed_index);
    png_set_benign_errors(png_ptr, benign_errors);
    png_set_crc_action(png_ptr, crit_action, ancil_action);

    // Check status
    png_byte status = png_get_rgb_to_gray_status(png_ptr);
    (void)status; // Use status to avoid unused variable warning

    // Attempt to read the image header
    png_read_info(png_ptr, info_ptr);

    // Clean up
    fclose(fp);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}