// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_set_eXIf_1 at pngset.c:319:1 in png.h
// png_get_eXIf_1 at pngget.c:892:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_free_data at png.c:480:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_data_freer at png.c:461:1 in png.h
// png_set_invalid at pngset.c:1927:1 in png.h
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
#include <cstddef>
#include <cstdlib>
#include <cstring>

#include "png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - suppress error output during fuzzing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing - suppress warning output during fuzzing
}

static png_voidp dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Create dummy PNG structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 
                                                 nullptr, 
                                                 dummy_error_fn, 
                                                 dummy_warning_fn);
    if (!png_ptr) {
        return 0;
    }
    
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
        return 0;
    }
    
    // Set custom allocator to test png_free with custom free_fn
    png_set_mem_fn(png_ptr, nullptr, dummy_malloc_fn, dummy_free_fn);
    
    // Test png_free with various inputs
    if (Size > 0) {
        // Allocate some memory using png_malloc
        size_t alloc_size = Size % 1024; // Limit allocation size
        void* allocated_ptr = png_malloc(png_ptr, alloc_size);
        if (allocated_ptr) {
            // Copy some fuzz data into the allocated memory
            size_t copy_size = (alloc_size < Size) ? alloc_size : Size;
            memcpy(allocated_ptr, Data, copy_size);
            
            // Test png_free with the allocated pointer
            png_free(png_ptr, allocated_ptr);
            
            // Test png_free with null pointer (should be safe)
            png_free(png_ptr, nullptr);
        }
    }
    
    // Test png_data_freer with different masks
    if (Size > 4) {
        // Use first 4 bytes as mask
        png_uint_32 mask = 0;
        if (Size >= 4) {
            memcpy(&mask, Data, 4);
        }
        
        // Test with PNG_DESTROY_WILL_FREE_DATA
        png_data_freer(png_ptr, info_ptr, PNG_DESTROY_WILL_FREE_DATA, mask);
        
        // Test with PNG_USER_WILL_FREE_DATA
        png_data_freer(png_ptr, info_ptr, PNG_USER_WILL_FREE_DATA, mask);
        
        // Test with arbitrary freer value
        int freer = (Size > 8) ? static_cast<int>(Data[4]) : 0;
        png_data_freer(png_ptr, info_ptr, freer, mask);
    }
    
    // Test png_set_invalid with various masks
    if (Size > 4) {
        png_uint_32 mask = 0;
        if (Size >= 8) {
            // Use bytes 4-7 as mask for set_invalid
            memcpy(&mask, Data + 4, 4);
        }
        png_set_invalid(png_ptr, info_ptr, static_cast<int>(mask));
    }
    
    // Test png_set_eXIf_1 and png_get_eXIf_1
    if (Size > 8) {
        // Use part of the fuzz data as eXIf data
        size_t exif_size = Size % 256; // Limit exif size
        if (exif_size > 0 && exif_size <= Size) {
            png_byte* exif_data = static_cast<png_byte*>(png_malloc(png_ptr, exif_size));
            if (exif_data) {
                memcpy(exif_data, Data, exif_size);
                
                // Set eXIf data
                png_set_eXIf_1(png_ptr, info_ptr, static_cast<png_uint_32>(exif_size), exif_data);
                
                // Get eXIf data back
                png_uint_32 num_exif = 0;
                png_byte* retrieved_exif = nullptr;
                png_uint_32 ret = png_get_eXIf_1(png_ptr, info_ptr, &num_exif, &retrieved_exif);
                
                // Free the allocated eXIf data
                png_free(png_ptr, exif_data);
            }
        }
    }
    
    // Test png_free_data with various masks and num values
    if (Size > 8) {
        png_uint_32 free_mask = 0;
        int free_num = -1;
        
        if (Size >= 12) {
            // Use bytes 8-11 as free mask
            memcpy(&free_mask, Data + 8, 4);
            
            // Use next byte as free_num
            free_num = static_cast<int>(Data[12]);
        }
        
        // Test with different num values
        png_free_data(png_ptr, info_ptr, free_mask, free_num);
        png_free_data(png_ptr, info_ptr, free_mask, -1); // Free all
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1); // Free everything
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    
    return 0;
}