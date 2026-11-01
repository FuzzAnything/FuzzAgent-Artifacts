// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_cLLI_fixed at pngget.c:788:1 in png.h
// png_get_x_pixels_per_meter at pngget.c:125:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_read_info at pngread.c:102:1 in png.h
// png_get_sCAL_fixed at pngget.c:1024:1 in png.h
// png_set_cLLI_fixed at pngset.c:162:1 in png.h
// png_get_gAMA_fixed at pngget.c:658:1 in png.h
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

// Define a simple struct for IO state
struct FuzzIOState {
    const uint8_t *data;
    size_t size;
    size_t pos;
};

static void static_fuzzer_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    } else if (info_ptr) {
        png_destroy_info_struct(nullptr, &info_ptr);
    }
}

// Custom read callback for libpng
static void static_read_data(png_structp png_ptr, png_bytep out_bytes, png_size_t byte_count_to_read) {
    FuzzIOState *io_data = (FuzzIOState *)png_get_io_ptr(png_ptr);
    if (!io_data) {
        png_error(png_ptr, "IO pointer is null");
        return;
    }

    if (io_data->pos + byte_count_to_read > io_data->size) {
        png_error(png_ptr, "Read beyond buffer");
    }
    memcpy(out_bytes, io_data->data + io_data->pos, byte_count_to_read);
    io_data->pos += byte_count_to_read;
}

extern "C" int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file as requested
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        fwrite(Data, 1, Size, dummy_file);
        fclose(dummy_file);
    }

    // Initialize PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Setup error handling with setjmp
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        static_fuzzer_cleanup(png_ptr, info_ptr);
        return 0;
    }

    // Prepare IO state
    FuzzIOState io_state;
    io_state.data = Data;
    io_state.size = Size;
    io_state.pos = 0;

    // Set read function
    png_set_read_fn(png_ptr, &io_state, static_read_data);

    // Read the info header
    png_read_info(png_ptr, info_ptr);

    // --- Fuzz Target 1: png_get_sCAL_fixed ---
    int unit = 0;
    png_fixed_point width = 0;
    png_fixed_point height = 0;
    png_get_sCAL_fixed(png_ptr, info_ptr, &unit, &width, &height);

    // --- Fuzz Target 2: png_set_cLLI_fixed ---
    // Values are in nits * 10,000. Max is 200,000 (approx 20 nits) for safety in fuzzing.
    // The API takes two arguments: max_content_light_level and max_frame_average_light_level.
    png_uint_32 clli_val = (png_uint_32)(Size % 200000); 
    png_set_cLLI_fixed(png_ptr, info_ptr, clli_val, clli_val);

    // --- Fuzz Target 3: png_get_gAMA_fixed ---
    png_fixed_point gamma_val = 0;
    png_get_gAMA_fixed(png_ptr, info_ptr, &gamma_val);

    // --- Fuzz Target 4: png_get_cLLI_fixed ---
    // Note: This function returns png_uint_32 and takes png_uint_32 pointers, not png_fixed_point.
    png_uint_32 max_cll = 0;
    png_uint_32 max_fall = 0;
    png_get_cLLI_fixed(png_ptr, info_ptr, &max_cll, &max_fall);

    // --- Fuzz Target 5: png_get_x_pixels_per_meter ---
    png_uint_32 x_ppm = png_get_x_pixels_per_meter(png_ptr, info_ptr);

    // Cleanup
    static_fuzzer_cleanup(png_ptr, info_ptr);

    return 0;
}