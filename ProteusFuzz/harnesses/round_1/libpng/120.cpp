// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_cache_max at pngget.c:1318:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_user_limits at pngset.c:1937:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
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
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Custom memory allocation functions for fuzzing
static void *fuzz_malloc(png_structp png_ptr, png_alloc_size_t size) {
    if (size == 0) return nullptr;
    void *ptr = malloc(size);
    if (!ptr) {
        png_error(png_ptr, "Out of memory");
    }
    return ptr;
}

static void fuzz_free(png_structp png_ptr, png_voidp ptr) {
    if (ptr) {
        free(ptr);
    }
}

// Custom error function for fuzzing
static void fuzz_error(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real scenario, we might log the error, but for fuzzing, we just abort
    abort();
}

// Custom warning function for fuzzing
static void fuzz_warning(png_structp png_ptr, const char *warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Warnings are ignored in fuzzing to avoid noise
}

extern "C" int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Extract parameters from the input data
    png_uint_32 user_width_max = 0;
    png_uint_32 user_height_max = 0;
    png_uint_32 user_chunk_cache_max = 0;
    png_alloc_size_t user_chunk_malloc_max = 0;
    
    if (Size >= 4) {
        user_width_max = *(const png_uint_32 *)(Data);
        if (Size >= 8) {
            user_height_max = *(const png_uint_32 *)(Data + 4);
            if (Size >= 12) {
                user_chunk_cache_max = *(const png_uint_32 *)(Data + 8);
                if (Size >= 16) {
                    user_chunk_malloc_max = *(const png_alloc_size_t *)(Data + 12);
                }
            }
        }
    }

    // Create a dummy file for testing
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Initialize png_struct
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

    // Test png_set_user_limits
    png_set_user_limits(png_ptr, user_width_max, user_height_max);

    // Test png_set_chunk_cache_max
    png_set_chunk_cache_max(png_ptr, user_chunk_cache_max);

    // Test png_set_chunk_malloc_max
    png_set_chunk_malloc_max(png_ptr, user_chunk_malloc_max);

    // Test png_get_chunk_cache_max
    png_uint_32 cache_max = png_get_chunk_cache_max(png_ptr);
    (void)cache_max; // Use the result to avoid unused variable warning

    // Test png_set_mem_fn with custom functions
    png_set_mem_fn(png_ptr, nullptr, fuzz_malloc, fuzz_free);

    // Test with NULL png_ptr (should be safe no-op)
    png_set_user_limits(nullptr, user_width_max, user_height_max);
    png_set_chunk_cache_max(nullptr, user_chunk_cache_max);
    png_set_chunk_malloc_max(nullptr, user_chunk_malloc_max);
    png_uint_32 cache_max_null = png_get_chunk_cache_max(nullptr);
    (void)cache_max_null;
    png_set_mem_fn(nullptr, nullptr, fuzz_malloc, fuzz_free);

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    // Remove the dummy file
    remove(dummy_file);

    return 0;
}