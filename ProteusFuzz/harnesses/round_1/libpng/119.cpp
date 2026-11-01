// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free at pngmem.c:234:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdlib>

// Custom memory allocation function
static void *custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    if (size == 0) return nullptr;
    void *ptr = malloc(size);
    if (ptr == nullptr) {
        png_error(png_ptr, "Out of memory");
    }
    return ptr;
}

// Custom memory deallocation function
static void custom_free(png_structp png_ptr, void *ptr) {
    if (ptr != nullptr) {
        free(ptr);
    }
}

// Custom error function to prevent aborts during fuzzing
static void custom_error(png_structp png_ptr, png_const_charp error_msg) {
    // Ignore errors to allow fuzzing to continue
    (void)png_ptr;
    (void)error_msg;
}

// Custom warning function
static void custom_warning(png_structp png_ptr, png_const_charp warning_msg) {
    // Ignore warnings to allow fuzzing to continue
    (void)png_ptr;
    (void)warning_msg;
}

// Helper to create a valid png_struct for testing
static png_structp create_png_struct(const uint8_t *data, size_t size) {
    // FIX: Always pass a valid, safe version string instead of raw input data.
    // Passing raw fuzz input as a version string caused a buffer overflow 
    // because libpng iterates over it without bounds checking relative to the input size.
    const char *version = PNG_LIBPNG_VER_STRING;
    
    png_structp png_ptr = png_create_read_struct_2(
        version,
        nullptr, // error_ptr
        custom_error,
        custom_warning,
        nullptr, // mem_ptr (will be set later)
        custom_malloc,
        custom_free
    );
    
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a png_struct
    png_structp png_ptr = create_png_struct(Data, Size);
    if (png_ptr == nullptr) {
        return 0;
    }

    // Test png_get_mem_ptr with default (should be NULL initially)
    void *mem_ptr = png_get_mem_ptr(png_ptr);
    (void)mem_ptr; // Use the result to avoid unused variable warning

    // Set custom memory functions
    void *custom_mem_context = (void *)(uintptr_t)Data; // Use input data as context
    png_set_mem_fn(png_ptr, custom_mem_context, custom_malloc, custom_free);

    // Verify mem_ptr after setting
    mem_ptr = png_get_mem_ptr(png_ptr);
    if (mem_ptr != custom_mem_context) {
        // This is expected behavior; just continue
    }

    // Test png_malloc
    png_alloc_size_t alloc_size = (Size > 0) ? (png_alloc_size_t)Data[0] : 100;
    if (alloc_size == 0) alloc_size = 1; // Avoid zero allocation
    if (alloc_size > 1024) alloc_size = 1024; // Limit allocation size for safety

    void *allocated_ptr = png_malloc(png_ptr, alloc_size);
    if (allocated_ptr != nullptr) {
        // Write some data to the allocated memory
        memset(allocated_ptr, 0xAA, alloc_size);

        // Test png_free
        png_free(png_ptr, allocated_ptr);
    }

    // Test png_free_default (deprecated but still callable)
    void *ptr_for_default_free = malloc(alloc_size);
    if (ptr_for_default_free != nullptr) {
        png_free_default(png_ptr, ptr_for_default_free);
    }

    // Test with NULL pointer for png_free
    png_free(png_ptr, nullptr);

    // Test with NULL png_ptr for png_get_mem_ptr
    void *null_mem_ptr = png_get_mem_ptr(nullptr);
    if (null_mem_ptr != nullptr) {
        // Should be NULL
    }

    // Clean up png_struct
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}