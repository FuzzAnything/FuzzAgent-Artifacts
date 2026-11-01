// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_header_ver at png.c:821:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Static helper to safely destroy read struct
static void safe_destroy_read_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr) {
    if (png_ptr_ptr && *png_ptr_ptr) {
        png_destroy_read_struct(png_ptr_ptr, info_ptr_ptr, NULL);
    }
}

// Static helper to safely destroy write struct
static void safe_destroy_write_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr) {
    if (png_ptr_ptr && *png_ptr_ptr) {
        png_destroy_write_struct(png_ptr_ptr, info_ptr_ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    // Avoid processing empty inputs to prevent potential issues with version string parsing
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for potential I/O operations if needed, though these specific APIs
    // mostly rely on version strings and struct initialization.
    // We write a small amount of data to ensure the file exists if the library attempts to open it.
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // 1. Test png_create_read_struct_2
    // We use the first few bytes as a potential version string, ensuring it's null-terminated
    // or we use a fixed safe version string.
    const char *user_ver = "1.6.0"; // Safe default
    if (Size > 10) {
        // Try to use fuzzed data as version string if it looks like a version (starts with digit)
        char ver_buf[16];
        size_t len = (Size < 15) ? Size : 15;
        memcpy(ver_buf, Data, len);
        ver_buf[len] = '\0';
        if (ver_buf[0] >= '0' && ver_buf[0] <= '9') {
            user_ver = ver_buf;
        }
    }

    png_structp read_ptr = png_create_read_struct_2(
        user_ver,
        NULL, // error_ptr
        NULL, // error_fn
        NULL, // warn_fn
        NULL, // mem_ptr
        NULL, // malloc_fn
        NULL  // free_fn
    );

    if (read_ptr) {
        // 2. Test png_get_header_version
        const char *header_ver = png_get_header_version(read_ptr);
        if (header_ver) {
            // Consume the string to avoid unused variable warnings and explore state
            size_t len = strlen(header_ver);
            if (len > 0) {
                volatile char c = header_ver[0];
                (void)c;
            }
        }

        // 3. Test png_get_libpng_ver
        const char *lib_ver = png_get_libpng_ver(read_ptr);
        if (lib_ver) {
            size_t len = strlen(lib_ver);
            if (len > 0) {
                volatile char c = lib_ver[0];
                (void)c;
            }
        }

        // 4. Test png_get_copyright
        const char *copyright = png_get_copyright(read_ptr);
        if (copyright) {
            size_t len = strlen(copyright);
            if (len > 0) {
                volatile char c = copyright[0];
                (void)c;
            }
        }

        // 5. Test png_get_header_ver
        const char *header_ver2 = png_get_header_ver(read_ptr);
        if (header_ver2) {
            size_t len = strlen(header_ver2);
            if (len > 0) {
                volatile char c = header_ver2[0];
                (void)c;
            }
        }

        // Cleanup read struct
        png_infop info_ptr = NULL;
        safe_destroy_read_struct(&read_ptr, &info_ptr);
    }

    // 6. Test png_create_write_struct
    // Use a different part of the input or the same safe version
    png_structp write_ptr = png_create_write_struct(
        user_ver,
        NULL, // error_ptr
        NULL, // error_fn
        NULL  // warn_fn
    );

    if (write_ptr) {
        // We can call the version functions again on the write struct as they ignore the struct type
        const char *w_header_ver = png_get_header_version(write_ptr);
        if (w_header_ver) {
            size_t len = strlen(w_header_ver);
            (void)len;
        }

        const char *w_lib_ver = png_get_libpng_ver(write_ptr);
        if (w_lib_ver) {
            size_t len = strlen(w_lib_ver);
            (void)len;
        }

        const char *w_copyright = png_get_copyright(write_ptr);
        if (w_copyright) {
            size_t len = strlen(w_copyright);
            (void)len;
        }

        const char *w_header_ver2 = png_get_header_ver(write_ptr);
        if (w_header_ver2) {
            size_t len = strlen(w_header_ver2);
            (void)len;
        }

        // Cleanup write struct
        png_infop info_ptr_w = NULL;
        safe_destroy_write_struct(&write_ptr, &info_ptr_w);
    }

    // Clean up dummy file
    remove("./dummy_file");

    return 0;
}