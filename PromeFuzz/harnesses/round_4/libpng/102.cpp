// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_free_default at pngmem.c:247:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_sig_cmp at png.c:80:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_info_init_3 at png.c:433:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "png.h"

// Custom memory handlers
static void* custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void custom_free(png_structp png_ptr, void* ptr) {
    (void)png_ptr;
    free(ptr);
}

// Error handlers
static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr;
    (void)error_msg;
    // Do nothing - just prevent default error handling
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
    // Do nothing
}

extern "C" int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine test mode
    uint8_t mode = Data[0];
    const uint8_t* png_data = Data + 1;
    size_t png_size = Size - 1;
    
    // Test png_create_read_struct_2 with various configurations
    png_structp png_ptr = nullptr;
    
    if (mode & 0x01) {
        // Use custom memory handlers
        png_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING,
            nullptr, error_handler, warning_handler,
            nullptr, custom_malloc, custom_free
        );
    } else {
        // Use default memory handlers
        png_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING,
            nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr
        );
    }
    
    if (!png_ptr) {
        return 0;
    }
    
    // Test png_create_info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    // Test png_info_init_3 (deprecated but we test it anyway)
    if (mode & 0x02) {
        png_infopp info_ptr_ptr = (png_infopp)malloc(sizeof(png_infop));
        if (info_ptr_ptr) {
            *info_ptr_ptr = nullptr;
            // Use a reasonable size for png_info structure
            png_info_init_3(info_ptr_ptr, 1024);
            
            // Clean up the allocated info struct
            if (*info_ptr_ptr) {
                // Use appropriate free function based on mode
                if (mode & 0x04) {
                    png_free_default(png_ptr, *info_ptr_ptr);
                } else {
                    png_free(png_ptr, *info_ptr_ptr);
                }
            }
            free(info_ptr_ptr);
        }
    }
    
    // Test png_free with some dummy allocations
    if (mode & 0x08) {
        void* test_ptr = png_malloc(png_ptr, 100);
        if (test_ptr) {
            png_free(png_ptr, test_ptr);
        }
        
        // Test png_free_default if we're using default allocator
        if (!(mode & 0x01)) {
            test_ptr = malloc(50);
            if (test_ptr) {
                png_free_default(png_ptr, test_ptr);
            }
        }
    }
    
    // Test png_destroy_info_struct
    if (info_ptr) {
        png_destroy_info_struct(png_ptr, &info_ptr);
        // info_ptr should now be nullptr
    }
    
    // Clean up png_struct
    if (png_ptr) {
        // Use appropriate cleanup based on allocator type
        if (mode & 0x01) {
            // For custom allocator, we need to free the structure manually
            // or use png_destroy_read_struct which we don't have in our API list
            // We'll use png_free since we used custom_malloc
            png_free(png_ptr, png_ptr);
        } else {
            // For default allocator, png_destroy_read_struct would normally be used
            // but since we don't have it, we'll use free directly
            free(png_ptr);
        }
    }
    
    // Try to create a dummy PNG file and read it if we have enough data
    if (png_size > 8) {
        // Check PNG signature
        bool is_png = png_sig_cmp((png_bytep)png_data, 0, 8) == 0;
        
        if (!is_png && (mode & 0x10)) {
            // Try to create structures anyway to test error paths
            png_structp png_ptr2 = png_create_read_struct_2(
                PNG_LIBPNG_VER_STRING,
                nullptr, nullptr, nullptr,
                nullptr, nullptr, nullptr
            );
            
            if (png_ptr2) {
                png_infop info_ptr2 = png_create_info_struct(png_ptr2);
                if (info_ptr2) {
                    png_destroy_info_struct(png_ptr2, &info_ptr2);
                }
                free(png_ptr2);
            }
        }
    }
    
    return 0;
}