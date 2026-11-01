// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
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
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr != NULL) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    } else if (info_ptr != NULL) {
        png_destroy_read_struct(NULL, &info_ptr, NULL);
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    int result = 0;

    // Initialize PNG structure
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        return 0;
    }

    // Use setjmp to handle errors gracefully during fuzzing
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        cleanup_png_struct(png_ptr, info_ptr);
        return 0;
    }

    // Interpret input data to drive fuzzing logic
    png_alloc_size_t size = 0;
    png_alloc_size_t max_chunk = 0;

    if (Size >= sizeof(png_alloc_size_t)) {
        size = *reinterpret_cast<const png_alloc_size_t*>(Data);
        // Cap size to prevent excessive allocation attempts
        if (size > 1024 * 1024) {
            size = 1024 * 1024;
        }
    }

    if (Size >= 2 * sizeof(png_alloc_size_t)) {
        max_chunk = *reinterpret_cast<const png_alloc_size_t*>(Data + sizeof(png_alloc_size_t));
        // Cap max_chunk to reasonable limits
        if (max_chunk > 1024 * 1024 * 1024) {
            max_chunk = 1024 * 1024 * 1024;
        }
    }

    // Test png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, max_chunk);

    // Test png_get_chunk_malloc_max
    png_alloc_size_t retrieved_max = png_get_chunk_malloc_max(png_ptr);
    (void)retrieved_max; // Suppress unused variable warning

    // Test png_calloc
    void *ptr_calloc = png_calloc(png_ptr, size);
    if (ptr_calloc != NULL) {
        memset(ptr_calloc, 0x00, size); // Ensure zeroed
        png_free(png_ptr, ptr_calloc);
    }

    // Test png_malloc_warn
    void *ptr_malloc_warn = png_malloc_warn(png_ptr, size);
    if (ptr_malloc_warn != NULL) {
        png_free(png_ptr, ptr_malloc_warn);
    }

    // Test png_malloc (may trigger error callback on failure)
    void *ptr_malloc = png_malloc(png_ptr, size);
    if (ptr_malloc != NULL) {
        png_free(png_ptr, ptr_malloc);
    }

    // Test png_malloc_default (deprecated, but still testable)
    void *ptr_malloc_default = png_malloc_default(png_ptr, size);
    if (ptr_malloc_default != NULL) {
        png_free(png_ptr, ptr_malloc_default);
    }

    // Test with NULL png_ptr for safety checks
    png_set_chunk_malloc_max(NULL, max_chunk);
    retrieved_max = png_get_chunk_malloc_max(NULL);
    (void)retrieved_max;

    // Cleanup
    cleanup_png_struct(png_ptr, info_ptr);

    return 0;
}