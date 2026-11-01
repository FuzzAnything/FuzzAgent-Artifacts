// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_user_chunk_fn at pngset.c:1830:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_get_io_ptr at png.c:688:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_user_chunk_ptr at pngget.c:1272:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_read_user_transform_fn at pngrtran.c:1099:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
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

// Dummy I/O functions
static void dummy_read_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
    (void)png_ptr;
    (void)data;
    (void)length;
}

static void dummy_flush(png_structp png_ptr) {
    (void)png_ptr;
}

static void dummy_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Dummy user transform function
static void dummy_user_transform(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

// Correct signature for user chunk callback:
// int callback(png_structp png_ptr, png_unknown_chunkp chunk)
static int dummy_read_user_chunk(png_structp png_ptr, png_unknown_chunkp chunk) {
    (void)png_ptr;
    (void)chunk;
    return 0; // Discard chunk
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        return 0;
    }

    // Create info struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Setup error handling to prevent aborts during fuzzing
    png_set_error_fn(png_ptr, NULL, dummy_error, dummy_warning);

    // Setup I/O functions
    png_set_read_fn(png_ptr, NULL, dummy_read_data);

    // Set various pointers to test retrieval functions
    void *custom_mem_ptr = (void*)Data;
    void *custom_user_transform_ptr = (void*)(Size > 1 ? Data + 1 : Data);
    void *custom_io_ptr = (void*)(Size > 2 ? Data + 2 : Data);
    void *custom_error_ptr = (void*)(Size > 3 ? Data + 3 : Data);
    void *custom_chunk_ptr = (void*)(Size > 4 ? Data + 4 : Data);

    // Set memory functions (sets the mem_ptr)
    png_set_mem_fn(png_ptr, custom_mem_ptr, NULL, NULL);

    // Set read user transform function (sets the user_transform_ptr)
    // Note: The second argument must be a function pointer, not a void*
    png_set_read_user_transform_fn(png_ptr, dummy_user_transform);

    // Set I/O pointer
    // png_set_io_ptr is not a standard public API in recent libpng versions for setting,
    // but png_get_io_ptr exists. We can simulate setting it by passing it to png_set_read_fn
    // or just rely on the internal state if we can't set it directly.
    // However, to test png_get_io_ptr, we need to set it.
    // In modern libpng, the io_ptr is set via png_set_read_fn or png_set_write_fn.
    // We already called png_set_read_fn with NULL. Let's call it again with custom_io_ptr.
    png_set_read_fn(png_ptr, custom_io_ptr, dummy_read_data);

    // Set error pointer (via error function setter)
    png_set_error_fn(png_ptr, custom_error_ptr, dummy_error, dummy_warning);

    // Set read user chunk function (sets the user_chunk_ptr)
    png_set_read_user_chunk_fn(png_ptr, custom_chunk_ptr, dummy_read_user_chunk);

    // Test png_get_mem_ptr
    void *mem_ptr = png_get_mem_ptr(png_ptr);
    (void)mem_ptr;

    // Test png_get_user_transform_ptr
    // Note: This retrieves the user pointer passed to the transform callback, 
    // which is not directly settable via a separate API in the same way as others.
    // However, the API exists to retrieve it if set.
    void *user_transform_ptr = png_get_user_transform_ptr(png_ptr);
    (void)user_transform_ptr;

    // Test png_get_io_ptr
    void *io_ptr = png_get_io_ptr(png_ptr);
    (void)io_ptr;

    // Test png_get_error_ptr
    void *error_ptr = png_get_error_ptr(png_ptr);
    (void)error_ptr;

    // Test png_get_user_chunk_ptr
    void *user_chunk_ptr = png_get_user_chunk_ptr(png_ptr);
    (void)user_chunk_ptr;

    // Test with NULL pointer (should return NULL safely)
    void *null_mem = png_get_mem_ptr(NULL);
    void *null_transform = png_get_user_transform_ptr(NULL);
    void *null_io = png_get_io_ptr(NULL);
    void *null_error = png_get_error_ptr(NULL);
    void *null_chunk = png_get_user_chunk_ptr(NULL);

    (void)null_mem;
    (void)null_transform;
    (void)null_io;
    (void)null_error;
    (void)null_chunk;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}