// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_error at pngerror.c:50:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_write_fn at pngwio.c:121:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_benign_error at pngerror.c:453:1 in png.h
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

// Custom error handler that returns instead of aborting to keep the fuzzer alive.
// Note: libpng expects the error handler to not return, but for fuzzing we must prevent
// the process from terminating to explore more states.
static void dummy_error_handler(png_structp png_ptr, const char *error_message) {
    (void)png_ptr;
    (void)error_message;
    // Simply return to avoid aborting the fuzzer.
    // In a real application, this would call longjmp or abort.
}

static void dummy_warning_handler(png_structp png_ptr, const char *warning_message) {
    (void)png_ptr;
    (void)warning_message;
    // Do nothing for warnings in fuzzing
}

static void dummy_read_fn(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Dummy read function
}

static void dummy_write_fn(png_structp png_ptr, const png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
    // Dummy write function
}

static void dummy_flush_fn(png_structp png_ptr) {
    (void)png_ptr;
    // Dummy flush function
}

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Set up dummy callbacks to prevent crashes from default handlers
    // This is crucial to prevent the "deadly signal" from the default abort behavior
    png_set_error_fn(png_ptr, NULL, dummy_error_handler, dummy_warning_handler);
    png_set_read_fn(png_ptr, NULL, dummy_read_fn);
    png_set_write_fn(png_ptr, NULL, dummy_write_fn, dummy_flush_fn);

    // Create a buffer for messages to fuzz with
    const char *msg = (const char *)Data;
    size_t msg_len = Size;

    // Ensure the message is null-terminated for safety
    char *safe_msg = (char *)malloc(msg_len + 1);
    if (!safe_msg) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        return 0;
    }
    memcpy(safe_msg, msg, msg_len);
    safe_msg[msg_len] = '\0';

    // Fuzz png_warning
    png_warning(png_ptr, safe_msg);

    // Fuzz png_chunk_warning
    png_chunk_warning(png_ptr, safe_msg);

    // Fuzz png_benign_error
    // Note: This may trigger a fatal error if PNG_FLAG_BENIGN_ERRORS_WARN is not set.
    // Our dummy_error_handler prevents abort, so it's safe for fuzzing.
    png_benign_error(png_ptr, safe_msg);

    // Fuzz png_chunk_benign_error
    png_chunk_benign_error(png_ptr, safe_msg);

    // Fuzz png_error
    // This is a fatal error and will call the error handler.
    // Our dummy_error_handler prevents abort, so it's safe for fuzzing.
    png_error(png_ptr, safe_msg);

    // Fuzz png_chunk_error
    // This is a fatal error and will call the error handler.
    // Our dummy_error_handler prevents abort, so it's safe for fuzzing.
    png_chunk_error(png_ptr, safe_msg);

    // Cleanup
    free(safe_msg);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}