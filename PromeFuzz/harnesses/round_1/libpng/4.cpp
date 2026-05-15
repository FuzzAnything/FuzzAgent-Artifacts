// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_longjmp_fn at pngerror.c:537:1 in png.h
// png_error at pngerror.c:50:1 in png.h
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
#include <setjmp.h>

#include "code/png.h"

// Custom error and warning handlers
static void custom_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    // Do nothing - just prevent default termination
}

static void custom_warning_fn(png_structp png_ptr, png_const_charp warning_msg) {
    // Do nothing
}

// Custom memory allocation functions for png_create_read_struct_2
static void* custom_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    return malloc(size);
}

static void custom_free_fn(png_structp png_ptr, void* ptr) {
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_4(const uint8_t* Data, size_t Size) {
    // Use first few bytes to determine test configuration
    if (Size < 1) return 0;
    
    uint8_t config = Data[0];
    const uint8_t* remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Create version string from fuzzer data (ensure null termination)
    char version[20] = "1.6.0";  // Default version
    if (remaining_size > 0) {
        size_t ver_len = remaining_data[0] % 19;
        if (ver_len > 0 && remaining_size > 1) {
            size_t copy_len = (ver_len < remaining_size - 1) ? ver_len : remaining_size - 1;
            memcpy(version, remaining_data + 1, copy_len);
            version[copy_len] = '\0';
        }
    }
    
    // Test different API functions based on configuration
    png_structp png_ptr = nullptr;
    void* error_ptr = nullptr;
    
    // Use fuzzer data to create different error pointers
    if (remaining_size > 10) {
        error_ptr = const_cast<void*>(static_cast<const void*>(remaining_data + 10));
    }
    
    // Test png_create_write_struct
    if ((config & 0x01) != 0) {
        png_ptr = png_create_write_struct(version, error_ptr, custom_error_fn, custom_warning_fn);
        if (png_ptr) {
            // Test png_set_error_fn and png_get_error_ptr
            void* custom_error_data = const_cast<void*>(static_cast<const void*>(Data));
            png_set_error_fn(png_ptr, custom_error_data, custom_error_fn, custom_warning_fn);
            void* retrieved_ptr = png_get_error_ptr(png_ptr);
            
            // Test png_error with setjmp protection
            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                // This will longjmp to the setjmp above
                png_error(png_ptr, "Fuzzer triggered error");
            }
            
            png_destroy_write_struct(&png_ptr, nullptr);
            png_ptr = nullptr;
        }
    }
    
    // Test png_create_read_struct
    if ((config & 0x02) != 0) {
        png_ptr = png_create_read_struct(version, error_ptr, custom_error_fn, custom_warning_fn);
        if (png_ptr) {
            // Test error handling
            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                png_error(png_ptr, "Fuzzer read error");
            }
            
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            png_ptr = nullptr;
        }
    }
    
    // Test png_create_read_struct_2 with custom memory handlers
    if ((config & 0x04) != 0) {
        void* mem_ptr = const_cast<void*>(static_cast<const void*>(Data));
        png_ptr = png_create_read_struct_2(version, error_ptr, custom_error_fn, 
                                          custom_warning_fn, mem_ptr, 
                                          custom_malloc_fn, custom_free_fn);
        if (png_ptr) {
            // Test error handling
            if (setjmp(png_jmpbuf(png_ptr)) == 0) {
                png_error(png_ptr, "Fuzzer read_struct_2 error");
            }
            
            png_destroy_read_struct(&png_ptr, nullptr, nullptr);
            png_ptr = nullptr;
        }
    }
    
    // Test combined usage
    if ((config & 0x08) != 0) {
        // Create write struct
        png_structp write_ptr = png_create_write_struct(version, error_ptr, 
                                                        custom_error_fn, custom_warning_fn);
        if (write_ptr) {
            // Create read struct
            png_structp read_ptr = png_create_read_struct(version, error_ptr,
                                                         custom_error_fn, custom_warning_fn);
            
            if (read_ptr) {
                // Set different error pointers
                void* write_error_data = const_cast<void*>(static_cast<const void*>(Data));
                void* read_error_data = const_cast<void*>(static_cast<const void*>(Data + (Size > 10 ? 10 : 0)));
                
                png_set_error_fn(write_ptr, write_error_data, custom_error_fn, custom_warning_fn);
                png_set_error_fn(read_ptr, read_error_data, custom_error_fn, custom_warning_fn);
                
                // Get and compare error pointers
                void* retrieved_write_ptr = png_get_error_ptr(write_ptr);
                void* retrieved_read_ptr = png_get_error_ptr(read_ptr);
                
                // Test error on write struct
                if (setjmp(png_jmpbuf(write_ptr)) == 0) {
                    png_error(write_ptr, "Combined test write error");
                }
                
                // Test error on read struct  
                if (setjmp(png_jmpbuf(read_ptr)) == 0) {
                    png_error(read_ptr, "Combined test read error");
                }
                
                png_destroy_write_struct(&write_ptr, nullptr);
                png_destroy_read_struct(&read_ptr, nullptr, nullptr);
            } else if (write_ptr) {
                png_destroy_write_struct(&write_ptr, nullptr);
            }
        }
    }
    
    return 0;
}