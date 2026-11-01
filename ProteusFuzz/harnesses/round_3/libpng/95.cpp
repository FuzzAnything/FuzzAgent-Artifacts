// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_hIST at pngget.c:911:1 in png.h
// png_get_tIME at pngget.c:1184:1 in png.h
// png_get_image_width at pngget.c:62:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
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
#include <cstdio>
#include <cstring>

// Custom error handling to prevent libpng from aborting
static void
fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    throw std::runtime_error("libpng error");
}

static void
fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void
fuzz_read_callback(png_structp png_ptr, png_bytep data, png_size_t length) {
    struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } *state = static_cast<decltype(state)>(png_get_io_ptr(png_ptr));
    
    if (state->offset + length > state->size) {
        // Truncate read to avoid out-of-bounds
        length = state->size - state->offset;
    }
    
    if (length > 0) {
        memcpy(data, state->data + state->offset, length);
        state->offset += length;
    }
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a custom state for the read callback
    struct {
        const uint8_t *data;
        size_t size;
        size_t offset;
    } read_state = { Data, Size, 0 };

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_png_error, fuzz_png_warning);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set up error handling with setjmp/longjmp
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Initialize I/O
    png_set_read_fn(png_ptr, &read_state, fuzz_read_callback);

    // Attempt to read the PNG file
    // We try to read the header first to ensure we have basic info
    try {
        png_read_info(png_ptr, info_ptr);
    } catch (...) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Fuzzing targets: Call the specified API functions

    // 1. png_get_gAMA_fixed
    png_fixed_point gamma_value = 0;
    png_uint_32 gama_flag = png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_value);
    (void)gama_flag;
    (void)gamma_value;

    // 2. png_get_x_pixels_per_meter
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);
    (void)x_ppm;

    // 3. png_get_hIST
    png_uint_16 *hist = nullptr;
    png_uint_32 hist_flag = png_get_hIST(png_ptr, info_ptr, &hist);
    if (hist_flag) {
        // hist is valid, we can inspect it if needed, but just using the pointer is enough for coverage
        (void)hist;
    }

    // 4. png_get_tIME
    png_time mod_time;
    png_timep mod_time_ptr = &mod_time;
    png_uint_32 time_flag = png_get_tIME(png_ptr, info_ptr, &mod_time_ptr);
    if (time_flag) {
        (void)mod_time_ptr;
    }

    // 5. png_get_image_width
    png_uint_32 width = png_get_image_width(png_ptr, info_ptr);
    (void)width;

    // 6. png_get_uint_16
    // We need a valid buffer. We can use the input data if size >= 2
    if (Size >= 2) {
        png_uint_16 val = png_get_uint_16(Data);
        (void)val;
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return 0;
}