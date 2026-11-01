// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int ret;
    const char *result;
    int fd;

    // Initialize magic handle
    cookie = magic_open(MAGIC_MIME_TYPE | MAGIC_ERROR);
    if (cookie == NULL) {
        return 0;
    }

    // Test magic_setflags with various flags
    ret = magic_setflags(cookie, MAGIC_PRESERVE_ATIME);
    (void)ret; // Ignore return for fuzzing stability
    ret = magic_setflags(cookie, 0); // Reset or test with 0
    (void)ret;

    // Load default database
    if (magic_load(cookie, NULL) == -1) {
        magic_close(cookie);
        return 0;
    }

    // Test magic_file with dummy file
    if (Size > 0) {
        write_dummy_file(Data, Size);
        result = magic_file(cookie, "./dummy_file");
        (void)result;
    }

    // Test magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        (void)result;
    }

    // Test magic_descriptor with a file descriptor
    if (Size > 0) {
        write_dummy_file(Data, Size);
        fd = open("./dummy_file", O_RDONLY);
        if (fd >= 0) {
            result = magic_descriptor(cookie, fd);
            (void)result;
            close(fd);
        }
    }

    // Test magic_load_buffers with in-memory buffers
    if (Size > 0) {
        void *buffers[1];
        size_t sizes[1];
        buffers[0] = (void *)Data;
        sizes[0] = Size;
        ret = magic_load_buffers(cookie, buffers, sizes, 1);
        (void)ret;

        // After loading buffers, try magic_buffer again
        result = magic_buffer(cookie, Data, Size);
        (void)result;
    }

    // Test magic_error
    result = magic_error(cookie);
    (void)result;

    // Cleanup
    magic_close(cookie);
    return 0;
}