// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
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

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {}
static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {}

extern "C" int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Create dummy PNG structure first
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, 
                                                  dummy_error_fn, dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }

    // Test png_create_info_struct
    png_infop info_ptr = png_create_info_struct(png_ptr);
    
    if (info_ptr) {
        // Test png_data_freer with different modes
        if (Size >= 4) {
            png_uint_32 mask = 0;
            // Use up to 4 bytes for mask
            size_t mask_size = Size < 4 ? Size : 4;
            memcpy(&mask, Data, mask_size);
            
            // Use both freer modes
            png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, mask);
            png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, mask);
        }
        
        // Test png_set_invalid with different masks
        if (Size >= 8) {
            int mask = 0;
            // Use up to 4 bytes for mask
            size_t mask_size = (Size - 4) < 4 ? (Size - 4) : 4;
            memcpy(&mask, Data + 4, mask_size);
            png_set_invalid(png_ptr, info_ptr, mask);
        }
        
        // Test png_free_data with different masks and nums
        if (Size >= 12) {
            png_uint_32 free_mask = 0;
            int num = -1;
            
            // Use up to 4 bytes for free_mask
            size_t mask_size = (Size - 8) < 4 ? (Size - 8) : 4;
            memcpy(&free_mask, Data + 8, mask_size);
            
            if (Size >= 16) {
                // Use up to 4 bytes for num
                size_t num_size = (Size - 12) < 4 ? (Size - 12) : 4;
                memcpy(&num, Data + 12, num_size);
            }
            
            png_free_data(png_ptr, info_ptr, free_mask, num);
        }
        
        // Test png_free with some allocated memory
        if (Size >= 20) {
            size_t alloc_size = 0;
            // Use up to 4 bytes for alloc_size, limit to reasonable size
            size_t size_size = (Size - 16) < 4 ? (Size - 16) : 4;
            memcpy(&alloc_size, Data + 16, size_size);
            alloc_size = alloc_size % 1024;  // Limit to 1KB
            
            void* mem = png_malloc(png_ptr, alloc_size);
            if (mem) {
                png_free(png_ptr, mem);
            }
        }
        
        // Test png_destroy_info_struct
        png_destroy_info_struct(png_ptr, &info_ptr);
        
        // Create another info struct to test double-free protection
        png_infop info_ptr2 = png_create_info_struct(png_ptr);
        if (info_ptr2) {
            png_destroy_info_struct(png_ptr, &info_ptr2);
            // Should be safe to call again
            png_destroy_info_struct(png_ptr, &info_ptr2);
        }
    }
    
    // Clean up PNG structure
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}