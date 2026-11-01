// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
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
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    const char *result = NULL;
    int ret = 0;

    // Test magic_open with different flags
    int flags = MAGIC_MIME_TYPE;
    if (Size > 0) {
        flags = (Data[0] % 7) | (MAGIC_MIME_TYPE & 0xFF);
        // Ensure at least one valid flag combination or 0
        if (flags == 0) flags = MAGIC_MIME_TYPE;
    }

    cookie = magic_open(flags);
    if (cookie == NULL) {
        // If open fails, we can't do much, but we can test error functions with NULL
        const char *err_msg = magic_error(NULL);
        int err_code = magic_errno(NULL);
        (void)err_msg;
        (void)err_code;
        return 0;
    }

    // Load the magic database
    if (magic_load(cookie, NULL) != 0) {
        // If load fails, we can still test error reporting
        const char *err_msg = magic_error(cookie);
        int err_code = magic_errno(cookie);
        (void)err_msg;
        (void)err_code;
        magic_close(cookie);
        return 0;
    }

    // Test magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        (void)result;
    }

    // Test magic_file with a dummy file
    if (Size > 0) {
        write_dummy_file(Data, Size);
        result = magic_file(cookie, "./dummy_file");
        (void)result;
    }

    // Test magic_load_buffers
    if (Size > 0) {
        void *buffers[2];
        size_t sizes[2];
        size_t count = 0;

        // Split data into two buffers if possible
        if (Size >= 2) {
            buffers[0] = (void *)Data;
            sizes[0] = Size / 2;
            buffers[1] = (void *)(Data + Size / 2);
            sizes[1] = Size - Size / 2;
            count = 2;
        } else {
            buffers[0] = (void *)Data;
            sizes[0] = Size;
            count = 1;
        }

        ret = magic_load_buffers(cookie, buffers, sizes, count);
        if (ret == -1) {
            const char *err_msg = magic_error(cookie);
            int err_code = magic_errno(cookie);
            (void)err_msg;
            (void)err_code;
        }
    }

    // Test error functions after potential failures
    const char *err_msg = magic_error(cookie);
    int err_code = magic_errno(cookie);
    (void)err_msg;
    (void)err_code;

    // Cleanup
    magic_close(cookie);
    unlink("./dummy_file");

    return 0;
}