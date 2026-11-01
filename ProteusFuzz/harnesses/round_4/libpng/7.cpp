// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_malloc_warn at pngmem.c:213:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_mem_ptr at pngmem.c:279:1 in png.h
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

static void custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Custom error handling: do nothing or log
}

static void custom_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Custom warning handling: do nothing or log
}

static void* custom_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    if (size == 0) {
        return NULL;
    }
    void* ptr = malloc(size);
    if (ptr == NULL && size != 0) {
        // Simulate out of memory if needed, but standard malloc is fine for fuzzing
    }
    return ptr;
}

static void custom_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    if (ptr != NULL) {
        free(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract version string from input if possible, otherwise use default
    // We must ensure the string is null-terminated and stored in a static or heap buffer
    // to avoid stack-use-after-scope when libpng accesses it later.
    const char *user_png_ver = PNG_LIBPNG_VER_STRING;
    
    // Use a static buffer to hold the extracted version string safely
    static char ver_buf[12]; 
    ver_buf[0] = '\0';

    if (Size >= 10) {
        // Copy up to 10 bytes, ensure null termination
        memcpy(ver_buf, Data, 10);
        ver_buf[10] = '\0';
        // Only use if not empty
        if (ver_buf[0] != '\0') {
            user_png_ver = ver_buf;
        }
    }

    // Test 1: png_create_read_struct_2
    png_structp read_ptr = png_create_read_struct_2(
        user_png_ver,
        NULL, // error_ptr
        custom_error_fn,
        custom_warning_fn,
        NULL, // mem_ptr
        custom_malloc_fn,
        custom_free_fn
    );

    if (read_ptr != NULL) {
        // Test 2: png_get_mem_ptr
        void *mem_ptr = png_get_mem_ptr(read_ptr);
        (void)mem_ptr; // Use to avoid unused warning

        // Test 3: png_set_mem_fn
        png_set_mem_fn(read_ptr, (void*)0x12345678, custom_malloc_fn, custom_free_fn);

        // Test 4: png_malloc_warn
        // Use input data to determine size, clamp to reasonable limits
        png_alloc_size_t alloc_size = 0;
        if (Size >= 4) {
            // Safe read of 4 bytes
            uint32_t val = 0;
            memcpy(&val, Data, 4);
            alloc_size = val;
        } else if (Size > 0) {
            alloc_size = Data[0];
        }
        
        // Limit allocation size to prevent OOM during fuzzing
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }

        void *malloced_ptr = png_malloc_warn(read_ptr, alloc_size);
        if (malloced_ptr != NULL) {
            // Write some data to the allocated memory to trigger potential issues
            size_t write_size = (Size > alloc_size) ? alloc_size : Size;
            if (write_size > 0) {
                memcpy(malloced_ptr, Data, write_size);
            }
            // Test 5: png_free
            png_free(read_ptr, malloced_ptr);
        }

        // Cleanup read struct
        png_destroy_read_struct(&read_ptr, NULL, NULL);
    }

    // Test 6: png_create_write_struct_2
    png_structp write_ptr = png_create_write_struct_2(
        user_png_ver,
        NULL, // error_ptr
        custom_error_fn,
        custom_warning_fn,
        NULL, // mem_ptr
        custom_malloc_fn,
        custom_free_fn
    );

    if (write_ptr != NULL) {
        // Test png_get_mem_ptr on write struct
        void *write_mem_ptr = png_get_mem_ptr(write_ptr);
        (void)write_mem_ptr;

        // Test png_set_mem_fn on write struct
        png_set_mem_fn(write_ptr, (void*)0x87654321, custom_malloc_fn, custom_free_fn);

        // Test png_malloc_warn on write struct
        png_alloc_size_t write_alloc_size = 0;
        if (Size >= 8) {
            // Safe read of 4 bytes starting at offset 4
            uint32_t val = 0;
            memcpy(&val, Data + 4, 4);
            write_alloc_size = val % 1024;
        } else {
            write_alloc_size = 64;
        }

        void *write_malloced_ptr = png_malloc_warn(write_ptr, write_alloc_size);
        if (write_malloced_ptr != NULL) {
            // Write some data
            size_t write_len = (Size > write_alloc_size) ? write_alloc_size : Size;
            if (write_len > 0) {
                memcpy(write_malloced_ptr, Data, write_len);
            }
            // Test png_free on write struct
            png_free(write_ptr, write_malloced_ptr);
        }

        // Cleanup write struct
        png_destroy_write_struct(&write_ptr, NULL);
    }

    return 0;
}