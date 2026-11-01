// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
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

static void *fuzz_malloc(png_structp png_ptr, png_alloc_size_t size) {
    if (size == 0) return nullptr;
    void *ptr = malloc(size);
    if (!ptr && size != 0) {
        png_error(png_ptr, "Out of memory");
    }
    return ptr;
}

static void fuzz_free(png_structp png_ptr, png_voidp ptr) {
    if (ptr) {
        free(ptr);
    }
}

static void fuzz_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    abort();
}

static void fuzz_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static uint32_t read_uint32_safe(const uint8_t *Data, size_t Size, size_t offset) {
    if (offset + 4 > Size) {
        return 0;
    }
    return *reinterpret_cast<const uint32_t*>(Data + offset);
}

extern "C" int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from input data safely
    uint32_t cache_max = read_uint32_safe(Data, Size, 0);
    uint32_t malloc_max = read_uint32_safe(Data, Size, 4);
    uint32_t width_max = read_uint32_safe(Data, Size, 8);
    uint32_t height_max = read_uint32_safe(Data, Size, 12);

    // Create read structure with custom functions
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        fuzz_error,
        fuzz_warning,
        nullptr,
        fuzz_malloc,
        fuzz_free
    );

    if (!png_ptr) {
        return 0;
    }

    // Test png_set_mem_fn (redundant but safe to call again)
    png_set_mem_fn(png_ptr, nullptr, fuzz_malloc, fuzz_free);

    // Test png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Test png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, malloc_max);

    // Test png_set_user_limits
    png_set_user_limits(png_ptr, width_max, height_max);

    // Test png_get_chunk_cache_max
    png_uint_32 retrieved_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)retrieved_cache_max; // Use the value to avoid unused warning

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}