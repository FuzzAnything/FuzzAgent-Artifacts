// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_access_version_number at png.c:963:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
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
#include <png.h>
#include <cstdint>
#include <cstring>
#include <cstdio>

// Static helper to create a valid png_struct for testing
static png_structp create_test_png_struct(const char* user_png_ver) {
    // Create a read struct with standard error handlers (NULL for default)
    return png_create_read_struct_2(user_png_ver, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

extern "C" int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Test png_access_version_number (no arguments)
    png_uint_32 ver_num = png_access_version_number();
    (void)ver_num; // Use the value to avoid unused variable warnings

    // 2. Test version string functions with nullptr (they ignore the pointer)
    const char *header_ver = png_get_header_ver(nullptr);
    const char *header_version = png_get_header_version(nullptr);
    const char *libpng_ver = png_get_libpng_ver(nullptr);
    const char *copyright = png_get_copyright(nullptr);

    // Basic sanity checks (optional, just to ensure pointers are not null if expected)
    if (header_ver) { (void)strlen(header_ver); }
    if (header_version) { (void)strlen(header_version); }
    if (libpng_ver) { (void)strlen(libpng_ver); }
    if (copyright) { (void)strlen(copyright); }

    // 3. Test png_create_read_struct_2 with various inputs
    // Case A: Valid version string (using the string from the fuzzer input if possible, or default)
    // We try to interpret the input data as a version string for the first few bytes
    char version_buf[64] = {0};
    size_t copy_len = Size < sizeof(version_buf) - 1 ? Size : sizeof(version_buf) - 1;
    memcpy(version_buf, Data, copy_len);
    // Ensure null termination
    version_buf[copy_len] = '\0';

    png_structp png_ptr = create_test_png_struct(version_buf);

    if (png_ptr) {
        // Test version functions with a valid png_ptr
        const char *v1 = png_get_header_ver(png_ptr);
        const char *v2 = png_get_header_version(png_ptr);
        const char *v3 = png_get_libpng_ver(png_ptr);
        const char *v4 = png_get_copyright(png_ptr);

        if (v1) (void)strlen(v1);
        if (v2) (void)strlen(v2);
        if (v3) (void)strlen(v3);
        if (v4) (void)strlen(v4);

        // Cleanup
        png_destroy_read_struct(&png_ptr, nullptr, nullptr);
    }

    // Case B: Test with an empty string version (might fail version check)
    png_structp png_ptr2 = create_test_png_struct("");
    if (png_ptr2) {
        // Even if creation succeeds (e.g., if version check is loose), test functions
        const char *v1 = png_get_header_ver(png_ptr2);
        if (v1) (void)strlen(v1);
        
        png_destroy_read_struct(&png_ptr2, nullptr, nullptr);
    }

    // Case C: Test with a clearly wrong version string to trigger version mismatch
    png_structp png_ptr3 = create_test_png_struct("0.0.0");
    if (png_ptr3) {
        // If it didn't fail during creation, test functions
        const char *v1 = png_get_header_ver(png_ptr3);
        if (v1) (void)strlen(v1);

        png_destroy_read_struct(&png_ptr3, nullptr, nullptr);
    }

    // Case D: Test png_create_read_struct_2 with custom error/warn functions (nullptrs are fine)
    // We already tested this in create_test_png_struct, but we can vary the user_png_ver
    // using the fuzzer data more aggressively if needed.
    
    return 0;
}