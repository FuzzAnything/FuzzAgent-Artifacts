// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
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
#include <png.h>

// Custom memory allocation function for libpng
static void *custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) return nullptr;
    void *ptr = malloc(size);
    if (ptr) {
        // Initialize memory to avoid uninitialized read issues in some fuzzer scenarios
        memset(ptr, 0, size);
    }
    return ptr;
}

// Custom memory deallocation function for libpng
static void custom_free(png_structp png_ptr, void *ptr) {
    (void)png_ptr;
    free(ptr);
}

// Custom error function to prevent libpng from calling exit()
static void custom_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real scenario, we might log this, but for fuzzing we just swallow it
    // to allow the fuzzer to continue exploring other paths.
}

// Custom warning function
static void custom_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Helper to create a valid png_struct for testing
static png_structp create_test_png_struct() {
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr, // error_ptr
        custom_error,
        custom_warning,
        nullptr, // mem_ptr (will be set later if needed)
        custom_malloc,
        custom_free
    );
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    // Create a png_struct
    png_structp png_ptr = create_test_png_struct();
    if (!png_ptr) {
        return 0;
    }

    // Use the fuzzer input to drive logic
    uint32_t action = (Size > 0) ? Data[0] : 0;
    
    // Safe extraction of size_hint ensuring bounds check
    uint32_t size_hint = 10;
    if (Size > 2) {
        size_hint = (Data[1] * 256 + Data[2]);
    } else if (Size > 1) {
        size_hint = Data[1];
    }
    
    // Ensure size_hint is reasonable to avoid huge allocations or 0
    if (size_hint == 0) size_hint = 1;
    if (size_hint > 1024 * 1024) size_hint = 1024 * 1024; // Cap at 1MB

    void *allocated_ptr = nullptr;
    void *mem_context = nullptr;

    // 2. Explore program states based on input
    // We use a switch with blocks to avoid variable initialization issues across case labels
    switch (action % 5) {
        case 0:
            // Test png_get_mem_ptr with default settings
            mem_context = png_get_mem_ptr(png_ptr);
            break;

        case 1: {
            // Test png_set_mem_fn and then png_get_mem_ptr
            mem_context = (void*)(size_hint); // Use size_hint as a dummy context pointer
            png_set_mem_fn(png_ptr, mem_context, custom_malloc, custom_free);
            void *retrieved_context = png_get_mem_ptr(png_ptr);
            (void)retrieved_context; // Suppress unused warning
            break;
        }

        case 2:
            // Test png_malloc
            allocated_ptr = png_malloc(png_ptr, size_hint);
            if (allocated_ptr) {
                // Write some data to the allocated memory to ensure it's valid
                memset(allocated_ptr, 0xAB, size_hint);
            }
            break;

        case 3:
            // Test png_malloc followed by png_free
            allocated_ptr = png_malloc(png_ptr, size_hint);
            if (allocated_ptr) {
                memset(allocated_ptr, 0xCD, size_hint);
                png_free(png_ptr, allocated_ptr);
                allocated_ptr = nullptr;
            }
            break;
            
        case 4:
            // Test png_free_default (deprecated but available)
            allocated_ptr = png_malloc(png_ptr, size_hint);
            if (allocated_ptr) {
                memset(allocated_ptr, 0xEF, size_hint);
                png_free_default(png_ptr, allocated_ptr);
                allocated_ptr = nullptr;
            }
            break;
    }

    // 3. Cleanup
    // Free any remaining allocated memory if not already freed in the switch
    if (allocated_ptr) {
        png_free(png_ptr, allocated_ptr);
    }

    // Destroy the png_struct
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}