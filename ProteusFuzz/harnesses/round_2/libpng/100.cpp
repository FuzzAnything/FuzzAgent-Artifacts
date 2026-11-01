// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_user_transform_ptr at pngtrans.c:836:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// Custom allocator functions for fuzzing
static void *fuzz_malloc(png_structp png_ptr, png_size_t size) {
    if (size == 0) return NULL;
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

static void fuzz_free(png_structp png_ptr, png_voidp ptr) {
    if (ptr) {
        free(ptr);
    }
}

// Custom error handler that does nothing to prevent aborts during fuzzing
static void fuzz_error(png_structp png_ptr, png_const_charp error_msg) {
    // Intentionally silent to allow fuzzing to continue
    (void)png_ptr;
    (void)error_msg;
}

// Custom warning handler that does nothing
static void fuzz_warn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Helper to create a read struct with custom handlers
static png_structp create_fuzz_read_struct(const uint8_t *data, size_t size) {
    if (size < 1) return NULL;
    
    // Use a portion of fuzz data as version string (ensure null termination)
    char version[256];
    size_t ver_len = (size < 255) ? size : 255;
    memcpy(version, data, ver_len);
    version[ver_len] = '\0';
    
    png_structp png_ptr = png_create_read_struct_2(
        version,
        (void*)data, // error_ptr
        fuzz_error,
        fuzz_warn,
        (void*)data, // mem_ptr
        fuzz_malloc,
        fuzz_free
    );
    
    return png_ptr;
}

// Helper to create a write struct with custom handlers
static png_structp create_fuzz_write_struct(const uint8_t *data, size_t size) {
    if (size < 1) return NULL;
    
    // Use a portion of fuzz data as version string (ensure null termination)
    char version[256];
    size_t ver_len = (size < 255) ? size : 255;
    memcpy(version, data, ver_len);
    version[ver_len] = '\0';
    
    png_structp png_ptr = png_create_write_struct_2(
        version,
        (void*)data, // error_ptr
        fuzz_error,
        fuzz_warn,
        (void*)data, // mem_ptr
        fuzz_malloc,
        fuzz_free
    );
    
    return png_ptr;
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test 1: Create read struct and test memory functions
    png_structp read_ptr = create_fuzz_read_struct(Data, Size);
    if (read_ptr) {
        // Test png_set_mem_fn with custom functions
        void *custom_mem_ptr = (void*)(Data + (Size / 2));
        png_set_mem_fn(read_ptr, custom_mem_ptr, fuzz_malloc, fuzz_free);
        
        // Test png_get_mem_ptr
        void *mem_ptr = png_get_mem_ptr(read_ptr);
        (void)mem_ptr; // Use to avoid unused warning
        
        // Test png_get_error_ptr
        void *error_ptr = png_get_error_ptr(read_ptr);
        (void)error_ptr;
        
        // Test png_get_user_transform_ptr
        void *user_transform_ptr = png_get_user_transform_ptr(read_ptr);
        (void)user_transform_ptr;
        
        png_destroy_read_struct(&read_ptr, NULL, NULL);
    }
    
    // Test 2: Create write struct and test memory functions
    png_structp write_ptr = create_fuzz_write_struct(Data, Size);
    if (write_ptr) {
        // Test png_set_mem_fn with custom functions
        void *custom_mem_ptr = (void*)(Data + (Size / 3));
        png_set_mem_fn(write_ptr, custom_mem_ptr, fuzz_malloc, fuzz_free);
        
        // Test png_get_mem_ptr
        void *mem_ptr = png_get_mem_ptr(write_ptr);
        (void)mem_ptr;
        
        // Test png_get_error_ptr
        void *error_ptr = png_get_error_ptr(write_ptr);
        (void)error_ptr;
        
        // Test png_get_user_transform_ptr
        void *user_transform_ptr = png_get_user_transform_ptr(write_ptr);
        (void)user_transform_ptr;
        
        png_destroy_write_struct(&write_ptr, NULL);
    }
    
    // Test 3: Test with NULL pointer (should be safe)
    png_set_mem_fn(NULL, NULL, fuzz_malloc, fuzz_free);
    void *null_mem_ptr = png_get_mem_ptr(NULL);
    void *null_error_ptr = png_get_error_ptr(NULL);
    void *null_user_transform_ptr = png_get_user_transform_ptr(NULL);
    (void)null_mem_ptr;
    (void)null_error_ptr;
    (void)null_user_transform_ptr;
    
    // Test 4: Create struct with different data patterns
    if (Size > 10) {
        png_structp alt_ptr = create_fuzz_read_struct(Data + 5, Size - 5);
        if (alt_ptr) {
            png_set_mem_fn(alt_ptr, (void*)Data, fuzz_malloc, fuzz_free);
            void *ptr = png_get_mem_ptr(alt_ptr);
            (void)ptr;
            png_destroy_read_struct(&alt_ptr, NULL, NULL);
        }
    }
    
    return 0;
}