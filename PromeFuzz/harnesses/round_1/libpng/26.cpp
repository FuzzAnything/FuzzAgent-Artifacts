// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct at pngread.c:24:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_set_error_fn at pngerror.c:709:1 in png.h
// png_warning at pngerror.c:186:1 in png.h
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
#include <cstring>
#include "code/png.h"

static void dummy_warning(png_structp png_ptr, png_const_charp warning_message) {
    // Dummy warning handler to avoid null pointer issues
    (void)png_ptr;
    (void)warning_message;
}

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    // Create dummy png_struct for functions that require it
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        return 0;
    }
    
    // Set up dummy error handling
    png_set_error_fn(png_ptr, nullptr, nullptr, dummy_warning);
    
    // Test png_warning with various inputs
    if (Size > 0) {
        // Use first byte to determine warning behavior
        uint8_t warning_type = Data[0] % 4;
        
        switch (warning_type) {
            case 0:
                png_warning(png_ptr, "Test warning message");
                break;
            case 1:
                png_warning(png_ptr, nullptr);
                break;
            case 2: {
                // Create warning from fuzzer data if available
                if (Size > 1) {
                    size_t msg_len = (Size - 1) < 100 ? (Size - 1) : 100;
                    char* warning_msg = new char[msg_len + 1];
                    memcpy(warning_msg, Data + 1, msg_len);
                    warning_msg[msg_len] = '\0';
                    png_warning(png_ptr, warning_msg);
                    delete[] warning_msg;
                }
                break;
            }
            case 3:
                // Empty warning
                png_warning(png_ptr, "");
                break;
        }
    }
    
    // Test png_get_copyright - can be called with valid or null pointer
    const char* copyright = png_get_copyright(png_ptr);
    (void)copyright;  // Use result to avoid unused variable warning
    
    // Also test with null pointer
    const char* copyright_null = png_get_copyright(nullptr);
    (void)copyright_null;
    
    // Test png_get_header_version
    const char* header_version = png_get_header_version(png_ptr);
    (void)header_version;
    
    const char* header_version_null = png_get_header_version(nullptr);
    (void)header_version_null;
    
    // Test png_get_header_ver
    const char* header_ver = png_get_header_ver(png_ptr);
    (void)header_ver;
    
    const char* header_ver_null = png_get_header_ver(nullptr);
    (void)header_ver_null;
    
    // Test png_access_version_number
    png_uint_32 version_number = png_access_version_number();
    (void)version_number;
    
    // Test png_get_libpng_ver with various inputs
    const char* libpng_ver = png_get_libpng_ver(png_ptr);
    (void)libpng_ver;
    
    const char* libpng_ver_null = png_get_libpng_ver(nullptr);
    (void)libpng_ver_null;
    
    // Test with different png_struct states by manipulating error handling
    if (Size > 0 && (Data[0] & 0x80)) {
        // Test with error handler set to null
        png_set_error_fn(png_ptr, nullptr, nullptr, nullptr);
        png_warning(png_ptr, "Warning with null handler");
    }
    
    // Clean up
    png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    
    return 0;
}