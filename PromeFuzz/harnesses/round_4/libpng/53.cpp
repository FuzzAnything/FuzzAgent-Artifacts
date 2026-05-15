// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
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
#include "code/png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    // Initialize a png_struct for write operations
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    // Initialize a png_info
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return 0;
    }

    // Use the fuzzer input to determine allocation size and function selection
    if (Size > 0) {
        // Use first byte to select function (0-5 for the 6 functions)
        uint8_t func_selector = Data[0] % 6;
        
        // Use next 8 bytes for allocation size (or as many as available)
        png_alloc_size_t alloc_size = 0;
        size_t size_bytes = (Size - 1) > sizeof(png_alloc_size_t) ? sizeof(png_alloc_size_t) : (Size - 1);
        if (size_bytes > 0) {
            memcpy(&alloc_size, Data + 1, size_bytes);
        }
        
        // Limit allocation size to avoid excessive memory usage
        if (alloc_size > 1024 * 1024) { // 1 MB limit
            alloc_size = 1024 * 1024;
        }

        void* allocated_ptr = nullptr;
        void* mem_ptr = nullptr;

        switch (func_selector) {
            case 0: // png_malloc_warn
                allocated_ptr = png_malloc_warn(png_ptr, alloc_size);
                if (allocated_ptr) {
                    png_free(png_ptr, allocated_ptr);
                }
                break;
                
            case 1: // png_malloc_default
                allocated_ptr = png_malloc_default(png_ptr, alloc_size);
                if (allocated_ptr) {
                    png_free_default(png_ptr, allocated_ptr);
                }
                break;
                
            case 2: // png_free_default (needs something to free)
                if (alloc_size > 0) {
                    allocated_ptr = png_malloc_default(png_ptr, alloc_size);
                    png_free_default(png_ptr, allocated_ptr);
                }
                break;
                
            case 3: // png_calloc
                allocated_ptr = png_calloc(png_ptr, alloc_size);
                if (allocated_ptr) {
                    png_free(png_ptr, allocated_ptr);
                }
                break;
                
            case 4: // png_get_mem_ptr
                mem_ptr = png_get_mem_ptr(png_ptr);
                (void)mem_ptr; // Use result to avoid unused variable warning
                break;
                
            case 5: // png_malloc
                allocated_ptr = png_malloc(png_ptr, alloc_size);
                if (allocated_ptr) {
                    png_free(png_ptr, allocated_ptr);
                }
                break;
        }

        // Test row_pointers allocation and freeing as mentioned in png_malloc documentation
        if (Size > 10 && alloc_size > 0 && alloc_size < 10000) {
            // Simulate row_pointers allocation
            png_bytepp row_pointers = (png_bytepp)png_malloc(png_ptr, 
                sizeof(png_bytep) * (alloc_size % 100));
            
            if (row_pointers) {
                for (size_t y = 0; y < (alloc_size % 100); y++) {
                    row_pointers[y] = (png_bytep)png_malloc(png_ptr, alloc_size % 1000);
                }
                
                // Free row_pointers as per documentation
                for (size_t y = 0; y < (alloc_size % 100); y++) {
                    if (row_pointers[y]) {
                        png_free(png_ptr, row_pointers[y]);
                    }
                }
                png_free(png_ptr, row_pointers);
            }
        }
    }

    // Clean up
    png_destroy_write_struct(&png_ptr, &info_ptr);
    
    return 0;
}