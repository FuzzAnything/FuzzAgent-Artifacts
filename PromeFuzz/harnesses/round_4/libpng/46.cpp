// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_warning at pngerror.c:186:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_get_error_ptr at pngerror.c:730:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
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
#include <csetjmp>
#include "png.h"

// Custom error and warning handlers
static void custom_error_fn(png_structp png_ptr, const char* error_msg) {
    // Don't call abort() - just longjmp back to the setjmp point
    jmp_buf* buf = (jmp_buf*)png_get_error_ptr(png_ptr);
    if (buf) {
        longjmp(*buf, 1);
    }
}

static void custom_warning_fn(png_structp png_ptr, const char* warning_msg) {
    // Non-fatal warning - just ignore in fuzzing context
    (void)png_ptr;
    (void)warning_msg;
}

// Custom memory handlers
static void* custom_malloc_fn(png_structp png_ptr, png_alloc_size_t size) {
    (void)png_ptr;
    return malloc(size);
}

static void custom_free_fn(png_structp png_ptr, void* ptr) {
    (void)png_ptr;
    free(ptr);
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine which API functions to test
    uint8_t selector = Data[0];
    Data++;
    Size--;
    
    // Prepare jump buffer for error recovery
    jmp_buf jump_buffer;
    
    // Test png_create_read_struct_2
    if (selector & 0x01) {
        if (setjmp(jump_buffer) == 0) {
            png_structp read_ptr = png_create_read_struct_2(
                PNG_LIBPNG_VER_STRING,
                &jump_buffer,
                custom_error_fn,
                custom_warning_fn,
                NULL,
                custom_malloc_fn,
                custom_free_fn
            );
            
            if (read_ptr) {
                // Test png_set_error_fn
                png_set_error_fn(read_ptr, &jump_buffer, custom_error_fn, custom_warning_fn);
                
                // Test png_get_error_ptr
                void* error_ptr = png_get_error_ptr(read_ptr);
                if (error_ptr != &jump_buffer) {
                    // Error pointer mismatch
                }
                
                // Test png_error if we have enough data
                if (Size > 0) {
                    // Create a temporary error message from fuzz data
                    size_t msg_len = Size > 255 ? 255 : Size;
                    char* error_msg = (char*)malloc(msg_len + 1);
                    if (error_msg) {
                        memcpy(error_msg, Data, msg_len);
                        error_msg[msg_len] = '\0';
                        png_error(read_ptr, error_msg);
                        free(error_msg);
                    }
                }
                
                // Cleanup
                png_destroy_read_struct(&read_ptr, NULL, NULL);
            }
        }
    }
    
    // Test png_create_write_struct
    if (selector & 0x02) {
        if (setjmp(jump_buffer) == 0) {
            png_structp write_ptr = png_create_write_struct(
                PNG_LIBPNG_VER_STRING,
                &jump_buffer,
                custom_error_fn,
                custom_warning_fn
            );
            
            if (write_ptr) {
                // Test png_warning if we have enough data
                if (Size > 0) {
                    size_t msg_len = Size > 255 ? 255 : Size;
                    char* warning_msg = (char*)malloc(msg_len + 1);
                    if (warning_msg) {
                        memcpy(warning_msg, Data, msg_len);
                        warning_msg[msg_len] = '\0';
                        png_warning(write_ptr, warning_msg);
                        free(warning_msg);
                    }
                }
                
                // Cleanup
                png_destroy_write_struct(&write_ptr, NULL);
            }
        }
    }
    
    // Test png_set_error_fn with different configurations
    if (selector & 0x04) {
        if (setjmp(jump_buffer) == 0) {
            png_structp read_ptr = png_create_read_struct_2(
                PNG_LIBPNG_VER_STRING,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                NULL
            );
            
            if (read_ptr) {
                // Set custom error handlers
                png_set_error_fn(read_ptr, &jump_buffer, custom_error_fn, custom_warning_fn);
                
                // Verify error pointer
                void* error_ptr = png_get_error_ptr(read_ptr);
                if (error_ptr != &jump_buffer) {
                    // Error pointer mismatch
                }
                
                // Cleanup
                png_destroy_read_struct(&read_ptr, NULL, NULL);
            }
        }
    }
    
    // Test error/warning functions with various message lengths
    if (selector & 0x08) {
        if (setjmp(jump_buffer) == 0) {
            png_structp write_ptr = png_create_write_struct(
                PNG_LIBPNG_VER_STRING,
                &jump_buffer,
                custom_error_fn,
                custom_warning_fn
            );
            
            if (write_ptr) {
                // Test with different message scenarios
                if (Size > 0) {
                    // Test with empty string
                    png_warning(write_ptr, "");
                    
                    // Test with short message from fuzz data
                    if (Size >= 10) {
                        char short_msg[11];
                        memcpy(short_msg, Data, 10);
                        short_msg[10] = '\0';
                        png_warning(write_ptr, short_msg);
                    }
                    
                    // Test with longer message
                    if (Size >= 100) {
                        char long_msg[101];
                        memcpy(long_msg, Data, 100);
                        long_msg[100] = '\0';
                        png_warning(write_ptr, long_msg);
                    }
                }
                
                // Cleanup
                png_destroy_write_struct(&write_ptr, NULL);
            }
        }
    }
    
    return 0;
}