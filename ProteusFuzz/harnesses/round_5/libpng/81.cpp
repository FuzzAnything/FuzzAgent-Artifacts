// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_calloc at pngmem.c:48:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_get_rowbytes at pngget.c:40:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
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
#include <cstring>
#include <cstdio>

// Static helper to safely destroy libpng structures to avoid memory leaks
static void cleanup_png_struct(png_structp png_ptr, png_infop info_ptr) {
    if (png_ptr != nullptr) {
        if (info_ptr != nullptr) {
            png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
        } else {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        }
    } else if (info_ptr != nullptr) {
        // If png_ptr is null but info_ptr exists, we cannot safely call png_destroy_read_struct.
        // In a fuzzing context, if we created info via png_create_info_struct, we need png_ptr to free it.
        // If we reached here, it implies a logic error in setup or a specific edge case.
        // We do nothing here as we cannot safely free without the struct.
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    png_structp png_ptr = nullptr;
    png_infop info_ptr = nullptr;
    void *malloc_result = nullptr;
    void *calloc_result = nullptr;
    void *malloc_warn_result = nullptr;

    // 1. Prepare the environment: Create read struct
    // We use the first byte to determine if we try to create a struct or just test allocs with null
    bool create_struct = (Data[0] % 2 == 0);

    if (create_struct) {
        png_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING,
            nullptr, // error_ptr
            nullptr, // error_fn (uses default)
            nullptr, // warn_fn (uses default)
            nullptr, // mem_ptr
            nullptr, // malloc_fn (uses default)
            nullptr  // free_fn (uses default)
        );

        if (png_ptr == nullptr) {
            return 0;
        }

        // Create info struct
        info_ptr = png_create_info_struct(png_ptr);
        if (info_ptr == nullptr) {
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            return 0;
        }
    }

    // 2. Fuzzing Logic: Invoke target functions diversely

    // Determine size to allocate based on input data
    // Use modulo to keep size reasonable but varied
    png_alloc_size_t alloc_size = 0;
    
    // Safe access to Data array
    if (Size >= 3) {
        alloc_size = (Data[1] << 8) | Data[2];
    } else if (Size == 2) {
        alloc_size = Data[1];
    } else if (Size == 1) {
        alloc_size = Data[0];
    } else {
        alloc_size = 0;
    }
    
    // Ensure size is not excessively large to prevent OOM in fuzzer
    if (alloc_size > 1024 * 1024) {
        alloc_size = 1024 * 1024;
    }

    // Test png_malloc_warn
    // This function requires a valid png_ptr. If png_ptr is null, behavior is undefined in some versions
    // or it might just return null. We only call it if we have a struct.
    if (png_ptr != nullptr) {
        malloc_warn_result = png_malloc_warn(png_ptr, alloc_size);
        if (malloc_warn_result != nullptr) {
            // Simulate usage and cleanup
            memset(malloc_warn_result, 0xAB, alloc_size);
            png_free(png_ptr, malloc_warn_result);
            malloc_warn_result = nullptr;
        }
    }

    // Test png_malloc
    // If png_ptr is null, png_malloc typically calls png_error which aborts.
    // We only call this if we have a valid struct to avoid crashing the fuzzer.
    if (png_ptr != nullptr) {
        malloc_result = png_malloc(png_ptr, alloc_size);
        if (malloc_result != nullptr) {
            memset(malloc_result, 0xCD, alloc_size);
            png_free(png_ptr, malloc_result);
            malloc_result = nullptr;
        }
    }

    // Test png_calloc
    if (png_ptr != nullptr) {
        calloc_result = png_calloc(png_ptr, alloc_size);
        if (calloc_result != nullptr) {
            // Verify it is zeroed (optional check)
            // memset(calloc_result, 0xFF, alloc_size); // Overwrite to test
            png_free(png_ptr, calloc_result);
            calloc_result = nullptr;
        }
    }

    // Test png_info_init_3
    // This is deprecated and requires a pointer to a png_info struct.
    // We can use the info_ptr we created, or a stack allocated one.
    // Note: The function signature takes png_infopp (pointer to pointer).
    // Since png_info is opaque in png.h, we cannot use sizeof(png_info).
    // We pass the size of the pointer or 0, or rely on the library's internal size check.
    // However, the API expects the actual size of the struct.
    // Since we cannot access the size, we will skip this specific call with sizeof
    // and instead just test png_get_rowbytes which is safer.
    // If we absolutely must test png_info_init_3, we would need to know the size from the version.
    // For safety in fuzzing with opaque types, we skip the sizeof call.
    
    if (info_ptr != nullptr) {
        // We cannot safely call png_info_init_3 with sizeof(png_info) because it is opaque.
        // Skipping this specific deprecated call to avoid compilation error.
        // The function is deprecated anyway.
        
        // Test png_get_rowbytes
        // Returns 0 if png_ptr or info_ptr is NULL.
        size_t rowbytes = png_get_rowbytes(png_ptr, info_ptr);
        (void)rowbytes; // Suppress unused variable warning
    } else if (png_ptr != nullptr) {
        // Test png_get_rowbytes with null info_ptr
        size_t rowbytes_null = png_get_rowbytes(png_ptr, nullptr);
        (void)rowbytes_null;
    }

    // 3. Cleanup
    cleanup_png_struct(png_ptr, info_ptr);

    return 0;
}