// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_info_init_3 at png.c:433:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_destroy_info_struct at png.c:396:1 in png.h
// png_free_default at pngmem.c:247:1 in png.h
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

#include "code/png.h"

static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing
}

static void dummy_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

static void* dummy_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

static void dummy_free_fn(png_structp png_ptr, void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to decide which function to test
    uint8_t selector = Data[0];
    Data++;
    Size--;

    // Create a dummy png_struct for testing
    png_structp png_ptr = png_create_read_struct(
        PNG_LIBPNG_VER_STRING, 
        nullptr, 
        dummy_error_fn, 
        dummy_warning_fn
    );
    
    if (!png_ptr) {
        return 0;
    }

    // Set custom allocator to test png_free
    png_set_mem_fn(png_ptr, nullptr, dummy_malloc_fn, dummy_free_fn);

    switch (selector % 6) {
        case 0: {
            // Test png_info_init_3
            png_infopp info_ptr_ptr = (png_infopp)malloc(sizeof(png_infop));
            if (info_ptr_ptr) {
                *info_ptr_ptr = nullptr;
                // Use a reasonable size for png_info struct
                // The exact size isn't needed since the function handles mismatches
                size_t struct_size = 1024; // Arbitrary size
                if (Size > 0) {
                    // Use fuzz data to modify struct size
                    struct_size = (Data[0] % 2048) + 1;
                }
                png_info_init_3(info_ptr_ptr, struct_size);
                if (*info_ptr_ptr) {
                    free(*info_ptr_ptr);
                }
                free(info_ptr_ptr);
            }
            break;
        }
        
        case 1: {
            // Test png_create_info_struct
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr) {
                // We cannot access internal members since png_info is opaque
                // Just create and destroy the struct
                png_destroy_info_struct(png_ptr, &info_ptr);
            }
            break;
        }
        
        case 2: {
            // Test png_free_default with various allocations
            if (Size > 0) {
                size_t alloc_size = (Data[0] % 1024) + 1;
                void* ptr = malloc(alloc_size);
                if (ptr) {
                    png_free_default(png_ptr, ptr);
                }
            }
            break;
        }
        
        case 3: {
            // Test png_free with png_malloc allocations
            if (Size > 0) {
                size_t alloc_size = (Data[0] % 1024) + 1;
                void* ptr = png_malloc(png_ptr, alloc_size);
                if (ptr) {
                    png_free(png_ptr, ptr);
                }
            }
            break;
        }
        
        case 4: {
            // Test png_destroy_read_struct with various states
            png_infop info_ptr = nullptr;
            png_infop end_info_ptr = nullptr;
            
            // Create info structs if we have enough data
            if (Size > 1) {
                info_ptr = png_create_info_struct(png_ptr);
                if (Size > 2) {
                    end_info_ptr = png_create_info_struct(png_ptr);
                }
            }
            
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info_ptr);
            
            // Recreate png_ptr for remaining tests
            png_ptr = png_create_read_struct(
                PNG_LIBPNG_VER_STRING, 
                nullptr, 
                dummy_error_fn, 
                dummy_warning_fn
            );
            if (!png_ptr) {
                return 0;
            }
            png_set_mem_fn(png_ptr, nullptr, dummy_malloc_fn, dummy_free_fn);
            break;
        }
        
        case 5: {
            // Test png_destroy_info_struct
            png_infop info_ptr = png_create_info_struct(png_ptr);
            if (info_ptr) {
                png_destroy_info_struct(png_ptr, &info_ptr);
            }
            break;
        }
    }

    // Cleanup
    if (png_ptr) {
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }

    return 0;
}