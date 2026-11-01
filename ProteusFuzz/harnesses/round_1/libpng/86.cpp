// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_PLTE at pngset.c:739:1 in png.h
// png_get_palette_max at pngget.c:1349:1 in png.h
// png_get_PLTE at pngget.c:1121:1 in png.h
// png_get_hIST at pngget.c:911:1 in png.h
// png_set_quantize at pngrtran.c:472:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_build_grayscale_palette at png.c:852:1 in png.h
// png_set_PLTE at pngset.c:739:1 in png.h
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
#include <vector>
#include <cstdio>

// Custom error and warning handlers to prevent aborts during fuzzing
static void
fuzz_png_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not call longjmp or abort, just return to allow fuzzing to continue
    throw std::runtime_error(error_msg);
}

static void
fuzz_png_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Ignore warnings
}

// Helper to safely create and initialize png_struct and png_info
static std::pair<png_structp, png_infop> create_png_context() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_png_error_fn, fuzz_png_warning_fn);
    if (!png_ptr) {
        return {nullptr, nullptr};
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return {nullptr, nullptr};
    }

    return {png_ptr, info_ptr};
}

// Helper to cleanup
static void destroy_png_context(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

// Helper to write dummy data to a file
static bool write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return false;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

extern "C" int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create PNG context
    auto [png_ptr, info_ptr] = create_png_context();
    if (!png_ptr || !info_ptr) return 0;

    // Use try-catch to handle potential exceptions from error handler
    try {
        // 1. png_build_grayscale_palette
        // We need a palette array. Size depends on bit_depth (1, 2, 4).
        // Max entries for 4-bit is 16.
        std::vector<png_color> palette(256, {0, 0, 0});
        
        // Try different bit depths
        int bit_depth = (Data[0] % 4); // 0, 1, 2, 3
        if (bit_depth == 0) bit_depth = 1;
        else if (bit_depth == 1) bit_depth = 2;
        else if (bit_depth == 2) bit_depth = 4;
        else bit_depth = 8; // Invalid for this function, but safe to test

        png_build_grayscale_palette(bit_depth, palette.data());
        
        // 2. png_set_PLTE
        // Set a palette. num_palette derived from input
        int num_palette = (Size > 1) ? (Data[1] % 256) : 10;
        if (num_palette == 0) num_palette = 1;
        
        // Ensure palette size is sufficient
        if (palette.size() < static_cast<size_t>(num_palette)) {
            palette.resize(num_palette, {0,0,0});
        }

        // Test with valid palette
        png_set_PLTE(png_ptr, info_ptr, palette.data(), num_palette);

        // Test with null palette (should trigger error or warning depending on implementation)
        // We catch the exception if the error handler throws
        try {
            png_set_PLTE(png_ptr, info_ptr, nullptr, num_palette);
        } catch (...) {
            // Expected behavior for invalid input
        }

        // 3. png_get_palette_max
        int max_palette = png_get_palette_max(png_ptr, info_ptr);
        (void)max_palette; // Use the value to avoid unused variable warning

        // 4. png_get_PLTE
        png_colorp retrieved_palette = nullptr;
        int retrieved_num_palette = 0;
        png_uint_32 result = png_get_PLTE(png_ptr, info_ptr, &retrieved_palette, &retrieved_num_palette);
        (void)result;

        // 5. png_get_hIST
        // To test this, we might need to simulate having hIST data, but the API just retrieves it.
        // We can call it regardless; it returns 0 if not set.
        png_uint_16 *hist = nullptr;
        png_uint_32 hist_result = png_get_hIST(png_ptr, info_ptr, &hist);
        (void)hist_result;

        // 6. png_set_quantize
        // Requires palette, num_palette, max_colors, histogram, full_quantize
        int max_colors = (Size > 2) ? (Data[2] % 256) : 16;
        if (max_colors == 0) max_colors = 1;
        
        // Create a dummy histogram
        std::vector<png_uint_16> histogram(256, 0);
        for (size_t i = 0; i < histogram.size() && i < Size; ++i) {
            histogram[i] = Data[i] * 256; // Scale up
        }

        int full_quantize = (Size > 3) ? (Data[3] % 2) : 1;

        png_set_quantize(png_ptr, palette.data(), num_palette, max_colors, 
                         histogram.data(), full_quantize);

        // Additional exploration: try with different sizes of input data written to file
        // Although the target functions don't directly read files, libpng context might be used
        // in a broader scenario. Here we just ensure we can write data.
        if (Size > 10) {
            write_dummy_file(Data, Size);
        }

    } catch (const std::exception &) {
        // Catch exceptions from error handler
    } catch (...) {
        // Catch any other unexpected exceptions
    }

    destroy_png_context(png_ptr, info_ptr);
    return 0;
}