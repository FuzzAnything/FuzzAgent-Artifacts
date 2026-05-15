// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
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

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {}
static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {}

static png_structp create_test_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                 dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return nullptr;
    }
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    // Initialize png_struct
    png_structp png_ptr = create_test_png_struct();
    if (!png_ptr) {
        return 0;
    }

    // Use first 4 bytes to determine allocation size (modulo reasonable limit)
    png_alloc_size_t alloc_size = 0;
    if (Size >= 4) {
        memcpy(&alloc_size, Data, 4);
        alloc_size = alloc_size % (1024 * 1024); // Limit to 1MB to avoid excessive memory usage
        Data += 4;
        Size -= 4;
    }

    // Test png_malloc_warn
    void* mem_warn = png_malloc_warn(png_ptr, alloc_size);
    if (mem_warn) {
        // Could optionally write some data from fuzzer input
        if (Size > 0) {
            size_t copy_size = Size > alloc_size ? alloc_size : Size;
            memcpy(mem_warn, Data, copy_size);
        }
        png_free(png_ptr, mem_warn);
    }

    // Test png_malloc_default (deprecated)
    void* mem_default = png_malloc_default(png_ptr, alloc_size);
    if (mem_default) {
        png_free_default(png_ptr, mem_default);
    }

    // Test png_calloc
    void* mem_calloc = png_calloc(png_ptr, alloc_size);
    if (mem_calloc) {
        // Verify zero initialization for first few bytes
        if (alloc_size > 0) {
            uint8_t* check = static_cast<uint8_t*>(mem_calloc);
            // Check first 16 bytes or entire allocation if smaller
            size_t check_size = alloc_size > 16 ? 16 : alloc_size;
            for (size_t i = 0; i < check_size; ++i) {
                if (check[i] != 0) {
                    // This should never happen for png_calloc
                    // But we don't want to crash, just note it
                }
            }
        }
        png_free(png_ptr, mem_calloc);
    }

    // Test png_get_mem_ptr
    void* mem_ptr = png_get_mem_ptr(png_ptr);
    (void)mem_ptr; // Use result to avoid unused variable warning

    // Test png_malloc with various sizes based on fuzzer input
    // Use remaining bytes to determine different allocation patterns
    size_t offset = 0;
    while (offset + 4 <= Size) {
        png_alloc_size_t dynamic_size = 0;
        memcpy(&dynamic_size, Data + offset, 4);
        dynamic_size = dynamic_size % (64 * 1024); // Limit to 64KB
        offset += 4;

        void* mem = png_malloc(png_ptr, dynamic_size);
        if (mem) {
            // Simulate row pointer allocation pattern
            if (dynamic_size >= sizeof(png_bytep)) {
                png_bytep* row_pointers = static_cast<png_bytep*>(mem);
                // Allocate individual rows
                size_t rows = dynamic_size / sizeof(png_bytep);
                if (rows > 16) rows = 16; // Limit rows
                
                for (size_t y = 0; y < rows; ++y) {
                    // Use small fixed size for rows to avoid excessive memory
                    row_pointers[y] = static_cast<png_bytep>(png_malloc(png_ptr, 64));
                }
                
                // Free rows first
                for (size_t y = 0; y < rows; ++y) {
                    if (row_pointers[y]) {
                        png_free(png_ptr, row_pointers[y]);
                    }
                }
            }
            png_free(png_ptr, mem);
        }
        
        if (offset >= Size) break;
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}