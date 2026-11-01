// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_benign_error at pngerror.c:308:1 in png.h
// png_chunk_benign_error at pngerror.c:453:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_chunk_error at pngerror.c:418:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_init_io at png.c:705:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_chunk_warning at pngerror.c:436:1 in png.h
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
#include <setjmp.h>

// Custom error handler to prevent abort during fuzzing
static void custom_error_fn(png_structp png_ptr, const char *error_message) {
    fprintf(stderr, "PNG Error: %s\n", error_message);
    // Do not call abort() here. Let the library handle it via longjmp.
}

// Custom warning handler
static void custom_warning_fn(png_structp png_ptr, const char *warning_message) {
    fprintf(stderr, "PNG Warning: %s\n", warning_message);
}

static void setup_png_struct(png_structpp png_ptr_out, png_infopp info_ptr_out) {
    // Create a png_struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        *png_ptr_out = NULL;
        *info_ptr_out = NULL;
        return;
    }

    // Set custom error and warning handlers
    png_set_error_fn(png_ptr, NULL, custom_error_fn, custom_warning_fn);

    // Create a png_info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        *png_ptr_out = NULL;
        *info_ptr_out = NULL;
        return;
    }

    // Initialize the read process
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        *png_ptr_out = NULL;
        *info_ptr_out = NULL;
        return;
    }

    // Initialize the file read
    FILE *fp = fopen("./dummy_file", "rb");
    if (!fp) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        *png_ptr_out = NULL;
        *info_ptr_out = NULL;
        return;
    }

    png_init_io(png_ptr, fp);
    fclose(fp); // We don't actually read from the file, just initialize the struct

    *png_ptr_out = png_ptr;
    *info_ptr_out = info_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Convert input data to a C string for messages
    char *message = (char *)malloc(Size + 1);
    if (!message) return 0;
    memcpy(message, Data, Size);
    message[Size] = '\0';

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    setup_png_struct(&png_ptr, &info_ptr);

    if (!png_ptr) {
        free(message);
        return 0;
    }

    // Test png_get_error_ptr
    void *error_ptr = png_get_error_ptr(png_ptr);
    (void)error_ptr; // Use the return value to avoid unused warning

    // Test png_warning
    png_warning(png_ptr, message);

    // Test png_chunk_warning
    png_chunk_warning(png_ptr, message);

    // Test png_benign_error
    // This might trigger an error if PNG_FLAG_BENIGN_ERRORS_WARN is not set.
    // We cannot easily set the flag in the struct without internal access,
    // so we let it be. If it aborts, the fuzzer will catch it.
    png_benign_error(png_ptr, message);

    // Test png_chunk_benign_error
    png_chunk_benign_error(png_ptr, message);

    // Test png_chunk_error - this will trigger a longjmp, so we need to handle it carefully.
    // Since it's a fatal error, we expect the program to jump back to the setjmp point.
    // We wrap the call in a setjmp to catch the jump.
    if (setjmp(png_jmpbuf(png_ptr))) {
        // If we jump here, it means png_chunk_error was called and triggered an error.
        // We can clean up and return.
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        free(message);
        return 0;
    }

    // Call png_chunk_error. This will trigger a longjmp if it's a fatal error.
    png_chunk_error(png_ptr, message);

    // If we reach here, it means png_chunk_error did not trigger a longjmp (unexpected).
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    free(message);

    return 0;
}