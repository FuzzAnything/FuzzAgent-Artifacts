// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
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
#include <setjmp.h>

// Custom memory allocation function for fuzzing
static void *fuzz_malloc(png_structp png_ptr, png_alloc_size_t size) {
    if (size == 0) {
        return NULL;
    }
    // Avoid excessively large allocations
    if (size > 1024 * 1024) {
        return NULL;
    }
    void *ptr = malloc(size);
    return ptr;
}

// Custom memory deallocation function for fuzzing
static void fuzz_free(png_structp png_ptr, void *ptr) {
    if (ptr != NULL) {
        free(ptr);
    }
}

// Custom error handler that does NOT abort or longjmp
static void fuzz_error(png_structp png_ptr, png_const_charp error_message) {
    (void)png_ptr;
    (void)error_message;
    // Do nothing to prevent crash
}

// Custom warning handler
static void fuzz_warn(png_structp png_ptr, png_const_charp warning_message) {
    (void)png_ptr;
    (void)warning_message;
}

// Static helper to safely create a png_struct for fuzzing
static png_structp create_fuzz_png_struct(const uint8_t *data, size_t size) {
    const char *user_png_ver = PNG_LIBPNG_VER_STRING;
    
    // Create the read struct with custom error handlers to prevent aborts
    png_structp png_ptr = png_create_read_struct_2(
        user_png_ver,
        NULL, // error_ptr
        fuzz_error, // error_fn (custom, non-aborting)
        fuzz_warn,  // warn_fn (custom)
        NULL,       // mem_ptr
        fuzz_malloc,
        fuzz_free
    );
    
    if (png_ptr == NULL) {
        return NULL;
    }
    
    // Set custom memory functions again if needed
    void *custom_mem_ptr = (size > 0) ? (void *)(const_cast<uint8_t*>(data)) : NULL;
    
    png_set_mem_fn(png_ptr, custom_mem_ptr, fuzz_malloc, fuzz_free);
    
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a png_struct
    png_structp png_ptr = create_fuzz_png_struct(Data, Size);
    
    if (png_ptr == NULL) {
        return 0;
    }

    // 1. Test png_get_mem_ptr
    void *mem_ptr = png_get_mem_ptr(png_ptr);
    (void)mem_ptr; 

    // 2. Test png_malloc
    png_alloc_size_t alloc_size = 0;
    if (Size >= 2) {
        alloc_size = (png_alloc_size_t)((Data[0] << 8) | Data[1]);
    } else if (Size == 1) {
        alloc_size = (png_alloc_size_t)Data[0];
    }
    
    // Clamp size to avoid excessive allocation or invalid sizes
    if (alloc_size > 1024 * 1024) { 
        alloc_size = 1024 * 1024;
    }
    
    // We use a try-catch block with setjmp/longjmp simulation if needed, 
    // but our custom error handler prevents the longjmp.
    // If png_malloc returns NULL, it means allocation failed (handled by our malloc logic).
    void *allocated_ptr = png_malloc(png_ptr, alloc_size);
    
    if (allocated_ptr != NULL) {
        // Write some fuzz data to the allocated memory
        size_t write_len = (alloc_size > Size) ? Size : alloc_size;
        if (write_len > 0) {
            memcpy(allocated_ptr, Data, write_len);
        }
        
        // 3. Test png_free
        png_free(png_ptr, allocated_ptr);
        allocated_ptr = NULL; 
    }

    // 4. Test png_free_default
    void *ptr_for_default = png_malloc(png_ptr, alloc_size);
    if (ptr_for_default != NULL) {
        png_free_default(png_ptr, ptr_for_default);
        ptr_for_default = NULL;
    }

    // Cleanup: Destroy the png_struct
    png_infop info_ptr = NULL;
    // png_destroy_read_struct is safe to call even if we didn't initialize IO
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    return 0;
}