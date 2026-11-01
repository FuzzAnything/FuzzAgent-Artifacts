// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_write_user_transform_fn at pngwrite.c:1360:1 in png.h
// png_set_text_compression_strategy at pngwrite.c:1295:1 in png.h
// png_set_flush at pngwrite.c:966:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_compression_buffer_size at pngget.c:1279:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
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
#include <cstddef>
#include <cstdio>
#include <cstdlib>

// Static helper to create a valid png_struct for writing
static png_structp create_png_struct_write() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return NULL;
    }
    return png_ptr;
}

// Static helper to create a valid png_struct for reading
static png_structp create_png_struct_read() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return NULL;
    }
    return png_ptr;
}

// Static user transform callback for testing
static void dummy_user_transform(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
    // Empty implementation to satisfy the callback requirement
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    // Use the first byte to decide which mode to test (read vs write)
    bool test_write_mode = (Data[0] % 2 == 0);
    png_structp png_ptr = NULL;

    if (test_write_mode) {
        png_ptr = create_png_struct_write();
    } else {
        png_ptr = create_png_struct_read();
    }

    if (png_ptr == NULL) {
        return 0;
    }

    // Initialize a dummy info struct if needed (some functions might implicitly rely on it, though not strictly for these APIs)
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_write_struct(&png_ptr, NULL);
        return 0;
    }

    // Use setjmp for error handling as per libpng conventions
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Extract parameters from fuzz input safely
    size_t buffer_size = 0;
    int strategy = 0;
    int nrows = 0;

    // Ensure we have enough data before accessing indices
    if (Size >= 3) {
        buffer_size = (static_cast<size_t>(Data[1]) << 8) | static_cast<size_t>(Data[2]);
    }
    if (Size >= 4) {
        strategy = static_cast<int>(Data[3]);
    }
    if (Size >= 5) {
        nrows = static_cast<int>(Data[4]);
    }

    // 1. Test png_get_compression_buffer_size
    // This is safe to call on any valid png_ptr
    size_t current_size = png_get_compression_buffer_size(png_ptr);
    (void)current_size; // Use the value to prevent unused variable warnings

    // 2. Test png_set_compression_buffer_size
    // This function may trigger an error if size is 0 or > 2GB, or if stream is active.
    // We wrap in setjmp to catch potential longjmps.
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_set_compression_buffer_size(png_ptr, buffer_size);
    } else {
        // Error occurred, destroy and return
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // 3. Test png_write_flush
    // This is primarily for write mode. In read mode, it might do nothing or error depending on state.
    // We wrap in setjmp just in case.
    if (setjmp(png_jmpbuf(png_ptr)) == 0) {
        png_write_flush(png_ptr);
    } else {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // 4. Test png_set_write_user_transform_fn
    // This sets a callback. Safe to call multiple times.
    png_set_write_user_transform_fn(png_ptr, dummy_user_transform);

    // 5. Test png_set_text_compression_strategy
    // Sets strategy for text chunks.
    png_set_text_compression_strategy(png_ptr, strategy);

    // 6. Test png_set_flush
    // Sets flush interval.
    png_set_flush(png_ptr, nrows);

    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}