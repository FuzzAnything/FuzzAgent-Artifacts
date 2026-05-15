// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_info_init_3 at png.c:433:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
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

#include "png.h"

// Custom memory handlers for testing
static void* test_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void test_free(png_structp png_ptr, void* ptr) {
    (void)png_ptr;
    free(ptr);
}

// Error handlers
static void test_error(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void test_warning(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

extern "C" int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    // Use first byte to determine which functions to test
    if (Size == 0) return 0;
    
    uint8_t selector = Data[0];
    Data++;
    Size--;
    
    // Test png_create_read_struct_2 with various configurations
    png_structp png_ptr = nullptr;
    
    // Try different version strings based on fuzzer input
    const char* version_strings[] = {
        PNG_LIBPNG_VER_STRING,
        "1.6.0",
        "1.5.0",
        "1.0.0",
        ""
    };
    
    size_t version_idx = selector % (sizeof(version_strings) / sizeof(version_strings[0]));
    const char* user_png_ver = version_strings[version_idx];
    
    // Try different combinations of handlers
    if (selector & 0x01) {
        // Use custom memory handlers
        png_ptr = png_create_read_struct_2(
            user_png_ver,
            nullptr,  // error_ptr
            test_error,
            test_warning,
            nullptr,  // mem_ptr
            test_malloc,
            test_free
        );
    } else {
        // Use default handlers
        png_ptr = png_create_read_struct_2(
            user_png_ver,
            nullptr,  // error_ptr
            nullptr,  // error_fn
            nullptr,  // warn_fn
            nullptr,  // mem_ptr
            nullptr,  // malloc_fn
            nullptr   // free_fn
        );
    }
    
    if (png_ptr == nullptr) {
        // Early return if structure creation failed
        return 0;
    }
    
    // Test png_create_info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    // Test png_info_init_3 (deprecated but still part of API)
    if (info_ptr != nullptr && (selector & 0x02)) {
        png_infopp info_ptr_ptr = &info_ptr;
        // Use a reasonable default size since we can't use sizeof on incomplete type
        size_t info_size = 256;  // Reasonable default
        
        // Try different sizes based on fuzzer input
        if (Size >= sizeof(size_t)) {
            size_t custom_size;
            memcpy(&custom_size, Data, sizeof(size_t));
            Data += sizeof(size_t);
            Size -= sizeof(size_t);
            
            // Ensure size is reasonable
            if (custom_size > 0 && custom_size < 1024 * 1024) {
                info_size = custom_size;
            }
        }
        
        png_info_init_3(info_ptr_ptr, info_size);
    }
    
    // Test png_destroy_info_struct
    if (info_ptr != nullptr) {
        png_infopp info_ptr_ptr = &info_ptr;
        png_destroy_info_struct(png_ptr, info_ptr_ptr);
        
        // info_ptr should now be nullptr
        if (info_ptr != nullptr) {
            // Something went wrong, but continue anyway
        }
    }
    
    // Test memory allocation and freeing functions
    if (selector & 0x04) {
        // Allocate some memory using libpng's allocator
        size_t alloc_size = 64;
        if (Size >= 1) {
            alloc_size = Data[0] % 256;
            if (alloc_size == 0) alloc_size = 1;
        }
        
        void* allocated_mem = png_malloc(png_ptr, alloc_size);
        
        if (allocated_mem != nullptr) {
            // Test png_free
            png_free(png_ptr, allocated_mem);
            
            // Test png_free_default (deprecated)
            if (selector & 0x08) {
                // Allocate again to test png_free_default
                allocated_mem = png_malloc(png_ptr, alloc_size);
                if (allocated_mem != nullptr) {
                    png_free_default(png_ptr, allocated_mem);
                }
            }
        }
    }
    
    // Clean up png_struct using png_destroy_read_struct
    // This is the proper way to clean up, even though it's not in our API list
    if (png_ptr != nullptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
    
    return 0;
}