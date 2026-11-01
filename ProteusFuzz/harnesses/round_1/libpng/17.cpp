// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_header_ver at png.c:821:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstddef>
#include <cstring>

extern "C" int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Fuzzing target: libpng version and struct creation functions
    // These functions are mostly static or depend on version checks, 
    // but we exercise them with various inputs to ensure robustness.

    // 1. Test png_access_version_number (no args, always safe)
    png_uint_32 version_num = png_access_version_number();
    (void)version_num; // Use to avoid unused variable warning

    // 2. Test png_create_read_struct_2
    // We try different user_png_ver strings based on input data if available
    const char *user_ver = nullptr;
    if (Size > 0) {
        // Create a temporary buffer for user version string
        // Limit size to avoid excessive allocation
        size_t ver_len = (Size > 64) ? 64 : Size;
        char *ver_buf = new char[ver_len + 1];
        std::memcpy(ver_buf, Data, ver_len);
        ver_buf[ver_len] = '\0';
        user_ver = ver_buf;
        
        png_structp read_ptr = png_create_read_struct_2(
            user_ver,
            nullptr, // error_ptr
            nullptr, // error_fn
            nullptr, // warn_fn
            nullptr, // mem_ptr
            nullptr, // malloc_fn
            nullptr  // free_fn
        );
        
        if (read_ptr) {
            // Test version retrieval functions with valid read_ptr
            const char *header_ver = png_get_header_ver(read_ptr);
            const char *lib_ver = png_get_libpng_ver(read_ptr);
            const char *header_version_str = png_get_header_version(read_ptr);
            
            // Cleanup read struct
            png_destroy_read_struct(&read_ptr, nullptr, nullptr);
            
            (void)header_ver;
            (void)lib_ver;
            (void)header_version_str;
        }
        
        delete[] ver_buf;
    } else {
        // Test with NULL user version (should use default)
        png_structp read_ptr = png_create_read_struct_2(
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );
        
        if (read_ptr) {
            const char *header_ver = png_get_header_ver(read_ptr);
            const char *lib_ver = png_get_libpng_ver(read_ptr);
            const char *header_version_str = png_get_header_version(read_ptr);
            
            png_destroy_read_struct(&read_ptr, nullptr, nullptr);
            
            (void)header_ver;
            (void)lib_ver;
            (void)header_version_str;
        }
    }

    // 3. Test png_create_write_struct
    user_ver = nullptr;
    if (Size > 0) {
        size_t ver_len = (Size > 64) ? 64 : Size;
        char *ver_buf = new char[ver_len + 1];
        std::memcpy(ver_buf, Data, ver_len);
        ver_buf[ver_len] = '\0';
        user_ver = ver_buf;
        
        png_structp write_ptr = png_create_write_struct(
            user_ver,
            nullptr, // error_ptr
            nullptr, // error_fn
            nullptr  // warn_fn
        );
        
        if (write_ptr) {
            // Test version retrieval functions with valid write_ptr
            const char *header_ver = png_get_header_ver(write_ptr);
            const char *lib_ver = png_get_libpng_ver(write_ptr);
            const char *header_version_str = png_get_header_version(write_ptr);
            
            // Cleanup write struct
            png_destroy_write_struct(&write_ptr, nullptr);
            
            (void)header_ver;
            (void)lib_ver;
            (void)header_version_str;
        }
        
        delete[] ver_buf;
    } else {
        // Test with NULL user version
        png_structp write_ptr = png_create_write_struct(
            nullptr,
            nullptr,
            nullptr,
            nullptr
        );
        
        if (write_ptr) {
            const char *header_ver = png_get_header_ver(write_ptr);
            const char *lib_ver = png_get_libpng_ver(write_ptr);
            const char *header_version_str = png_get_header_version(write_ptr);
            
            png_destroy_write_struct(&write_ptr, nullptr);
            
            (void)header_ver;
            (void)lib_ver;
            (void)header_version_str;
        }
    }

    // 4. Test version functions with nullptr (they should handle it gracefully)
    // According to documentation, these functions ignore png_ptr
    const char *header_ver_null = png_get_header_ver(nullptr);
    const char *lib_ver_null = png_get_libpng_ver(nullptr);
    const char *header_version_str_null = png_get_header_version(nullptr);
    
    (void)header_ver_null;
    (void)lib_ver_null;
    (void)header_version_str_null;

    return 0;
}