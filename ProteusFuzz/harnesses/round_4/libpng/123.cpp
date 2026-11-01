// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_error at pngerror.c:50:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

static void *fuzz_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) return nullptr;
    void *ptr = malloc(size);
    if (!ptr) {
        png_error(png_ptr, "Out of memory");
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
    // In a real scenario, log the error. For fuzzing, just return to avoid crash.
    longjmp(png_jmpbuf(png_ptr), 1);
}

static void fuzz_warn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Ignore warnings for fuzzing
}

extern "C" int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from fuzz data
    png_uint_32 cache_max = (png_uint_32)Data[0];
    png_alloc_size_t malloc_max = (png_alloc_size_t)(Data[1] * 1024);
    png_uint_32 width_max = (png_uint_32)(Data[0] + 1);
    png_uint_32 height_max = (png_uint_32)(Data[1] + 1);

    // Create read structure
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        fuzz_error,
        fuzz_warn,
        nullptr,
        fuzz_malloc,
        fuzz_free
    );

    if (!png_ptr) {
        return 0;
    }

    // Set custom memory functions (optional, but tests the API)
    png_set_mem_fn(png_ptr, nullptr, fuzz_malloc, fuzz_free);

    // Set chunk cache max
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Get chunk cache max
    png_uint_32 current_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)current_cache_max; // Use the value to avoid unused variable warning

    // Set chunk malloc max
    png_set_chunk_malloc_max(png_ptr, malloc_max);

    // Set user limits
    png_set_user_limits(png_ptr, width_max, height_max);

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}