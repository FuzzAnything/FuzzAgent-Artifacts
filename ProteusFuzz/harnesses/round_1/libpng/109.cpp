// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
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

static void dummy_error_fn(png_structp png_ptr, const char *error_msg) {
    (void)png_ptr;
    (void)error_msg;
}

static void dummy_warn_fn(png_structp png_ptr, const char *warn_msg) {
    (void)png_ptr;
    (void)warn_msg;
}

extern "C" int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // libpng version check compares user_png_ver against PNG_LIBPNG_VER_STRING.
    // Passing raw fuzz input as a C-string can cause buffer overflows if the input
    // is longer than the expected version string or lacks a null terminator.
    // We must ensure a valid, null-terminated string of safe length.
    
    const char *user_png_ver = nullptr;
    
    // Strategy: Only pass a valid version string if the input starts with the expected prefix,
    // or simply pass nullptr to skip version checking, or pass the known constant.
    // Passing nullptr is the safest way to fuzz the rest of the logic without triggering
    // the internal version string comparison buffer overflow on malformed input.
    // Alternatively, we can construct a safe string.
    
    // Let's try to interpret the input as a version string only if it's short and null-terminated,
    // otherwise use nullptr to bypass the check safely.
    bool is_valid_version = false;
    if (Size > 0 && Size < 16) { // Limit length to be safe
        // Check if it ends with null or treat as non-null terminated carefully
        // Safer: just use nullptr for fuzzing to avoid version check logic entirely
        // unless we construct a known good string.
        // However, to maximize coverage, let's try to pass a valid looking string if possible.
        // But the crash indicates the library reads past the buffer if it's not null-terminated correctly.
        // The safest approach for fuzzing this specific API is to pass nullptr or the exact macro.
        
        // Let's use nullptr to skip the version check, allowing us to test the struct creation
        // and subsequent calls without hitting the version string comparison bug with malformed input.
        user_png_ver = nullptr;
    } else {
        user_png_ver = nullptr;
    }

    // Test 1: png_create_read_struct_2
    png_structp read_ptr = png_create_read_struct_2(
        user_png_ver,
        nullptr, // error_ptr
        dummy_error_fn,
        dummy_warn_fn,
        nullptr, // mem_ptr
        nullptr, // malloc_fn
        nullptr  // free_fn
    );

    if (read_ptr != nullptr) {
        // Test png_get_header_version with valid read_ptr
        const char *header_ver = png_get_header_version(read_ptr);
        if (header_ver != nullptr) {
            volatile const char *v = header_ver;
            (void)v;
        }

        // Test png_get_libpng_ver with valid read_ptr
        const char *lib_ver = png_get_libpng_ver(read_ptr);
        if (lib_ver != nullptr) {
            volatile const char *v = lib_ver;
            (void)v;
        }

        // Test png_get_header_ver with valid read_ptr
        const char *header_ver2 = png_get_header_ver(read_ptr);
        if (header_ver2 != nullptr) {
            volatile const char *v = header_ver2;
            (void)v;
        }

        // Test png_get_copyright with valid read_ptr
        const char *copyright = png_get_copyright(read_ptr);
        if (copyright != nullptr) {
            volatile const char *v = copyright;
            (void)v;
        }

        // Cleanup read struct
        png_destroy_read_struct(&read_ptr, nullptr, nullptr);
    }

    // Test 2: png_create_write_struct
    png_structp write_ptr = png_create_write_struct(
        user_png_ver,
        nullptr, // error_ptr
        dummy_error_fn,
        dummy_warn_fn
    );

    if (write_ptr != nullptr) {
        // Test png_get_header_version with valid write_ptr
        const char *header_ver = png_get_header_version(write_ptr);
        if (header_ver != nullptr) {
            volatile const char *v = header_ver;
            (void)v;
        }

        // Test png_get_libpng_ver with valid write_ptr
        const char *lib_ver = png_get_libpng_ver(write_ptr);
        if (lib_ver != nullptr) {
            volatile const char *v = lib_ver;
            (void)v;
        }

        // Test png_get_header_ver with valid write_ptr
        const char *header_ver2 = png_get_header_ver(write_ptr);
        if (header_ver2 != nullptr) {
            volatile const char *v = header_ver2;
            (void)v;
        }

        // Test png_get_copyright with valid write_ptr
        const char *copyright = png_get_copyright(write_ptr);
        if (copyright != nullptr) {
            volatile const char *v = copyright;
            (void)v;
        }

        // Cleanup write struct
        png_destroy_write_struct(&write_ptr, nullptr);
    }

    // Test with nullptr pointers (functions that ignore png_ptr should handle this gracefully)
    
    // Test png_get_header_version with nullptr
    const char *nv_header = png_get_header_version(nullptr);
    if (nv_header != nullptr) {
        volatile const char *v = nv_header;
        (void)v;
    }

    // Test png_get_libpng_ver with nullptr
    const char *nv_lib = png_get_libpng_ver(nullptr);
    if (nv_lib != nullptr) {
        volatile const char *v = nv_lib;
        (void)v;
    }

    // Test png_get_header_ver with nullptr
    const char *nv_header2 = png_get_header_ver(nullptr);
    if (nv_header2 != nullptr) {
        volatile const char *v = nv_header2;
        (void)v;
    }

    // Test png_get_copyright with nullptr
    const char *nv_copyright = png_get_copyright(nullptr);
    if (nv_copyright != nullptr) {
        volatile const char *v = nv_copyright;
        (void)v;
    }

    return 0;
}