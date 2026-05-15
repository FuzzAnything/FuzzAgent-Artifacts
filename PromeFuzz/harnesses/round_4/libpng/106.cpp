// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
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
#include "png.h"
#include <cstdint>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Create dummy png_struct for testing
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Create dummy png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use fuzzer input to determine allocation sizes
    png_alloc_size_t alloc_size = 0;
    if (Size >= sizeof(png_alloc_size_t)) {
        memcpy(&alloc_size, Data, sizeof(png_alloc_size_t));
        // Limit size to avoid excessive memory allocation
        alloc_size = alloc_size % 65536;
    }

    // Test png_malloc_warn
    void* mem_warn = png_malloc_warn(png_ptr, alloc_size);
    if (mem_warn) {
        png_free(png_ptr, mem_warn);
    }

    // Test png_get_chunk_malloc_max
    png_alloc_size_t current_max = png_get_chunk_malloc_max(png_ptr);

    // Test png_malloc_default (deprecated but still testable)
    void* mem_default = png_malloc_default(png_ptr, alloc_size);
    if (mem_default) {
        png_free(png_ptr, mem_default);
    }

    // Test png_set_chunk_malloc_max
    png_alloc_size_t new_max = (alloc_size % 1000000);
    png_set_chunk_malloc_max(png_ptr, new_max);

    // Verify the set value
    png_alloc_size_t updated_max = png_get_chunk_malloc_max(png_ptr);

    // Test png_calloc
    void* mem_calloc = png_calloc(png_ptr, alloc_size);
    if (mem_calloc) {
        png_free(png_ptr, mem_calloc);
    }

    // Test png_malloc with cleanup
    void* mem_alloc = png_malloc(png_ptr, alloc_size);
    if (mem_alloc) {
        png_free(png_ptr, mem_alloc);
    }

    // Test with row_pointers allocation pattern
    if (alloc_size > 0 && alloc_size < 1000) {
        png_uint_32 height = static_cast<png_uint_32>(alloc_size % 100);
        png_bytepp row_pointers = static_cast<png_bytepp>(png_malloc(png_ptr, height * sizeof(png_bytep)));
        
        if (row_pointers) {
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = static_cast<png_bytep>(png_malloc(png_ptr, 100));
            }
            
            // Cleanup row_pointers
            for (png_uint_32 y = 0; y < height; y++) {
                if (row_pointers[y]) {
                    png_free(png_ptr, row_pointers[y]);
                }
            }
            png_free(png_ptr, row_pointers);
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}