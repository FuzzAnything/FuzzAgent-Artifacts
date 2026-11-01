// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free_default at pngmem.c:247:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
#include <png.h>
#include <setjmp.h>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstddef>

// Forward declaration to avoid accessing incomplete type in error handler
// We cannot access png_struct members directly due to opaque structure in newer libpng versions.
// Instead, we use the standard error handling mechanism or a simple abort.
static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // In a real scenario, this would longjmp, but we cannot access jmp_buf directly.
    // For fuzzing, we can just return or abort.
    abort();
}

static void dummy_warn_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

static void *dummy_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    void *ptr = malloc(size);
    if (ptr == nullptr && size > 0) {
        png_error(png_ptr, "Out of memory");
    }
    return ptr;
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    if (ptr != nullptr) {
        free(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    void *mem_ptr = nullptr;
    void *allocated_mem = nullptr;

    // Parse input to determine behavior
    uint8_t mode = Data[0] % 10;

    // Create read struct with custom handlers
    png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr,
        dummy_error_fn,
        dummy_warn_fn,
        nullptr,
        dummy_malloc_fn,
        dummy_free_fn
    );

    if (png_ptr == nullptr) {
        return 0;
    }

    // Initialize info structure
    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }

    // Test png_get_mem_ptr
    void *retrieved_mem_ptr = png_get_mem_ptr(png_ptr);
    (void)retrieved_mem_ptr; // Use to avoid unused warning

    // Test png_set_mem_fn with custom pointer
    if (mode == 1) {
        mem_ptr = malloc(100);
        if (mem_ptr != nullptr) {
            png_set_mem_fn(png_ptr, mem_ptr, dummy_malloc_fn, dummy_free_fn);
            retrieved_mem_ptr = png_get_mem_ptr(png_ptr);
            (void)retrieved_mem_ptr;
            free(mem_ptr);
        }
    }

    // Test png_malloc
    png_alloc_size_t alloc_size = 0;
    if (Size > 1) {
        alloc_size = (png_alloc_size_t)Data[1];
        if (alloc_size > 1024 * 1024) { // Limit allocation size
            alloc_size = 1024 * 1024;
        }
    }
    
    if (alloc_size > 0) {
        allocated_mem = png_malloc(png_ptr, alloc_size);
        if (allocated_mem != nullptr) {
            // Write some data to avoid unused memory warnings
            memset(allocated_mem, 0xAB, alloc_size);
        }
    }

    // Test png_free_default (deprecated but tested)
    if (allocated_mem != nullptr) {
        png_free_default(png_ptr, allocated_mem);
        allocated_mem = nullptr;
    }

    // Test png_free
    if (Size > 2) {
        png_alloc_size_t alloc_size2 = (png_alloc_size_t)Data[2];
        if (alloc_size2 > 0 && alloc_size2 <= 1024 * 1024) {
            allocated_mem = png_malloc(png_ptr, alloc_size2);
            if (allocated_mem != nullptr) {
                memset(allocated_mem, 0xCD, alloc_size2);
                png_free(png_ptr, allocated_mem);
                allocated_mem = nullptr;
            }
        }
    }

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);

    return 0;
}