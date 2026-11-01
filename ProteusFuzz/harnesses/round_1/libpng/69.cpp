// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_set_compression_buffer_size at pngset.c:1865:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_write_flush at pngwrite.c:978:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_init_io at png.c:705:1 in png.h
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
#include <cstring>

static void custom_error_fn(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Custom error handler: do nothing to avoid aborting in fuzzing context
}

static void custom_warning_fn(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
    // Custom warning handler: do nothing
}

static png_voidp custom_malloc_fn(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    png_voidp ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void custom_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    if (ptr) {
        free(ptr);
    }
}

static void write_data_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Dummy write function, discards data
}

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    int result = 0;

    // Use first byte to decide version string (simulate mismatch or match)
    const char *user_png_ver = (Data[0] == 0) ? "1.6.0" : "0.0.0";

    // Create write struct with custom handlers
    png_ptr = png_create_write_struct_2(
        user_png_ver,
        nullptr,
        custom_error_fn,
        custom_warning_fn,
        nullptr,
        custom_malloc_fn,
        custom_free_fn
    );

    if (!png_ptr) {
        return 0;
    }

    // Create info struct
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Set up error handling with setjmp to catch png_error
    if (setjmp(png_jmpbuf(png_ptr))) {
        // Error occurred, clean up and return
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 0;
    }

    // Initialize dummy file for writing
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        png_init_io(png_ptr, fp);
        fclose(fp);
    }

    // Initialize write functions
    png_set_write_fn(png_ptr, nullptr, write_data_fn, nullptr);

    // Use second byte to determine buffer size for png_set_compression_buffer_size
    size_t buffer_size = (size_t)Data[1];
    if (buffer_size == 0) {
        buffer_size = 1024; // Default to 1KB if 0
    }
    if (buffer_size > 2000000000ULL) {
        buffer_size = 2000000000ULL; // Cap at 2GB
    }

    // Set compression buffer size
    png_set_compression_buffer_size(png_ptr, buffer_size);

    // Trigger warning
    png_warning(png_ptr, "Fuzzing warning message");

    // Attempt to flush (safe even if nothing written)
    png_write_flush(png_ptr);

    // Note: png_process_data_pause is only callable within png_process_data,
    // which is part of the progressive API. Since we are using the write API,
    // we cannot safely call it here without a full progressive setup.
    // Skipping to avoid undefined behavior.

    // Trigger error (will be caught by setjmp)
    // Uncommenting the line below will cause the function to jump to the setjmp handler
    // png_error(png_ptr, "Fuzzing error message");

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}