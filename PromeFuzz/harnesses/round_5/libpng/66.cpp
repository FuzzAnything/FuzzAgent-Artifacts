// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_access_version_number at png.c:963:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
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

#include "code/png.h"

extern "C" int LLVMFuzzerTestOneInput_66(const uint8_t *Data, size_t Size) {
    // Test png_access_version_number - no parameters needed
    png_uint_32 version_number = png_access_version_number();
    (void)version_number; // Use result to avoid unused variable warning

    // Test functions that accept NULL pointer
    const char* libpng_ver = png_get_libpng_ver(NULL);
    (void)libpng_ver;
    
    const char* header_ver = png_get_header_ver(NULL);
    (void)header_ver;
    
    const char* copyright = png_get_copyright(NULL);
    (void)copyright;
    
    const char* header_version = png_get_header_version(NULL);
    (void)header_version;

    // For png_benign_error, we need to create a valid png_struct
    // Use png_create_read_struct to get a valid pointer
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    
    if (png_ptr) {
        // Test png_benign_error with various inputs
        if (Size > 0) {
            // Create a null-terminated string from the fuzzer input
            // Limit the size to avoid excessive memory usage
            size_t str_len = Size > 1024 ? 1024 : Size;
            char* warning_message = static_cast<char*>(std::malloc(str_len + 1));
            if (warning_message) {
                std::memcpy(warning_message, Data, str_len);
                warning_message[str_len] = '\0';
                
                // Call png_benign_error with the fuzzer-provided message
                png_benign_error(png_ptr, warning_message);
                
                std::free(warning_message);
            }
        } else {
            // Test with empty string
            png_benign_error(png_ptr, "");
        }
        
        // Test with some fixed strings to explore different paths
        png_benign_error(png_ptr, "test warning");
        png_benign_error(png_ptr, "another warning message");
        
        // Test the other functions with the valid pointer as well
        libpng_ver = png_get_libpng_ver(png_ptr);
        (void)libpng_ver;
        
        header_ver = png_get_header_ver(png_ptr);
        (void)header_ver;
        
        copyright = png_get_copyright(png_ptr);
        (void)copyright;
        
        header_version = png_get_header_version(png_ptr);
        (void)header_version;
        
        // Clean up
        png_destroy_read_struct(&png_ptr, NULL, NULL);
    }

    return 0;
}