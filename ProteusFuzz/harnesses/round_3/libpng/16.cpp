// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_set_chunk_cache_max at pngset.c:1955:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
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

static void* custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) return nullptr;
    void* ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void custom_free(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    if (ptr) {
        free(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Create a dummy file for potential file-based operations if needed
    const char* dummy_file = "./dummy_file";
    FILE* f = fopen(dummy_file, "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    // Initialize libpng structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use setjmp for error handling
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        return 0;
    }

    // Fuzz png_set_mem_fn
    png_set_mem_fn(png_ptr, nullptr, custom_malloc, custom_free);

    // Fuzz png_set_chunk_cache_max with various values derived from input
    png_uint_32 cache_max = 0;
    if (Size >= 4) {
        cache_max = *reinterpret_cast<const png_uint_32*>(Data);
    } else if (Size > 0) {
        cache_max = Data[0];
    }
    png_set_chunk_cache_max(png_ptr, cache_max);

    // Fuzz png_set_chunk_malloc_max with various values
    png_alloc_size_t malloc_max = 0;
    if (Size >= 8) {
        malloc_max = *reinterpret_cast<const png_alloc_size_t*>(Data + 4);
    } else if (Size > 4) {
        malloc_max = Data[4];
    }
    png_set_chunk_malloc_max(png_ptr, malloc_max);

    // Fuzz png_get_chunk_malloc_max
    png_alloc_size_t retrieved_max = png_get_chunk_malloc_max(png_ptr);
    (void)retrieved_max; // Use the value to prevent unused variable warnings

    // Fuzz png_malloc_warn with various sizes
    png_alloc_size_t alloc_size = 0;
    if (Size >= 12) {
        alloc_size = *reinterpret_cast<const png_alloc_size_t*>(Data + 8);
    } else if (Size > 8) {
        alloc_size = Data[8];
    }
    // Limit allocation size to prevent excessive memory usage during fuzzing
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }
    void* malloc_result = png_malloc_warn(png_ptr, alloc_size);
    if (malloc_result) {
        // Free the allocated memory
        png_free(png_ptr, malloc_result);
    }

    // Fuzz png_malloc_default with various sizes (deprecated but still testable)
    png_alloc_size_t default_alloc_size = 0;
    if (Size >= 16) {
        default_alloc_size = *reinterpret_cast<const png_alloc_size_t*>(Data + 12);
    } else if (Size > 12) {
        default_alloc_size = Data[12];
    }
    if (default_alloc_size > 1024 * 1024) {
        default_alloc_size = 1024 * 1024;
    }
    void* default_malloc_result = png_malloc_default(png_ptr, default_alloc_size);
    if (default_malloc_result) {
        png_free(png_ptr, default_malloc_result);
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    // Remove dummy file
    remove(dummy_file);

    return 0;
}