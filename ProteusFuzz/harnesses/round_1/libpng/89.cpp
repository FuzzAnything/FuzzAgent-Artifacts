// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
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
#include <cstdlib>
#include <cstring>

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    return;
}

static void dummy_warn_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    return;
}

static png_voidp dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Custom pointers derived from fuzz input to vary states
    void *error_ptr = const_cast<uint8_t*>(Data);
    void *mem_ptr = const_cast<uint8_t*>(Data) + 1;
    void *transform_ptr = const_cast<uint8_t*>(Data) + 2;

    // Test 1: png_create_write_struct_2
    png_structp write_png = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        error_ptr,
        dummy_error_fn,
        dummy_warn_fn,
        mem_ptr,
        dummy_malloc_fn,
        dummy_free_fn
    );

    if (write_png != nullptr) {
        // Test 2: png_get_error_ptr
        void *err_ptr = png_get_error_ptr(write_png);
        (void)err_ptr; // Use to suppress unused variable warning

        // Test 3: png_get_mem_ptr
        void *mem_get_ptr = png_get_mem_ptr(write_png);
        (void)mem_get_ptr;

        // Test 4: png_get_user_transform_ptr (initially NULL or set via other means)
        void *user_trans_ptr = png_get_user_transform_ptr(write_png);
        (void)user_trans_ptr;

        // Test 5: png_set_mem_fn
        png_set_mem_fn(write_png, mem_ptr, dummy_malloc_fn, dummy_free_fn);

        // Cleanup write struct
        png_destroy_write_struct(&write_png, nullptr);
    }

    // Test 6: png_create_read_struct_2
    png_structp read_png = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        error_ptr,
        dummy_error_fn,
        dummy_warn_fn,
        mem_ptr,
        dummy_malloc_fn,
        dummy_free_fn
    );

    if (read_png != nullptr) {
        // Test 7: png_get_error_ptr on read struct
        void *read_err_ptr = png_get_error_ptr(read_png);
        (void)read_err_ptr;

        // Test 8: png_get_mem_ptr on read struct
        void *read_mem_ptr = png_get_mem_ptr(read_png);
        (void)read_mem_ptr;

        // Test 9: png_get_user_transform_ptr on read struct
        void *read_user_trans_ptr = png_get_user_transform_ptr(read_png);
        (void)read_user_trans_ptr;

        // Test 10: png_set_mem_fn on read struct
        png_set_mem_fn(read_png, mem_ptr, dummy_malloc_fn, dummy_free_fn);

        // Cleanup read struct
        png_destroy_read_struct(&read_png, nullptr, nullptr);
    }

    return 0;
}