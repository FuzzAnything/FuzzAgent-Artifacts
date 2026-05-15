// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
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
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
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
#include "png.h"
#include <cstdint>
#include <cstring>
#include <cstdlib>

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    // Initialize png_struct and png_info
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) return 0;
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }
    
    // Use fuzzer input to generate parameters
    png_alloc_size_t alloc_size = 0;
    if (Size >= sizeof(png_alloc_size_t)) {
        memcpy(&alloc_size, Data, sizeof(png_alloc_size_t));
        Data += sizeof(png_alloc_size_t);
        Size -= sizeof(png_alloc_size_t);
    } else {
        alloc_size = Size;
        Size = 0;
    }
    
    // Limit allocation size to prevent excessive memory usage
    if (alloc_size > 65536) alloc_size = 65536;
    
    // Test png_malloc_warn
    void* mem_warn = png_malloc_warn(png_ptr, alloc_size);
    if (mem_warn) {
        png_free(png_ptr, mem_warn);
    }
    
    // Test png_get_chunk_malloc_max
    png_alloc_size_t chunk_max = png_get_chunk_malloc_max(png_ptr);
    
    // Test png_malloc_default (deprecated but still testable)
    void* mem_default = png_malloc_default(png_ptr, alloc_size);
    if (mem_default) {
        png_free(png_ptr, mem_default);
    }
    
    // Test png_set_chunk_malloc_max
    png_alloc_size_t new_max = alloc_size % 1000000; // Limit to reasonable value
    png_set_chunk_malloc_max(png_ptr, new_max);
    
    // Verify the set value
    png_alloc_size_t verify_max = png_get_chunk_malloc_max(png_ptr);
    
    // Test png_calloc
    void* mem_calloc = png_calloc(png_ptr, alloc_size);
    if (mem_calloc) {
        png_free(png_ptr, mem_calloc);
    }
    
    // Test png_malloc with row pointers scenario
    const int width = 64;
    const int height = 64;
    png_bytep row_pointers[height];
    
    for (int y = 0; y < height; y++) {
        row_pointers[y] = static_cast<png_bytep>(png_malloc(png_ptr, width));
        if (row_pointers[y]) {
            // Initialize with some data
            memset(row_pointers[y], y % 256, width);
        }
    }
    
    // Free row pointers properly
    for (int y = 0; y < height; y++) {
        if (row_pointers[y]) {
            png_free(png_ptr, row_pointers[y]);
        }
    }
    
    // Test with NULL png_ptr for functions that handle it
    png_alloc_size_t null_max = png_get_chunk_malloc_max(nullptr);
    
    // Test edge cases with small sizes
    void* small_mem = png_malloc(png_ptr, 1);
    if (small_mem) {
        png_free(png_ptr, small_mem);
    }
    
    void* zero_mem = png_malloc(png_ptr, 0);
    if (zero_mem) {
        png_free(png_ptr, zero_mem);
    }
    
    // Cleanup
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}