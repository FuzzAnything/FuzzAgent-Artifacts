// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_hIST at pngget.c:911:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_build_grayscale_palette at png.c:852:1 in png.h
// png_build_grayscale_palette at png.c:852:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
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
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
    /* Do nothing to prevent abort during fuzzing */
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    /* Do nothing to prevent stderr output during fuzzing */
}

extern "C" int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    /* Initialize libpng structures */
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    /* Setup longjmp to handle errors gracefully */
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }

    /* Allocate palette and histogram buffers based on input size */
    const size_t max_palette_size = 256;
    std::vector<png_color> palette(max_palette_size);
    std::vector<png_uint_16> histogram(max_palette_size);
    
    /* Initialize buffers with fuzz data if available */
    size_t data_idx = 0;
    for (size_t i = 0; i < max_palette_size && data_idx < Size; ++i) {
        palette[i].red = Data[data_idx++];
        if (data_idx < Size) palette[i].green = Data[data_idx++];
        if (data_idx < Size) palette[i].blue = Data[data_idx++];
    }
    for (size_t i = 0; i < max_palette_size && data_idx < Size; ++i) {
        if (data_idx + 1 <= Size) {
            histogram[i] = (Data[data_idx] << 8) | Data[data_idx + 1];
            data_idx += 2;
        } else {
            histogram[i] = 0;
        }
    }

    /* 1. Fuzz png_build_grayscale_palette */
    int bit_depth = (Size > 0) ? (Data[0] % 5) : 2; /* 1, 2, 3, 4, or 0 */
    png_build_grayscale_palette(bit_depth, palette.data());
    png_build_grayscale_palette(bit_depth, nullptr); /* Test NULL handling */

    /* 2. Fuzz png_set_PLTE */
    int num_palette = (Size > 1) ? (Data[1] % 257) : 16;
    png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);
    png_set_PLTE(png_ptr, info_ptr, nullptr, num_palette); /* Test NULL palette */
    png_set_PLTE(png_ptr, info_ptr, palette.data(), 0);    /* Test zero count */

    /* 3. Fuzz png_set_quantize */
    int maximum_colors = (Size > 2) ? (Data[2] % 257) : 16;
    int full_quantize = (Size > 3) ? (Data[3] % 2) : 0;
    png_set_quantize(png_ptr, palette.data(), num_palette, maximum_colors, histogram.data(), full_quantize);
    png_set_quantize(png_ptr, nullptr, num_palette, maximum_colors, histogram.data(), full_quantize);
    png_set_quantize(png_ptr, palette.data(), num_palette, maximum_colors, nullptr, full_quantize);

    /* 4. Fuzz png_get_palette_max */
    int max_pal = png_get_palette_max(png_ptr, info_ptr);
    (void)max_pal; /* Suppress unused variable warning */

    /* 5. Fuzz png_get_PLTE */
    png_color *pal_out = nullptr;
    int num_pal_out = 0;
    png_uint_32 ret_plte = png_get_PLTE(png_ptr, info_ptr, &pal_out, &num_pal_out);
    (void)ret_plte;
    (void)pal_out;
    (void)num_pal_out;

    /* 6. Fuzz png_get_hIST */
    png_uint_16 *hist_out = nullptr;
    png_uint_32 ret_hist = png_get_hIST(png_ptr, info_ptr, &hist_out);
    (void)ret_hist;
    (void)hist_out;

    /* Cleanup */
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    return 0;
}