// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_info_init_3 at png.c:433:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
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

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
    abort();
}

static void dummy_warning_fn(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void *dummy_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (ptr == NULL) {
        png_error(png_ptr, "Out of memory");
    }
    return ptr;
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    if (ptr != NULL) {
        free(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    uint32_t seed = 0;
    if (Size >= 4) {
        seed = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }

    // 1. Initialize png_struct
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        NULL,
        dummy_error_fn,
        dummy_warning_fn,
        NULL,
        dummy_malloc_fn,
        dummy_free_fn
    );

    if (png_ptr == NULL) {
        return 0;
    }

    // 2. Initialize png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // 3. Fuzz png_malloc
    png_alloc_size_t malloc_size = (seed % 10000) + 1;
    void *malloc_result = png_malloc(png_ptr, malloc_size);
    if (malloc_result != NULL) {
        memset(malloc_result, 0xAA, malloc_size);
        png_free(png_ptr, malloc_result);
    }

    // 4. Fuzz png_malloc_warn
    png_alloc_size_t warn_size = (seed % 10000) + 1;
    void *warn_result = png_malloc_warn(png_ptr, warn_size);
    if (warn_result != NULL) {
        memset(warn_result, 0xBB, warn_size);
        png_free(png_ptr, warn_result);
    }

    // 5. Fuzz png_calloc
    png_alloc_size_t calloc_size = (seed % 10000) + 1;
    void *calloc_result = png_calloc(png_ptr, calloc_size);
    if (calloc_result != NULL) {
        memset(calloc_result, 0xCC, calloc_size);
        png_free(png_ptr, calloc_result);
    }

    // 6. Fuzz png_info_init_3 (Deprecated)
    // Since png_info is an opaque type in modern libpng, we cannot use sizeof(png_info).
    // However, the API expects the size of the structure known to the library.
    // We pass the size of the pointer or a fixed constant if the library version allows.
    // In many versions, passing sizeof(png_info) is invalid if the struct is opaque.
    // We will pass 0 or a safe value if the function is deprecated and might be a no-op or handled internally.
    // To strictly follow the API signature without sizeof on opaque type, we rely on the fact that
    // png_info_init_3 is deprecated. We pass a dummy size that might trigger internal checks.
    // Note: In a real scenario, this function is dangerous. Here we just call it to exercise the path.
    // We use a constant that is likely to be the size in older versions or 0 to test error handling.
    png_info_init_3(&info_ptr, 0);

    // 7. Fuzz png_get_rowbytes
    size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    (void)rowbytes;

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}