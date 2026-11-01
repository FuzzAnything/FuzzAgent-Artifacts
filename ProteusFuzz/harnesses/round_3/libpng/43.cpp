// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free at pngmem.c:234:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_info_init_3 at png.c:433:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cstdio>

// Static helper to handle libpng errors without aborting during fuzzing
static void
fuzz_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void
fuzz_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Static allocator that can handle small sizes safely
static void *
fuzz_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) {
        size = 1;
    }
    void *ptr = malloc(size);
    if (!ptr && size > 0) {
        // Simulate failure for very large sizes to test error paths
        if (size > 1024 * 1024) {
            return nullptr;
        }
    }
    return ptr;
}

static void
fuzz_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment: Create png_struct using png_create_read_struct_2
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        fuzz_error_fn,
        fuzz_warning_fn,
        nullptr,
        fuzz_malloc_fn,
        fuzz_free_fn
    );

    if (!png_ptr) {
        return 0;
    }

    // Initialize png_info using png_info_init_3 (deprecated but target API)
    // png_info is an opaque type, so we cannot use sizeof(png_info).
    // png_info_init_3 reallocates if the provided struct size is too small.
    // We pass a reasonable initial size (e.g., 1024) to trigger the reallocation logic if needed.
    png_info *info = nullptr;
    png_infopp info_ptr_p = &info;
    
    // Use a safe default size since png_info is opaque
    size_t init_size = 1024; 
    
    // Call png_info_init_3
    png_info_init_3(info_ptr_p, init_size);

    if (!info) {
        // Allocation failed inside png_info_init_3
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // 2. Test allocation functions with varying sizes from input
    png_alloc_size_t alloc_size = 0;
    if (Size >= sizeof(png_alloc_size_t)) {
        alloc_size = *(const png_alloc_size_t*)Data;
    } else if (Size > 0) {
        alloc_size = Data[0];
    }
    
    // Clamp size to prevent OOM or extremely slow execution
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    // Test png_malloc
    void *malloc_buf = png_malloc(png_ptr, alloc_size);
    if (malloc_buf) {
        // Write fuzz data to the buffer to trigger potential issues
        size_t write_len = (alloc_size > Size) ? Size : alloc_size;
        memcpy(malloc_buf, Data, write_len);
        png_free(png_ptr, malloc_buf);
    }

    // Test png_malloc_warn
    void *warn_buf = png_malloc_warn(png_ptr, alloc_size);
    if (warn_buf) {
        size_t write_len = (alloc_size > Size) ? Size : alloc_size;
        memcpy(warn_buf, Data, write_len);
        png_free(png_ptr, warn_buf);
    }

    // Test png_calloc
    void *calloc_buf = png_calloc(png_ptr, alloc_size);
    if (calloc_buf) {
        // Verify it's zeroed (optional check)
        // Write fuzz data
        size_t write_len = (alloc_size > Size) ? Size : alloc_size;
        memcpy(calloc_buf, Data, write_len);
        png_free(png_ptr, calloc_buf);
    }

    // 3. Test png_get_rowbytes
    // This requires valid png_ptr and info_ptr. 
    // The info structure might not have rowbytes set, but the function should handle it.
    size_t rowbytes = png_get_rowbytes(png_ptr, info);
    (void)rowbytes; // Use the value to suppress unused warnings

    // 4. Cleanup
    // Destroy the read struct and the info struct
    png_destroy_read_struct(&png_ptr, info_ptr_p, nullptr);

    return 0;
}