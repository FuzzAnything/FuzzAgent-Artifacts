// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_benign_error at pngerror.c:308:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    // Create a dummy png_struct pointer for functions that require it
    // These functions can accept NULL or any pointer since they ignore the parameter
    png_structp png_ptr = nullptr;
    
    // Test png_get_copyright - always returns static text
    const char* copyright = png_get_copyright(png_ptr);
    (void)copyright; // Use result to avoid unused variable warning
    
    // Test png_get_header_ver - returns header version string
    const char* header_ver = png_get_header_ver(png_ptr);
    (void)header_ver;
    
    // Test png_get_header_version - returns version string
    const char* header_version = png_get_header_version(png_ptr);
    (void)header_version;
    
    // Test png_get_libpng_ver - returns version string from build metadata
    const char* libpng_ver = png_get_libpng_ver(png_ptr);
    (void)libpng_ver;
    
    // Test png_access_version_number - returns compile-time version number
    png_uint_32 version_number = png_access_version_number();
    (void)version_number;
    
    // Test png_benign_error - requires a png_struct and warning message
    // Create a minimal png_struct for this function
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    
    if (png_ptr) {
        // Create a warning message from fuzzer input
        // Ensure null termination by allocating extra byte
        char* warning_message = static_cast<char*>(malloc(Size + 1));
        if (warning_message) {
            if (Size > 0) {
                memcpy(warning_message, Data, Size);
            }
            warning_message[Size] = '\0';
            
            // Call png_benign_error with the warning message
            png_benign_error(png_ptr, warning_message);
            
            free(warning_message);
        }
        
        // Clean up the png_struct
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }
    
    return 0;
}