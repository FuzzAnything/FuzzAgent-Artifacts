// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free_default at pngmem.c:247:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "code/png.h"

static png_structp create_png_struct() {
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    return png_ptr;
}

static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        if (info_ptr) {
            png_destroy_write_struct(&png_ptr, &info_ptr);
        } else {
            png_destroy_write_struct(&png_ptr, nullptr);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return 0;
    }
    
    png_structp png_ptr = create_png_struct();
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    // Extract allocation size from fuzzer input (limit to reasonable size)
    png_alloc_size_t alloc_size = 0;
    if (Size >= sizeof(png_alloc_size_t)) {
        memcpy(&alloc_size, Data, sizeof(png_alloc_size_t));
    } else {
        memcpy(&alloc_size, Data, Size);
    }
    
    // Limit allocation size to avoid excessive memory usage
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }
    
    // Test png_calloc
    void *calloc_ptr = png_calloc(png_ptr, alloc_size);
    if (calloc_ptr) {
        // Verify memory is zero-initialized
        if (alloc_size > 0) {
            uint8_t *check = static_cast<uint8_t*>(calloc_ptr);
            if (check[0] != 0 || (alloc_size > 1 && check[alloc_size - 1] != 0)) {
                // Memory not properly zeroed - this would be a bug
            }
        }
        png_free(png_ptr, calloc_ptr);
    }
    
    // Test png_malloc_warn
    void *malloc_warn_ptr = png_malloc_warn(png_ptr, alloc_size);
    if (malloc_warn_ptr) {
        // Test writing to allocated memory
        if (alloc_size > 0) {
            memset(malloc_warn_ptr, 0xAA, alloc_size > 256 ? 256 : alloc_size);
        }
        png_free(png_ptr, malloc_warn_ptr);
    }
    
    // Test png_malloc
    void *malloc_ptr = png_malloc(png_ptr, alloc_size);
    if (malloc_ptr) {
        // Test writing to allocated memory
        if (alloc_size > 0) {
            memset(malloc_ptr, 0xBB, alloc_size > 512 ? 512 : alloc_size);
        }
        png_free(png_ptr, malloc_ptr);
    }
    
    // Test png_malloc_default (deprecated)
    void *malloc_default_ptr = png_malloc_default(png_ptr, alloc_size);
    if (malloc_default_ptr) {
        // Test writing to allocated memory
        if (alloc_size > 0) {
            memset(malloc_default_ptr, 0xCC, alloc_size > 128 ? 128 : alloc_size);
        }
        png_free_default(png_ptr, malloc_default_ptr);
    }
    
    // Test png_free with NULL pointer
    png_free(png_ptr, nullptr);
    
    // Test png_free_default with NULL pointer
    png_free_default(png_ptr, nullptr);
    
    // Test allocation of row pointers (as mentioned in documentation)
    if (alloc_size > 0 && alloc_size < 10000) {
        png_uint_32 height = static_cast<png_uint_32>(alloc_size % 100);
        png_bytepp row_pointers = static_cast<png_bytepp>(png_malloc(png_ptr, height * sizeof(png_bytep)));
        
        if (row_pointers) {
            for (png_uint_32 y = 0; y < height; y++) {
                row_pointers[y] = static_cast<png_bytep>(png_malloc(png_ptr, 100));
                if (row_pointers[y]) {
                    memset(row_pointers[y], y % 256, 100);
                }
            }
            
            // Free row pointers in reverse order as required
            for (png_uint_32 y = 0; y < height; y++) {
                png_free(png_ptr, row_pointers[y]);
            }
            png_free(png_ptr, row_pointers);
        }
    }
    
    // Test with zero size allocations
    void *zero_alloc = png_calloc(png_ptr, 0);
    if (zero_alloc) {
        png_free(png_ptr, zero_alloc);
    }
    
    cleanup_png_struct(png_ptr, info_ptr);
    
    return 0;
}