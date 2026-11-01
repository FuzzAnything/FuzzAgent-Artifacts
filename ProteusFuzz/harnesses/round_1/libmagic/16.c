// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int ret;
    const char *result;
    int fd;

    // Initialize magic handle
    cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Test magic_setflags with various flags derived from input
    if (Size > 0) {
        int flags = Data[0] & 0xFF;
        magic_setflags(cookie, flags);
    }

    // Attempt to load buffers using fuzz input
    // We create a small array of pointers and sizes to test magic_load_buffers
    if (Size > 0) {
        void *buffers[2];
        size_t sizes[2];
        buffers[0] = (void *)Data;
        sizes[0] = Size;
        buffers[1] = NULL;
        sizes[1] = 0;
        magic_load_buffers(cookie, buffers, sizes, 1);
    }

    // Test magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        if (result != NULL) {
            // Consume result to avoid unused variable warnings
            (void)strlen(result);
        }
    }

    // Create a dummy file for magic_file and magic_descriptor tests
    write_dummy_file(Data, Size);

    // Test magic_file
    result = magic_file(cookie, "./dummy_file");
    if (result != NULL) {
        (void)strlen(result);
    }

    // Test magic_descriptor
    fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        result = magic_descriptor(cookie, fd);
        if (result != NULL) {
            (void)strlen(result);
        }
        close(fd);
    }

    // Test magic_error
    result = magic_error(cookie);
    if (result != NULL) {
        (void)strlen(result);
    }

    // Cleanup
    magic_close(cookie);
    unlink("./dummy_file");

    return 0;
}