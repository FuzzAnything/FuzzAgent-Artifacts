// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_build_grayscale_palette at png.c:852:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
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
#include <cstring>
#include <cstdlib>

static void fuzz_png_api(const uint8_t *Data, size_t Size) {
    // Allocate and initialize png_struct and png_info
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return;
    }

    // Initialize palette and histogram buffers for testing
    png_color palette[256];
    memset(palette, 0, sizeof(palette));
    png_uint_16 histogram[256];
    memset(histogram, 0, sizeof(histogram));

    // Test 1: png_build_grayscale_palette
    // Try different bit depths (1, 2, 4)
    int bit_depths[] = {1, 2, 4, 8}; // 8 is invalid but tests error handling
    for (int bd : bit_depths) {
        png_build_grayscale_palette(bd, palette);
    }

    // Test 2: png_set_PLTE
    // Set a palette with varying sizes based on input data
    int num_palette = 0;
    if (Size > 0) {
        num_palette = Data[0] % 257; // 0 to 256
    }
    // Ensure palette array is large enough
    if (num_palette > 256) num_palette = 256;
    
    // Fill palette with fuzz data
    for (int i = 0; i < num_palette; ++i) {
        palette[i].red = (Size > i + 1) ? Data[i + 1] : 0;
        palette[i].green = (Size > i + 2) ? Data[i + 2] : 0;
        palette[i].blue = (Size > i + 3) ? Data[i + 3] : 0;
    }
    
    png_set_PLTE(png_ptr, info_ptr, palette, num_palette);

    // Test 3: png_get_PLTE
    png_colorp retrieved_palette = nullptr;
    int retrieved_num_palette = 0;
    png_get_PLTE(png_ptr, info_ptr, &retrieved_palette, &retrieved_num_palette);

    // Test 4: png_get_hIST
    png_uint_16 *hist = nullptr;
    png_get_hIST(png_ptr, info_ptr, &hist);

    // Test 5: png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr, info_ptr);

    // Test 6: png_set_quantize
    int max_colors = 0;
    if (Size > 10) {
        max_colors = Data[10] % 257;
    }
    int full_quantize = (Size > 11) ? (Data[11] % 2) : 0;
    
    // Prepare histogram for quantize
    for (int i = 0; i < num_palette; ++i) {
        histogram[i] = (Size > i + 20) ? ((Data[i + 20] << 8) | (Size > i + 21 ? Data[i + 21] : 0)) : 0;
    }
    
    png_set_quantize(png_ptr, palette, num_palette, max_colors, histogram, full_quantize);

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
}

extern "C" int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    fuzz_png_api(Data, Size);
    return 0;
}