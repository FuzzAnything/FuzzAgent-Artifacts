// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_benign_error at pngerror.c:453:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <vector>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_message) {
    // Do nothing to prevent abort during fuzzing
    (void)png_ptr;
    (void)error_message;
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_message) {
    // Do nothing to prevent output during fuzzing
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file to satisfy file-based operations if needed
    std::ofstream dummy_file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (dummy_file.is_open()) {
        dummy_file.write(reinterpret_cast<const char*>(Data), Size);
        dummy_file.close();
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Set custom error and warning handlers to prevent aborts
    png_set_error_fn(png_ptr, nullptr, dummy_error_fn, dummy_warning_fn);

    // Fuzz png_warning
    png_warning(png_ptr, "Test warning message");

    // Fuzz png_chunk_warning
    png_chunk_warning(png_ptr, "Test chunk warning message");

    // Fuzz png_benign_error
    png_benign_error(png_ptr, "Test benign error message");

    // Fuzz png_chunk_benign_error
    png_chunk_benign_error(png_ptr, "Test chunk benign error message");

    // Fuzz png_error (will call dummy_error_fn and not abort)
    png_error(png_ptr, "Test fatal error message");

    // Fuzz png_chunk_error (will call dummy_error_fn and not abort)
    png_chunk_error(png_ptr, "Test chunk fatal error message");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}