// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int ret;
    const char *result;
    int err_code;
    const char *err_msg;

    if (Size == 0) {
        return 0;
    }

    // Initialize magic handle
    cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Test 1: magic_setflags with various flags
    // Using a subset of common flags to avoid undefined behavior
    int flags[] = {0, MAGIC_MIME, MAGIC_MIME_TYPE, MAGIC_MIME_ENCODING, 
                   MAGIC_PRESERVE_ATIME, MAGIC_SYMLINK, MAGIC_RAW};
    for (int i = 0; i < 7; i++) {
        ret = magic_setflags(cookie, flags[i]);
        if (ret == -1) {
            err_code = magic_errno(cookie);
            (void)err_code; // Suppress unused warning
            err_msg = magic_error(cookie);
            (void)err_msg; // Suppress unused warning
        }
    }

    // Test 2: magic_buffer
    result = magic_buffer(cookie, Data, Size);
    if (result == NULL) {
        err_code = magic_errno(cookie);
        (void)err_code;
        err_msg = magic_error(cookie);
        (void)err_msg;
    }

    // Test 3: magic_file with dummy file
    write_dummy_file(Data, Size);
    result = magic_file(cookie, "./dummy_file");
    if (result == NULL) {
        err_code = magic_errno(cookie);
        (void)err_code;
        err_msg = magic_error(cookie);
        (void)err_msg;
    }

    // Test 4: magic_load_buffers
    // Prepare buffers for magic_load_buffers
    void *buffers[1];
    size_t buffer_sizes[1];
    buffers[0] = (void *)Data;
    buffer_sizes[0] = Size;

    ret = magic_load_buffers(cookie, buffers, buffer_sizes, 1);
    if (ret == -1) {
        err_code = magic_errno(cookie);
        (void)err_code;
        err_msg = magic_error(cookie);
        (void)err_msg;
    } else {
        // Try to use magic_buffer after loading buffers
        result = magic_buffer(cookie, Data, Size);
        if (result == NULL) {
            err_code = magic_errno(cookie);
            (void)err_code;
            err_msg = magic_error(cookie);
            (void)err_msg;
        }
    }

    // Test 5: Test error handling with NULL handle
    err_code = magic_errno(NULL);
    (void)err_code;
    err_msg = magic_error(NULL);
    (void)err_msg;

    // Cleanup
    magic_close(cookie);
    return 0;
}