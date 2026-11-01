// This fuzz driver is generated for library libpng, aiming to fuzz the following functions:
// png_get_header_version at png.c:829:1 in png.h
// png_destroy_write_struct at pngwrite.c:1037:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_create_read_struct_2 at pngread.c:40:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
// png_get_header_version at png.c:829:1 in png.h
// png_destroy_read_struct at pngread.c:919:1 in png.h
// png_create_write_struct at pngwrite.c:553:1 in png.h
// png_get_libpng_ver at png.c:814:1 in png.h
// png_get_copyright at png.c:790:1 in png.h
// png_get_header_ver at png.c:821:1 in png.h
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
#include <stddef.h>
#include <stdio.h>

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test png_get_libpng_ver with NULL and valid pointers
    const char *ver1 = png_get_libpng_ver(NULL);
    if (ver1 != NULL) {
        // Just consume the string to avoid unused variable warnings
        (void)ver1[0];
    }

    // Test png_get_copyright with NULL
    const char *copyright = png_get_copyright(NULL);
    if (copyright != NULL) {
        (void)copyright[0];
    }

    // Test png_get_header_ver with NULL
    const char *header_ver = png_get_header_ver(NULL);
    if (header_ver != NULL) {
        (void)header_ver[0];
    }

    // Test png_get_header_version with NULL
    const char *header_version = png_get_header_version(NULL);
    if (header_version != NULL) {
        (void)header_version[0];
    }

    // Create a dummy file for write operations if needed
    FILE *dummy_file = fopen("./dummy_file", "wb");
    if (dummy_file) {
        if (Size > 0) {
            fwrite(Data, 1, Size, dummy_file);
        }
        fclose(dummy_file);
    }

    // Test png_create_read_struct_2
    png_structp read_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );

    if (read_ptr != NULL) {
        // Test version functions with valid read_ptr
        const char *ver2 = png_get_libpng_ver(read_ptr);
        if (ver2 != NULL) (void)ver2[0];

        const char *copyright2 = png_get_copyright(read_ptr);
        if (copyright2 != NULL) (void)copyright2[0];

        const char *header_ver2 = png_get_header_ver(read_ptr);
        if (header_ver2 != NULL) (void)header_ver2[0];

        const char *header_version2 = png_get_header_version(read_ptr);
        if (header_version2 != NULL) (void)header_version2[0];

        // Destroy read struct
        png_destroy_read_struct(&read_ptr, NULL, NULL);
    }

    // Test png_create_write_struct
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL
    );

    if (write_ptr != NULL) {
        // Test version functions with valid write_ptr
        const char *ver3 = png_get_libpng_ver(write_ptr);
        if (ver3 != NULL) (void)ver3[0];

        const char *copyright3 = png_get_copyright(write_ptr);
        if (copyright3 != NULL) (void)copyright3[0];

        const char *header_ver3 = png_get_header_ver(write_ptr);
        if (header_ver3 != NULL) (void)header_ver3[0];

        const char *header_version3 = png_get_header_version(write_ptr);
        if (header_version3 != NULL) (void)header_version3[0];

        // Destroy write struct
        png_destroy_write_struct(&write_ptr, NULL);
    }

    return 0;
}