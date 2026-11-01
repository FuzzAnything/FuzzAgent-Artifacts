// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
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

static void custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void custom_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static png_voidp custom_malloc_fn(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void custom_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

static void *custom_error_ptr = nullptr;
static void *custom_mem_ptr = nullptr;

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Use fuzz data to derive parameters
    int mode = Data[0] % 3; // 0: read, 1: write, 2: mixed
    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;

    // Prepare custom pointers from fuzz data if size allows
    if (Size >= 16) {
        custom_error_ptr = (void*)(uintptr_t)(Data[1] | (Data[2] << 8));
        custom_mem_ptr = (void*)(uintptr_t)(Data[3] | (Data[4] << 8));
    } else {
        custom_error_ptr = nullptr;
        custom_mem_ptr = nullptr;
    }

    if (mode == 0 || mode == 2) {
        // Test png_create_read_struct_2
        png_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING,
            custom_error_ptr,
            custom_error_fn,
            custom_warning_fn,
            custom_mem_ptr,
            custom_malloc_fn,
            custom_free_fn
        );

        if (png_ptr) {
            // Test png_set_error_fn
            uint8_t val5 = (Size > 5) ? Data[5] : 0;
            png_set_error_fn(png_ptr, (void*)(uintptr_t)(val5 % 256), custom_error_fn, custom_warning_fn);
            
            // Test png_get_error_ptr
            void *err_ptr = png_get_error_ptr(png_ptr);
            (void)err_ptr;

            // Test png_set_mem_fn
            uint8_t val6 = (Size > 6) ? Data[6] : 0;
            png_set_mem_fn(png_ptr, (void*)(uintptr_t)(val6 % 256), custom_malloc_fn, custom_free_fn);

            // Test png_get_user_transform_ptr
            void *user_ptr = png_get_user_transform_ptr(png_ptr);
            (void)user_ptr;

            // Cleanup
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        }
    }

    if (mode == 1 || mode == 2) {
        // Test png_create_write_struct_2
        png_ptr = png_create_write_struct_2(
            PNG_LIBPNG_VER_STRING,
            custom_error_ptr,
            custom_error_fn,
            custom_warning_fn,
            custom_mem_ptr,
            custom_malloc_fn,
            custom_free_fn
        );

        if (png_ptr) {
            // Test png_set_error_fn
            uint8_t val7 = (Size > 7) ? Data[7] : 0;
            png_set_error_fn(png_ptr, (void*)(uintptr_t)(val7 % 256), custom_error_fn, custom_warning_fn);
            
            // Test png_get_error_ptr
            void *err_ptr = png_get_error_ptr(png_ptr);
            (void)err_ptr;

            // Test png_set_mem_fn
            uint8_t val8 = (Size > 8) ? Data[8] : 0;
            png_set_mem_fn(png_ptr, (void*)(uintptr_t)(val8 % 256), custom_malloc_fn, custom_free_fn);

            // Test png_get_user_transform_ptr
            void *user_ptr = png_get_user_transform_ptr(png_ptr);
            (void)user_ptr;

            // Cleanup
            png_destroy_write_struct(&png_ptr, &info_ptr);
        }
    }

    return 0;
}