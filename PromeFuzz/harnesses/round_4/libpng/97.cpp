// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_set_read_fn at pngrio.c:86:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_info_init_3 at png.c:433:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

#include "png.h"

static void custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Custom error handler that does nothing
    (void)png_ptr;
    (void)error_msg;
}

static void custom_warn_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Custom warning handler that does nothing
    (void)png_ptr;
    (void)warning_msg;
}

static void* custom_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    // Custom malloc that uses standard malloc
    (void)png_ptr;
    return malloc(size);
}

static void custom_free_fn(png_structp png_ptr, void* ptr) {
    // Custom free that uses standard free
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Use fuzzer input to create variation in parameters
    const char* version_strings[] = {
        "1.6.39",
        "1.5.30",
        "invalid_version",
        NULL
    };
    
    int version_index = Size > 0 ? Data[0] % 4 : 0;
    const char* user_png_ver = version_strings[version_index];
    
    // Create dummy error pointer based on fuzzer data
    void* error_ptr = NULL;
    if (Size > 1) {
        error_ptr = (void*)(Data + 1);
    }
    
    // Test png_create_read_struct_2 with various configurations
    png_structp png_ptr = png_create_read_struct_2(
        user_png_ver,
        error_ptr,
        custom_error_fn,
        custom_warn_fn,
        NULL,  // Default memory pointer
        NULL,  // Default malloc
        NULL   // Default free
    );
    
    if (png_ptr == NULL) {
        // Try with custom memory handlers
        png_ptr = png_create_read_struct_2(
            user_png_ver,
            error_ptr,
            custom_error_fn,
            custom_warn_fn,
            (void*)Data,  // Custom memory pointer
            custom_malloc_fn,
            custom_free_fn
        );
    }
    
    if (png_ptr == NULL) {
        return 0;  // Early exit if structure creation failed
    }
    
    // Test png_create_info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    // Test png_info_init_3 (deprecated but still available)
    if (Size > 2 && info_ptr != NULL) {
        png_infopp info_pp = &info_ptr;
        // Use a reasonable size value from fuzzer input
        size_t struct_size = Data[2] % 256 + 1;  // Ensure non-zero
        png_info_init_3(info_pp, struct_size);
    }
    
    // Test png_free and png_free_default with various allocations
    if (Size > 3 && png_ptr != NULL) {
        size_t alloc_size = Data[3] % 1024 + 1;
        void* test_ptr = png_malloc(png_ptr, alloc_size);
        
        if (test_ptr != NULL) {
            // Test png_free
            png_free(png_ptr, test_ptr);
            
            // Test png_free_default (deprecated)
            test_ptr = png_malloc(png_ptr, alloc_size);
            if (test_ptr != NULL) {
                png_free_default(png_ptr, test_ptr);
            }
        }
    }
    
    // Test png_destroy_info_struct with various states
    if (png_ptr != NULL && info_ptr != NULL) {
        png_infopp info_ptr_ptr = &info_ptr;
        png_destroy_info_struct(png_ptr, info_ptr_ptr);
        
        // Double check pointer is set to NULL
        if (info_ptr != NULL) {
            // Force cleanup if not already done
            png_destroy_info_struct(png_ptr, &info_ptr);
        }
    }
    
    // Cleanup png_struct
    if (png_ptr != NULL) {
        // Create a dummy file to test file-based cleanup if needed
        FILE* dummy_file = fopen("./dummy_file", "wb");
        if (dummy_file != NULL) {
            if (Size > 0) {
                fwrite(Data, 1, Size > 100 ? 100 : Size, dummy_file);
            }
            fclose(dummy_file);
            
            // Set up read function (simplified)
            png_set_read_fn(png_ptr, NULL, NULL);
        }
        
        // Destroy the read structure
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }
    
    // Additional test: create and destroy multiple structures
    if (Size > 4) {
        png_structp png_ptr2 = png_create_read_struct_2(
            "1.6.39",
            NULL,
            NULL,
            NULL,
            NULL,
            NULL,
            NULL
        );
        
        if (png_ptr2 != NULL) {
            png_infop info_ptr2 = png_create_info_struct(png_ptr2);
            png_infop end_info_ptr2 = png_create_info_struct(png_ptr2);
            
            png_destroy_read_struct(&png_ptr2, &info_ptr2, &end_info_ptr2);
        }
    }
    
    return 0;
}