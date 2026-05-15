// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
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

#include "png.h"

// Custom memory functions for testing
static void* custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void custom_free(png_structp png_ptr, void* ptr) {
    (void)png_ptr;
    free(ptr);
}

// Error and warning handlers
static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    // Create a dummy file for libpng operations
    const char* dummy_file = "./dummy_file";
    FILE* fp = fopen(dummy_file, "wb");
    if (!fp) {
        return 0;
    }
    
    // Write fuzzed data to dummy file
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Test 1: Create write structure with default memory handlers
    png_structp png_ptr1 = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        error_handler,
        warning_handler,
        nullptr,
        nullptr,
        nullptr
    );
    
    if (png_ptr1) {
        // Test png_calloc with default allocator
        void* mem1 = png_calloc(png_ptr1, Size > 0 ? Size : 1);
        if (mem1) {
            // Test png_free with default allocator
            png_free(png_ptr1, mem1);
            
            // Test deprecated png_free_default
            void* mem2 = png_calloc(png_ptr1, Size > 0 ? Size : 1);
            if (mem2) {
                png_free_default(png_ptr1, mem2);
            }
        }
        
        // Test png_get_mem_ptr with default (should return NULL)
        void* mem_ptr1 = png_get_mem_ptr(png_ptr1);
        (void)mem_ptr1;
        
        // Clean up
        png_destroy_write_struct(&png_ptr1, nullptr);
    }
    
    // Test 2: Create write structure with custom memory handlers
    png_structp png_ptr2 = png_create_write_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        error_handler,
        warning_handler,
        (void*)0x12345678,  // Arbitrary user pointer
        custom_malloc,
        custom_free
    );
    
    if (png_ptr2) {
        // Test png_get_mem_ptr with custom handler
        void* mem_ptr2 = png_get_mem_ptr(png_ptr2);
        (void)mem_ptr2;
        
        // Test png_calloc with custom allocator
        void* mem3 = png_calloc(png_ptr2, Size > 0 ? Size : 1);
        if (mem3) {
            // Test png_free with custom allocator
            png_free(png_ptr2, mem3);
        }
        
        // Test png_set_mem_fn to change memory handlers
        png_set_mem_fn(png_ptr2, (void*)0x87654321, custom_malloc, custom_free);
        
        // Test allocations with new handlers
        void* mem4 = png_calloc(png_ptr2, Size > 0 ? Size : 1);
        if (mem4) {
            png_free(png_ptr2, mem4);
        }
        
        // Clean up
        png_destroy_write_struct(&png_ptr2, nullptr);
    }
    
    // Test 3: Test png_set_mem_fn with NULL png_ptr (should do nothing)
    png_set_mem_fn(nullptr, nullptr, nullptr, nullptr);
    
    // Test 4: Test png_free with NULL pointers (should return early)
    png_free(nullptr, nullptr);
    png_free(nullptr, (void*)0x1);
    
    // Test 5: Create multiple structures with different configurations
    for (int i = 0; i < 3 && i < (int)Size; i++) {
        png_structp png_ptr = png_create_write_struct_2(
            PNG_LIBPNG_VER_STRING,
            (void*)(uintptr_t)Data[i],
            (i % 2) ? error_handler : nullptr,
            (i % 3) ? warning_handler : nullptr,
            (void*)(uintptr_t)Data[i],
            (i % 2) ? custom_malloc : nullptr,
            (i % 2) ? custom_free : nullptr
        );
        
        if (png_ptr) {
            // Test various allocations
            size_t alloc_size = (Size > 0) ? (Data[i % Size] + 1) : 1;
            void* mem = png_calloc(png_ptr, alloc_size);
            if (mem) {
                png_free(png_ptr, mem);
            }
            
            png_destroy_write_struct(&png_ptr, nullptr);
        }
    }
    
    // Remove dummy file
    remove(dummy_file);
    
    return 0;
}