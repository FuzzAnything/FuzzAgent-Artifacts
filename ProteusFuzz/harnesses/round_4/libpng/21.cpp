// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_build_grayscale_palette at png.c:852:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
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

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void dummy_write_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up error handling to prevent crashes during fuzzing
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    png_set_error_fn(png_ptr, NULL, dummy_error_fn, dummy_warning_fn);

    // Prepare palette buffer for testing
    png_color palette[256];
    memset(palette, 0, sizeof(palette));
    int num_palette = 0;
    png_uint_16 *hist = NULL;

    // Extract bit_depth from input if possible, otherwise default
    int bit_depth = (Data[0] % 5) + 1; // 1 to 5, but we clamp to 1, 2, 4
    if (bit_depth > 4) bit_depth = 4;
    if (bit_depth == 3) bit_depth = 2; // Only 1, 2, 4 are valid for grayscale palette

    // 1. Test png_build_grayscale_palette
    png_build_grayscale_palette(bit_depth, palette);

    // 2. Test png_set_PLTE
    // Calculate number of palette entries based on bit depth
    int expected_entries = 1 << bit_depth;
    // Use input data to vary num_palette slightly to test edge cases
    int test_num_palette = (Size > 0) ? (Data[1] % (expected_entries + 2)) : expected_entries;
    
    // Ensure we don't pass NULL palette with non-zero count
    if (test_num_palette > 0) {
        png_set_PLTE(png_ptr, info_ptr, palette, test_num_palette);
    }

    // 3. Test png_set_quantize
    // Prepare histogram if needed
    png_uint_16 histogram[256];
    memset(histogram, 0, sizeof(histogram));
    // Fill histogram with some fuzzed data
    for (int i = 0; i < 256 && i < Size; i++) {
        histogram[i] = (png_uint_16)(Data[i] * 256);
    }

    int maximum_colors = (Size > 2) ? (Data[2] % 257) : 256;
    int full_quantize = (Size > 3) ? (Data[3] % 2) : 0;

    // Only call if we have a valid palette setup
    if (test_num_palette > 0) {
        png_set_quantize(png_ptr, palette, test_num_palette, maximum_colors, histogram, full_quantize);
    }

    // 4. Test png_get_hIST
    png_uint_16 *hist_out = NULL;
    png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist_out);
    (void)hist_result; // Suppress unused warning
    (void)hist_out;

    // 5. Test png_get_PLTE
    png_color *palette_out = NULL;
    int num_palette_out = 0;
    png_uint_32 plte_result = png_get_PLTE(png_ptr, info_ptr, &palette_out, &num_palette_out);
    (void)plte_result;
    (void)palette_out;
    (void)num_palette_out;

    // 6. Test png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr, info_ptr);
    (void)max_palette;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}