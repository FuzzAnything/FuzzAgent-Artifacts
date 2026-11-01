// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
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

// Custom memory tracking for fuzzing
static struct {
    size_t total_allocated;
    size_t alloc_count;
} g_mem_stats = {0, 0};

// Custom malloc function
static void *custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    if (size == 0) {
        return nullptr;
    }
    void *ptr = malloc(size);
    if (ptr) {
        g_mem_stats.total_allocated += size;
        g_mem_stats.alloc_count++;
        // Zero out memory to detect uninitialized reads
        memset(ptr, 0, size);
    }
    return ptr;
}

// Custom free function
static void custom_free(png_structp png_ptr, void *ptr) {
    if (ptr) {
        free(ptr);
        g_mem_stats.total_allocated = 0; // Reset stats on free for simplicity in fuzzing
        g_mem_stats.alloc_count = 0;
    }
}

// Error handler that does nothing (prevents aborts during fuzzing)
static void custom_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

// Warning handler that does nothing
static void custom_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to read at least 8 bytes (2 uint32_t)
    if (Size < 8) {
        return 0;
    }

    // Extract parameters from fuzz input safely
    uint32_t mode = 0;
    uint32_t alloc_size = 0;
    
    // Read first 4 bytes for mode
    std::memcpy(&mode, Data, sizeof(uint32_t));
    
    // Read next 4 bytes for alloc_size
    std::memcpy(&alloc_size, Data + sizeof(uint32_t), sizeof(uint32_t));
    
    // Clamp sizes to reasonable limits to avoid OOM or extremely slow execution
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    // Test 1: Create read struct with custom memory functions
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr, // error_ptr
        custom_error,
        custom_warning,
        nullptr, // mem_ptr
        custom_malloc,
        custom_free
    );

    if (!png_ptr) {
        return 0;
    }

    // Test 2: png_set_mem_fn - override memory functions again
    void *user_mem_ptr = (void*)(uintptr_t)mode;
    png_set_mem_fn(png_ptr, user_mem_ptr, custom_malloc, custom_free);

    // Test 3: png_get_mem_ptr - retrieve the user pointer
    void *retrieved_ptr = png_get_mem_ptr(png_ptr);
    if (retrieved_ptr != user_mem_ptr) {
        // This is expected to match, but we don't fail on mismatch in fuzzing
    }

    // Test 4: png_malloc - allocate memory
    void *allocated = nullptr;
    if (alloc_size > 0) {
        allocated = png_malloc(png_ptr, alloc_size);
    }

    // Test 5: png_free - free the allocated memory
    if (allocated) {
        png_free(png_ptr, allocated);
    }

    // Test 6: png_free_default - deprecated function, test with NULL and valid
    png_free_default(png_ptr, nullptr);
    // Allocate again to test free_default with non-null
    void *temp_ptr = nullptr;
    if (alloc_size > 0) {
        temp_ptr = png_malloc(png_ptr, alloc_size);
    }
    if (temp_ptr) {
        png_free_default(png_ptr, temp_ptr);
    }

    // Test 7: Test with NULL png_ptr for safety
    png_set_mem_fn(nullptr, user_mem_ptr, custom_malloc, custom_free);
    void *null_mem_ptr = png_get_mem_ptr(nullptr);
    png_free(nullptr, allocated);
    png_free_default(nullptr, allocated);
    void *null_alloc = png_malloc(nullptr, alloc_size);

    // Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}