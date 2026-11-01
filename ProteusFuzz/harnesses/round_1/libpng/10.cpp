// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc_default at pngmem.c:190:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstdio>

static void
fuzz_cleanup(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

extern "C" int
LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    int result = 0;

    // Initialize PNG structures
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Use a dummy file for read operations if needed, though these APIs don't strictly require reading
    // We focus on the memory management and query APIs provided.

    // 1. Test png_set_chunk_malloc_max
    // Use input data to determine the limit, ensuring it's not too small to cause immediate issues
    // but varied enough to test logic.
    png_alloc_size_t max_alloc = 0;
    if (Size > 0) {
        max_alloc = Data[0];
        if (max_alloc == 0) max_alloc = 1024; // Avoid 0 if we want to test non-zero logic, though 0 is valid (unlimited)
    }
    png_set_chunk_malloc_max(png_ptr, max_alloc);

    // 2. Test png_get_chunk_malloc_max
    png_alloc_size_t retrieved_max = png_get_chunk_malloc_max(png_ptr);
    (void)retrieved_max; // Suppress unused warning

    // 3. Test png_get_rowbytes
    // This requires info_ptr to be populated with dimensions, which happens after reading a header.
    // Since we are fuzzing without a valid PNG file, info_ptr->rowbytes might be 0 or uninitialized.
    // The API is designed to return 0 if pointers are null or state is invalid, so this is safe.
    size_t row_bytes = png_get_rowbytes(png_ptr, info_ptr);
    (void)row_bytes;

    // 4. Test png_malloc_warn
    // Allocate a small buffer based on input to test the warning path if allocation fails (unlikely for small sizes)
    // or success path.
    size_t alloc_size = (Size > 0) ? Data[0] : 10;
    // Cap size to prevent huge allocations in fuzzing
    if (alloc_size > 1024) alloc_size = 1024;
    
    void *ptr_warn = png_malloc_warn(png_ptr, alloc_size);
    if (ptr_warn) {
        // Free it using standard free if png_malloc_warn uses system malloc, 
        // but libpng usually expects png_free. However, png_malloc_warn returns void*.
        // To be safe and consistent with libpng, we should use png_free if we have a valid png_ptr.
        // Note: png_free is the counterpart to png_malloc.
        png_free(png_ptr, ptr_warn);
    }

    // 5. Test png_malloc
    // This will call png_error on failure. We need to set up error handling or expect abort.
    // In fuzzing, we usually want to avoid aborting on expected errors.
    // However, png_malloc is designed to abort on failure. 
    // To fuzz this safely, we try small allocations that should succeed.
    size_t alloc_size_safe = (Size > 0) ? Data[0] : 10;
    if (alloc_size_safe > 1024) alloc_size_safe = 1024;

    void *ptr_malloc = png_malloc(png_ptr, alloc_size_safe);
    if (ptr_malloc) {
        png_free(png_ptr, ptr_malloc);
    }

    // 6. Test png_malloc_default (Deprecated)
    // Similar to png_malloc but uses system malloc directly.
    void *ptr_default = png_malloc_default(png_ptr, alloc_size_safe);
    if (ptr_default) {
        // png_malloc_default uses system malloc, so we use free.
        // However, libpng documentation says it's for internal use.
        // If it returns a pointer, it's likely from malloc.
        free(ptr_default);
    }

    // Cleanup
    fuzz_cleanup(png_ptr, info_ptr);

    return 0;
}