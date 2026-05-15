// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_info_struct at png.c:362:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct_2 at pngwrite.c:567:1 in png.h
// png_create_info_struct at png.c:362:1 in png.h
// png_malloc at pngmem.c:172:1 in png.h
// png_free at pngmem.c:234:1 in png.h
// png_set_mem_fn at pngmem.c:263:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "code/png.h"
#include <cstdint>
#include <cstdlib>
#include <cstring>

// Custom memory handlers for testing
static void* custom_malloc(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr; // Unused parameter
    return malloc(size);
}

static void custom_free(png_structp png_ptr, void* ptr) {
    (void)png_ptr; // Unused parameter
    free(ptr);
}

// Error and warning handlers
static void error_handler(png_structp png_ptr, png_const_charp error_msg) {
    (void)png_ptr; // Unused parameter
    (void)error_msg; // Unused parameter
}

static void warning_handler(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr; // Unused parameter
    (void)warning_msg; // Unused parameter
}

extern "C" int LLVMFuzzerTestOneInput_322(const uint8_t* Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    Data++;
    Size--;

    // Test png_create_write_struct_2
    if ((selector & 0x01) != 0) {
        png_structp write_ptr = png_create_write_struct_2(
            PNG_LIBPNG_VER_STRING,
            nullptr,
            error_handler,
            warning_handler,
            nullptr,
            custom_malloc,
            custom_free
        );
        
        if (write_ptr != nullptr) {
            // Test png_create_info_struct for write
            png_infop write_info = png_create_info_struct(write_ptr);
            
            if (write_info != nullptr) {
                // Test png_malloc and png_free
                if (Size >= sizeof(png_alloc_size_t)) {
                    png_alloc_size_t alloc_size;
                    memcpy(&alloc_size, Data, sizeof(png_alloc_size_t));
                    alloc_size = alloc_size % 4096; // Limit allocation size
                    
                    void* allocated_mem = png_malloc(write_ptr, alloc_size);
                    if (allocated_mem != nullptr) {
                        png_free(write_ptr, allocated_mem);
                    }
                }
                
                // Test png_set_mem_fn
                png_set_mem_fn(write_ptr, nullptr, custom_malloc, custom_free);
                
                // Cleanup write structures
                png_destroy_write_struct(&write_ptr, &write_info);
            } else {
                png_destroy_write_struct(&write_ptr, nullptr);
            }
        }
    }

    // Test png_create_read_struct_2
    if ((selector & 0x02) != 0) {
        png_structp read_ptr = png_create_read_struct_2(
            PNG_LIBPNG_VER_STRING,
            nullptr,
            error_handler,
            warning_handler,
            nullptr,
            custom_malloc,
            custom_free
        );
        
        if (read_ptr != nullptr) {
            // Test png_create_info_struct for read
            png_infop read_info = png_create_info_struct(read_ptr);
            
            if (read_info != nullptr) {
                // Test png_set_mem_fn
                png_set_mem_fn(read_ptr, nullptr, custom_malloc, custom_free);
                
                // Test png_malloc with different sizes
                if (Size >= 2 * sizeof(png_alloc_size_t)) {
                    png_alloc_size_t alloc_size1, alloc_size2;
                    memcpy(&alloc_size1, Data, sizeof(png_alloc_size_t));
                    memcpy(&alloc_size2, Data + sizeof(png_alloc_size_t), sizeof(png_alloc_size_t));
                    
                    alloc_size1 = alloc_size1 % 2048;
                    alloc_size2 = alloc_size2 % 2048;
                    
                    void* mem1 = png_malloc(read_ptr, alloc_size1);
                    void* mem2 = png_malloc(read_ptr, alloc_size2);
                    
                    if (mem1 != nullptr) {
                        png_free(read_ptr, mem1);
                    }
                    if (mem2 != nullptr) {
                        png_free(read_ptr, mem2);
                    }
                }
                
                // Cleanup read structures
                png_destroy_read_struct(&read_ptr, &read_info, nullptr);
            } else {
                png_destroy_read_struct(&read_ptr, nullptr, nullptr);
            }
        }
    }

    // Test standalone png_free with null pointers
    if ((selector & 0x04) != 0) {
        png_free(nullptr, nullptr);
        
        // Test png_free with non-null pointer but null png_ptr
        if (Size > 0) {
            void* dummy = malloc(16);
            if (dummy != nullptr) {
                png_free(nullptr, dummy);
            }
        }
    }

    // Test png_malloc with edge cases
    if ((selector & 0x08) != 0) {
        // Create a temporary structure for testing
        png_structp temp_ptr = png_create_write_struct_2(
            PNG_LIBPNG_VER_STRING,
            nullptr,
            error_handler,
            warning_handler,
            nullptr,
            custom_malloc,
            custom_free
        );
        
        if (temp_ptr != nullptr) {
            // Test with size 0
            void* zero_alloc = png_malloc(temp_ptr, 0);
            if (zero_alloc != nullptr) {
                png_free(temp_ptr, zero_alloc);
            }
            
            // Test with reasonable sizes
            for (int i = 0; i < 5 && Size > i * sizeof(png_alloc_size_t); i++) {
                png_alloc_size_t size;
                memcpy(&size, Data + i * sizeof(png_alloc_size_t), sizeof(png_alloc_size_t));
                size = size % 1024;
                
                void* mem = png_malloc(temp_ptr, size);
                if (mem != nullptr) {
                    png_free(temp_ptr, mem);
                }
            }
            
            png_destroy_write_struct(&temp_ptr, nullptr);
        }
    }

    return 0;
}