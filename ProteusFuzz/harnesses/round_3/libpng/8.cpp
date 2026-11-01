// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free at pngmem.c:234:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_default at pngmem.c:190:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_chunk_malloc_max at pngget.c:1325:1 in png.h
// png_set_chunk_malloc_max at pngset.c:1965:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
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
#include <cstdlib>
#include <cstring>
#include <vector>
#include <stdexcept>

// Static helper to handle libpng errors without aborting
static void fuzz_png_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real scenario, we might log this, but for fuzzing we just return
    // to avoid crashing the fuzzer.
}

static void fuzz_png_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Static helper to create a minimal valid png_struct
static std::pair<png_structp, png_infop> create_png_struct() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, fuzz_png_error, fuzz_png_warning);
    if (!png_ptr) {
        return {nullptr, nullptr};
    }
    // Set a dummy info struct to ensure full initialization
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return {nullptr, nullptr};
    }
    return {png_ptr, info_ptr};
}

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Parse input to determine test case
    uint32_t action = *reinterpret_cast<const uint32_t*>(Data);
    uint32_t size_val = 0;
    if (Size >= 8) {
        size_val = *reinterpret_cast<const uint32_t*>(Data + 4);
    }

    // Ensure size_val is reasonable to avoid immediate OOM or invalid states
    if (size_val > 1024 * 1024) { // Cap at 1MB for fuzzing safety
        size_val = 1024 * 1024;
    }

    auto [png_ptr, info_ptr] = create_png_struct();
    if (!png_ptr || !info_ptr) {
        return 0;
    }

    // Action 0: png_get_chunk_malloc_max
    if (action == 0) {
        png_alloc_size_t limit = png_get_chunk_malloc_max(png_ptr);
        (void)limit; // Use the result to avoid unused warnings
    }

    // Action 1: png_set_chunk_malloc_max
    else if (action == 1) {
        png_set_chunk_malloc_max(png_ptr, size_val);
    }

    // Action 2: png_malloc
    else if (action == 2) {
        void *ptr = png_malloc(png_ptr, size_val);
        if (ptr) {
            // Write some data to ensure memory is valid
            memset(ptr, 0xAA, size_val);
            png_free(png_ptr, ptr);
        }
    }

    // Action 3: png_malloc_warn
    else if (action == 3) {
        void *ptr = png_malloc_warn(png_ptr, size_val);
        if (ptr) {
            memset(ptr, 0xBB, size_val);
            png_free(png_ptr, ptr);
        }
    }

    // Action 4: png_calloc
    else if (action == 4) {
        void *ptr = png_calloc(png_ptr, size_val);
        if (ptr) {
            // Verify it's zeroed (optional check)
            // memset(ptr, 0xCC, size_val); // Overwrite to test usage
            png_free(png_ptr, ptr);
        }
    }

    // Action 5: png_malloc_default
    else if (action == 5) {
        void *ptr = png_malloc_default(png_ptr, size_val);
        if (ptr) {
            memset(ptr, 0xDD, size_val);
            png_free(png_ptr, ptr);
        }
    }

    // Action 6: Test with NULL pointer (should be safe)
    else if (action == 6) {
        png_get_chunk_malloc_max(nullptr);
        png_set_chunk_malloc_max(nullptr, size_val);
        // png_malloc, png_malloc_warn, png_calloc, png_malloc_default with null
        // typically trigger error callbacks which we handle, or return null.
        // We skip explicit calls to these with null to avoid triggering error
        // callbacks repeatedly in a loop if the fuzzer generates this often,
        // as the error callback is already defined to be safe.
        // However, to be thorough:
        void *ptr = png_malloc(nullptr, size_val);
        if (ptr) png_free(nullptr, ptr);
        
        ptr = png_malloc_warn(nullptr, size_val);
        if (ptr) png_free(nullptr, ptr);

        ptr = png_calloc(nullptr, size_val);
        if (ptr) png_free(nullptr, ptr);

        ptr = png_malloc_default(nullptr, size_val);
        if (ptr) png_free(nullptr, ptr);
    }

    // Cleanup: Pass info_ptr to destroy to prevent leaks
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}