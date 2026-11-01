// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
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

extern "C" int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Test 1: png_create_write_struct
    // We use a subset of input data to simulate a version string or just use NULL
    png_structp write_ptr = NULL;
    const char *user_ver = nullptr;
    
    // Attempt to create write struct with NULL version (common case)
    write_ptr = png_create_write_struct(nullptr, nullptr, nullptr, nullptr);
    if (write_ptr != nullptr) {
        // Call version functions with the valid pointer
        const char *header_ver = png_get_header_ver(write_ptr);
        const char *copyright = png_get_copyright(write_ptr);
        const char *header_version = png_get_header_version(write_ptr);
        const char *libpng_ver = png_get_libpng_ver(write_ptr);
        
        // Just ensure they return non-null (they should always return static strings)
        if (header_ver) (void)*header_ver;
        if (copyright) (void)*copyright;
        if (header_version) (void)*header_version;
        if (libpng_ver) (void)*libpng_ver;
        
        // Cleanup
        png_destroy_write_struct(&write_ptr, nullptr);
    }

    // Test 2: png_create_read_struct_2
    png_structp read_ptr = NULL;
    
    // Try creating read struct with NULL custom handlers (default behavior)
    read_ptr = png_create_read_struct_2(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
    
    if (read_ptr != nullptr) {
        // Call version functions with the valid read pointer
        const char *header_ver_r = png_get_header_ver(read_ptr);
        const char *copyright_r = png_get_copyright(read_ptr);
        const char *header_version_r = png_get_header_version(read_ptr);
        const char *libpng_ver_r = png_get_libpng_ver(read_ptr);
        
        if (header_ver_r) (void)*header_ver_r;
        if (copyright_r) (void)*copyright_r;
        if (header_version_r) (void)*header_version_r;
        if (libpng_ver_r) (void)*libpng_ver_r;
        
        // Cleanup
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
    }

    // Test 3: Call version functions with NULL pointer (they should handle it gracefully or ignore it)
    // According to docs, these ignore the png_ptr argument, but passing NULL is safe to test.
    const char *header_ver_null = png_get_header_ver(nullptr);
    const char *copyright_null = png_get_copyright(nullptr);
    const char *header_version_null = png_get_header_version(nullptr);
    const char *libpng_ver_null = png_get_libpng_ver(nullptr);

    if (header_ver_null) (void)*header_ver_null;
    if (copyright_null) (void)*copyright_null;
    if (header_version_null) (void)*header_version_null;
    if (libpng_ver_null) (void)*libpng_ver_null;

    return 0;
}