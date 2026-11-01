// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free_default at pngmem.c:247:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstdlib>

// Custom memory tracking for fuzzing
static void* fuzz_malloc(png_structp png_ptr, png_alloc_size_t size) {
    if (size == 0) {
        return nullptr;
    }
    return malloc(size);
}

static void fuzz_free(png_structp png_ptr, void* ptr) {
    if (ptr != nullptr) {
        free(ptr);
    }
}

// Custom error handler that does nothing to prevent aborts during fuzzing
static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warn_fn(png_structp png_ptr, png_const_charp warn_msg) {
    (void)png_ptr;
    (void)warn_msg;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use the input data to seed decisions
    uint32_t seed = 0;
    if (Size >= 4) {
        seed = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }

    // 1. Prepare environment: Create png_struct
    png_structp png_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        nullptr, // error_ptr
        dummy_error_fn,
        dummy_warn_fn,
        nullptr, // mem_ptr
        fuzz_malloc,
        fuzz_free
    );

    if (png_ptr == nullptr) {
        return 0;
    }

    // 2. Test png_get_mem_ptr
    void* mem_ptr = png_get_mem_ptr(png_ptr);
    (void)mem_ptr;

    // 3. Test png_set_mem_fn
    if (seed & 1) {
        png_set_mem_fn(png_ptr, (void*)0x12345678, fuzz_malloc, fuzz_free);
    }

    // 4. Test png_malloc
    png_alloc_size_t alloc_size = 0;
    if (Size >= 2) {
        alloc_size = Data[0] + 1;
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
    }

    void* ptr = nullptr;
    if (alloc_size > 0) {
        ptr = png_malloc(png_ptr, alloc_size);
    }

    // 5. Test png_free
    if (ptr != nullptr) {
        png_free(png_ptr, ptr);
        ptr = nullptr;
    }

    // 6. Test png_free_default (Deprecated)
    if (Size >= 2) {
        alloc_size = Data[1] + 1;
        if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;
        
        void* ptr_default = png_malloc(png_ptr, alloc_size);
        if (ptr_default != nullptr) {
            png_free_default(png_ptr, ptr_default);
        }
    }

    // Test with NULL pointer for png_free_default
    png_free_default(png_ptr, nullptr);
    
    // Test with NULL png_ptr for png_free_default
    png_free_default(nullptr, nullptr);

    // 7. Cleanup
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);

    return 0;
}