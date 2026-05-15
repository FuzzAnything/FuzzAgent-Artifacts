// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_set_palette_to_rgb at pngrtran.c:945:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_set_check_for_invalid_index at pngset.c:2024:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_IHDR at pngset.c:417:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
// png_set_hIST at pngset.c:375:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <vector>

extern "C" {
#include "code/png.h"
}

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    // Do nothing - dummy function for write operations
}

static void dummy_flush_data(png_structp png_ptr) {
    // Do nothing - dummy function for flush operations
}

extern "C" int LLVMFuzzerTestOneInput_381(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize write structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Set up dummy write functions
    png_set_write_fn(png_ptr, nullptr, dummy_write_data, dummy_flush_data);

    // Parse input data to generate parameters
    size_t offset = 0;
    
    // Extract palette size (1 byte, max 255)
    int num_palette = Data[offset++] % 256;
    if (num_palette == 0) num_palette = 1;
    
    // Extract maximum colors for quantization (1 byte)
    int maximum_colors = Data[offset++] % 256;
    if (maximum_colors == 0) maximum_colors = 1;
    
    // Extract full_quantize flag (1 byte)
    int full_quantize = Data[offset++] % 2;
    
    // Extract allowed flag for invalid index check (1 byte)
    int allowed = Data[offset++] % 2;
    
    // Ensure we have enough data for palette and histogram
    size_t palette_bytes_needed = num_palette * 3; // 3 bytes per color (RGB)
    size_t hist_bytes_needed = num_palette * 2;    // 2 bytes per histogram entry
    
    if (Size - offset < palette_bytes_needed + hist_bytes_needed) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Create palette from input data
    std::vector<png_color> palette(num_palette);
    for (int i = 0; i < num_palette; i++) {
        palette[i].red = Data[offset++];
        palette[i].green = Data[offset++];
        palette[i].blue = Data[offset++];
    }

    // Create histogram from input data
    std::vector<png_uint_16> histogram(num_palette);
    for (int i = 0; i < num_palette; i++) {
        histogram[i] = (Data[offset] << 8) | Data[offset + 1];
        offset += 2;
    }

    // Set image dimensions (required for some operations)
    png_set_IHDR(png_ptr, info_ptr, 100, 100, 8, PNG_COLOR_TYPE_PALETTE,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Test png_set_PLTE
    png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);

    // Test png_set_hIST
    png_set_hIST(png_ptr, info_ptr, histogram.data());

    // Test png_set_quantize
    png_set_quantize(png_ptr, palette.data(), num_palette, maximum_colors, 
                     histogram.data(), full_quantize);

    // Test png_set_palette_to_rgb
    png_set_palette_to_rgb(png_ptr);

    // Test png_get_palette_max
    int max_palette = png_get_palette_max(png_ptr, info_ptr);
    (void)max_palette; // Use result to avoid unused variable warning

    // Test png_set_check_for_invalid_index
    png_set_check_for_invalid_index(png_ptr, allowed);

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}