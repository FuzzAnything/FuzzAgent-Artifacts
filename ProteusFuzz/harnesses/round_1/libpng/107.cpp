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
#include <png.h>
#include <setjmp.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstddef>

// Forward declaration for the error handler
static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg);

// Dummy error handler that triggers longjmp
static void dummy_error_fn(png_structp png_ptr, png_const_charp error_msg) {
    (void)error_msg;
    if (png_ptr != NULL) {
        // png_struct is opaque, so we cannot access members directly.
        // We rely on the library's internal longjmp mechanism.
        // Since we cannot safely call longjmp without access to the buffer,
        // we will just abort to prevent undefined behavior.
        abort();
    }
}

// Dummy warning handler
static void dummy_warn_fn(png_structp png_ptr, png_const_charp warning_msg) {
    (void)png_ptr;
    (void)warning_msg;
}

// Dummy malloc function
static png_voidp dummy_malloc_fn(png_structp png_ptr, png_size_t size) {
    (void)png_ptr;
    if (size == 0) return NULL;
    png_voidp ptr = malloc(size);
    if (ptr == NULL) {
        return NULL;
    }
    return ptr;
}

// Dummy free function
static void dummy_free_fn(png_structp png_ptr, png_voidp ptr) {
    (void)png_ptr;
    if (ptr != NULL) {
        free(ptr);
    }
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for write operations if needed
    const char *dummy_file = "./dummy_file";
    FILE *fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }

    // Test png_create_read_struct_2
    png_structp read_ptr = png_create_read_struct_2(
        PNG_LIBPNG_VER_STRING,
        NULL,
        dummy_error_fn,
        dummy_warn_fn,
        NULL,
        dummy_malloc_fn,
        dummy_free_fn
    );

    if (read_ptr != NULL) {
        // Test png_get_header_version with read_ptr
        const char *header_ver = png_get_header_version(read_ptr);
        if (header_ver != NULL) {
            (void)strlen(header_ver);
        }

        // Test png_get_libpng_ver with read_ptr
        const char *lib_ver = png_get_libpng_ver(read_ptr);
        if (lib_ver != NULL) {
            (void)strlen(lib_ver);
        }

        // Test png_get_header_ver with read_ptr
        const char *header_ver_str = png_get_header_ver(read_ptr);
        if (header_ver_str != NULL) {
            (void)strlen(header_ver_str);
        }

        // Test png_get_copyright with read_ptr
        const char *copyright = png_get_copyright(read_ptr);
        if (copyright != NULL) {
            (void)strlen(copyright);
        }

        // Cleanup read_ptr
        png_destroy_read_struct(&read_ptr, NULL, NULL);
    }

    // Test png_create_write_struct
    png_structp write_ptr = png_create_write_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        dummy_error_fn,
        dummy_warn_fn
    );

    if (write_ptr != NULL) {
        // Test png_get_header_version with write_ptr
        const char *header_ver_write = png_get_header_version(write_ptr);
        if (header_ver_write != NULL) {
            (void)strlen(header_ver_write);
        }

        // Test png_get_libpng_ver with write_ptr
        const char *lib_ver_write = png_get_libpng_ver(write_ptr);
        if (lib_ver_write != NULL) {
            (void)strlen(lib_ver_write);
        }

        // Test png_get_header_ver with write_ptr
        const char *header_ver_str_write = png_get_header_ver(write_ptr);
        if (header_ver_str_write != NULL) {
            (void)strlen(header_ver_str_write);
        }

        // Test png_get_copyright with write_ptr
        const char *copyright_write = png_get_copyright(write_ptr);
        if (copyright_write != NULL) {
            (void)strlen(copyright_write);
        }

        // Cleanup write_ptr
        png_destroy_write_struct(&write_ptr, NULL);
    }

    // Test with NULL pointer (should not crash, but behavior may vary)
    const char *header_ver_null = png_get_header_version(NULL);
    if (header_ver_null != NULL) {
        (void)strlen(header_ver_null);
    }

    const char *lib_ver_null = png_get_libpng_ver(NULL);
    if (lib_ver_null != NULL) {
        (void)strlen(lib_ver_null);
    }

    const char *header_ver_str_null = png_get_header_ver(NULL);
    if (header_ver_str_null != NULL) {
        (void)strlen(header_ver_str_null);
    }

    const char *copyright_null = png_get_copyright(NULL);
    if (copyright_null != NULL) {
        (void)strlen(copyright_null);
    }

    return 0;
}