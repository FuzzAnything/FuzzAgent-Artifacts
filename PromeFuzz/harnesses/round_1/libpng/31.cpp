// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_compression_mem_level at pngwrite.c:1196:1 in png.h
// png_set_compression_level at pngwrite.c:1185:1 in png.h
// png_set_text_compression_window_bits at pngwrite.c:1309:1 in png.h
// png_set_text_compression_method at pngwrite.c:1332:1 in png.h
// png_set_compression_method at pngwrite.c:1253:1 in png.h
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

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {}
static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {}

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, dummy_error_fn, dummy_warning_fn);
    return png_ptr;
}

static void cleanup_png_struct(png_structp png_ptr) {
    if (png_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 6) {
        return 0;
    }

    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        return 0;
    }

    // Extract integer parameters from fuzzer input
    int strategy = Data[0] % 256;
    int mem_level = Data[1] % 256;
    int level = Data[2] % 256;
    int window_bits = Data[3] % 256;
    int text_method = Data[4] % 256;
    int comp_method = Data[5] % 256;

    // Call target API functions
    png_set_text_compression_strategy(png_ptr, strategy);
    png_set_compression_mem_level(png_ptr, mem_level);
    png_set_compression_level(png_ptr, level);
    png_set_text_compression_window_bits(png_ptr, window_bits);
    png_set_text_compression_method(png_ptr, text_method);
    png_set_compression_method(png_ptr, comp_method);

    cleanup_png_struct(png_ptr);
    return 0;
}