// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_set_benign_errors at pngset.c:1994:1 in png.h
// png_set_rgb_to_gray_fixed at pngrtran.c:1013:1 in png.h
// png_set_crc_action at pngrtran.c:24:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_set_interlace_handling at pngtrans.c:98:1 in png.h
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

static void write_dummy_png(const uint8_t *Data, size_t Size) {
    if (Data == nullptr || Size == 0) return;
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Data == nullptr || Size == 0) {
        return 0;
    }

    // Ensure we have enough data for all the indices we plan to access
    if (Size < 8) {
        return 0;
    }

    // Create a dummy file if data is provided
    write_dummy_png(Data, Size);

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

    // Use setjmp/longjmp for error handling as required by libpng
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    // Open the dummy file
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_init_io(png_ptr, fp);

    // Fuzz png_set_benign_errors
    int benign_allowed = (Data[0] % 2);
    png_set_benign_errors(png_ptr, benign_allowed);

    // Fuzz png_set_rgb_to_gray_fixed
    int error_action = (Data[1] % 3); // 0: NONE, 1: WARN, 2: ERROR
    png_fixed_point red = (png_fixed_point)((Data[2] * 100000) / 255);
    png_fixed_point green = (png_fixed_point)((Data[3] * 100000) / 255);
    png_set_rgb_to_gray_fixed(png_ptr, error_action, red, green);

    // Fuzz png_set_crc_action
    int crit_action = (Data[4] % 3); // 0: WARN/USE, 1: QUIET/USE, 2: ERROR/QUIT
    int ancil_action = (Data[5] % 3);
    png_set_crc_action(png_ptr, crit_action, ancil_action);

    // Fuzz png_set_check_for_invalid_index
    int invalid_index_allowed = (Data[6] % 2);
    png_set_check_for_invalid_index(png_ptr, invalid_index_allowed);

    // Fuzz png_set_chunk_cache_max
    png_uint_32 cache_max = (png_uint_32)(Data[7] * 100 + 1);
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Attempt to read info (may fail due to invalid data, which is expected in fuzzing)
    png_read_info(png_ptr, info_ptr);

    // Fuzz png_set_interlace_handling
    int passes = png_set_interlace_handling(png_ptr);
    (void)passes; // Suppress unused variable warning

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    return 0;
}