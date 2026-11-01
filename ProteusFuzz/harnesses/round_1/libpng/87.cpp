// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
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
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

static void *fuzz_malloc(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void fuzz_free(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

static void fuzz_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do not abort in fuzzing, just return
}

static void fuzz_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void fuzz_user_transform(png_structp png_ptr, png_row_infop row_info, png_bytep row) {
    (void)png_ptr;
    (void)row_info;
    (void)row;
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Extract parameters from fuzz input safely
    uint8_t byte0 = Data[0];
    uint8_t byte1 = (Size > 1) ? Data[1] : 0;
    uint8_t byte2 = (Size > 2) ? Data[2] : 0;

    // Custom pointers to store in the struct
    void *error_ptr = (void*)(uintptr_t)byte0;
    void *mem_ptr = (void*)(uintptr_t)(byte1 << 8);
    void *transform_ptr = (void*)(uintptr_t)(byte2 << 16);

    png_structp write_ptr = NULL;
    png_structp read_ptr = NULL;

    // 1. Test png_create_write_struct_2
    write_ptr = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        error_ptr,
        fuzz_error,
        fuzz_warning,
        mem_ptr,
        fuzz_malloc,
        fuzz_free
    );

    if (write_ptr) {
        // Test png_get_error_ptr
        void *got_error_ptr = png_get_error_ptr(write_ptr);
        (void)got_error_ptr;

        // Test png_get_mem_ptr
        void *got_mem_ptr = png_get_mem_ptr(write_ptr);
        (void)got_mem_ptr;

        // Test png_get_user_transform_ptr
        void *got_transform_ptr = png_get_user_transform_ptr(write_ptr);
        (void)got_transform_ptr;

        // Test png_set_mem_fn with new functions
        png_set_mem_fn(write_ptr, (void*)(uintptr_t)0xDEADBEEF, fuzz_malloc, fuzz_free);

        // Cleanup write struct
        png_destroy_write_struct(&write_ptr, NULL);
    }

    // 2. Test png_create_read_struct_2
    read_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        error_ptr,
        fuzz_error,
        fuzz_warning,
        mem_ptr,
        fuzz_malloc,
        fuzz_free
    );

    if (read_ptr) {
        // Test png_get_error_ptr
        void *got_error_ptr = png_get_error_ptr(read_ptr);
        (void)got_error_ptr;

        // Test png_get_mem_ptr
        void *got_mem_ptr = png_get_mem_ptr(read_ptr);
        (void)got_mem_ptr;

        // Test png_get_user_transform_ptr
        void *got_transform_ptr = png_get_user_transform_ptr(read_ptr);
        (void)got_transform_ptr;

        // Test png_set_mem_fn
        png_set_mem_fn(read_ptr, (void*)(uintptr_t)0xCAFEBABE, fuzz_malloc, fuzz_free);

        // Cleanup read struct
        png_destroy_read_struct(&read_ptr, NULL, NULL);
    }

    // Test with NULL pointers (should be safe)
    void *null_error = png_get_error_ptr(NULL);
    (void)null_error;
    void *null_mem = png_get_mem_ptr(NULL);
    (void)null_mem;
    void *null_transform = png_get_user_transform_ptr(NULL);
    (void)null_transform;

    // Test png_set_mem_fn with NULL png_ptr
    png_set_mem_fn(NULL, NULL, NULL, NULL);

    // Test version mismatch (should return NULL)
    png_structp bad_write = png_create_write_struct_2(
        "0.0.0", // Wrong version
        NULL, NULL, NULL, NULL, NULL, NULL
    );
    if (bad_write) {
        png_destroy_write_struct(&bad_write, NULL);
    }

    png_structp bad_read = png_create_read_struct_2(
        "0.0.0", // Wrong version
        NULL, NULL, NULL, NULL, NULL, NULL
    );
    if (bad_read) {
        png_destroy_read_struct(&bad_read, NULL, NULL);
    }

    return 0;
}