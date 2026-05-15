// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct at pngread.c:24:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_error at pngerror.c:50:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
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
#include <cstring>
#include <csetjmp>

static jmp_buf png_jmpbuf;

static void custom_png_error(png_structp png_ptr, png_const_charp error_msg) {
    // Custom error handler that uses longjmp instead of abort
    longjmp(png_jmpbuf, 1);
}

extern "C" int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    // Initialize png_struct for functions that require it
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    
    if (png_ptr) {
        // Test functions that accept png_ptr
        const char* header_ver = png_get_header_version(png_ptr);
        const char* header_ver2 = png_get_header_ver(png_ptr);
        const char* libpng_ver = png_get_libpng_ver(png_ptr);
        const char* copyright = png_get_copyright(png_ptr);
        
        // Also test with NULL pointer
        const char* header_ver_null = png_get_header_version(nullptr);
        const char* header_ver2_null = png_get_header_ver(nullptr);
        const char* libpng_ver_null = png_get_libpng_ver(nullptr);
        const char* copyright_null = png_get_copyright(nullptr);
        
        // Test png_error with custom error handler
        png_set_error_fn(png_ptr, nullptr, custom_png_error, nullptr);
        
        if (setjmp(png_jmpbuf) == 0) {
            // Create error message from fuzzer input
            char error_msg[256];
            size_t copy_size = Size < 255 ? Size : 255;
            if (copy_size > 0) {
                memcpy(error_msg, Data, copy_size);
                error_msg[copy_size] = '\0';
                png_error(png_ptr, error_msg);
            } else {
                png_error(png_ptr, "Fuzzer-generated error");
            }
        }
        
        // Clean up
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
    
    // Test function that doesn't require png_ptr
    png_uint_32 version_num = png_access_version_number();
    
    return 0;
}