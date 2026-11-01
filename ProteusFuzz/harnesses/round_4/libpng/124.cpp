// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
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
#include <cstdint>
#include <cstring>
#include <cstdio>

static void *custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void custom_free(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from input data to diversify fuzzing
    png_uint_32 width_max = Data[0] | (Data[1] << 8);
    png_uint_32 height_max = Data[2] | (Data[3] << 8);
    png_uint_32 chunk_cache_max = Data[4 % Size] | (Data[(5 + (Size % 10)) % Size] << 8);
    png_alloc_size_t chunk_malloc_max = Data[6 % Size] | (Data[(7 + (Size % 10)) % Size] << 8);

    // Initialize read structure
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL,
        NULL,
        custom_malloc,
        custom_free
    );

    if (!png_ptr) {
        return 0;
    }

    // Test png_set_mem_fn with custom handlers (redundant but tests the API)
    png_set_mem_fn(png_ptr, NULL, custom_malloc, custom_free);

    // Test png_set_user_limits
    png_set_user_limits(png_ptr, width_max, height_max);

    // Test png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, chunk_cache_max);

    // Test png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, chunk_malloc_max);

    // Test png_get_chunk_cache_max
    png_uint_32 current_cache_max = png_get_chunk_cache_max(png_ptr);
    (void)current_cache_max; // Use to avoid unused variable warning

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    return 0;
}