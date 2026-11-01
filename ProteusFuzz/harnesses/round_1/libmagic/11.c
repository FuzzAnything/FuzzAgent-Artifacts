// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static int create_dummy_file(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    if (size > 0) {
        ssize_t written = write(fd, data, size);
        if (written != (ssize_t)size) {
            close(fd);
            unlink(filename);
            return -1;
        }
    }
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int fd = -1;
    int ret;
    const char *result;
    size_t param_val;

    // Initialize magic cookie
    cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Load magic database
    if (magic_load(cookie, NULL) != 0) {
        magic_close(cookie);
        return 0;
    }

    // Test 1: magic_buffer
    result = magic_buffer(cookie, Data, Size);
    (void)result; // Suppress unused variable warning

    // Test 2: magic_setflags and magic_getflags
    ret = magic_setflags(cookie, MAGIC_PRESERVE_ATIME);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_MIME_TYPE);
    (void)ret;
    ret = magic_getflags(cookie);
    (void)ret;

    // Test 3: magic_getparam
    // Use MAGIC_PARAM_REGEX_MAX which is a standard parameter
    ret = magic_getparam(cookie, MAGIC_PARAM_REGEX_MAX, &param_val);
    (void)ret;
    // Try another valid parameter if available, otherwise just regex_max
    ret = magic_getparam(cookie, MAGIC_PARAM_NAME_MAX, &param_val);
    (void)ret;

    // Test 4: magic_file (requires a file)
    if (create_dummy_file(Data, Size) == 0) {
        result = magic_file(cookie, "./dummy_file");
        (void)result;
        // Clean up dummy file
        unlink("./dummy_file");
    }

    // Test 5: magic_descriptor (requires a file descriptor)
    if (create_dummy_file(Data, Size) == 0) {
        fd = open("./dummy_file", O_RDONLY);
        if (fd >= 0) {
            result = magic_descriptor(cookie, fd);
            (void)result;
            close(fd);
        }
        unlink("./dummy_file");
    }

    // Cleanup
    magic_close(cookie);

    return 0;
}