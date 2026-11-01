// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) return;
    if (size > 0) {
        write(fd, data, size);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int fd = -1;
    int ret;
    size_t param_val;
    const char *result;
    int flags;

    // Initialize magic handle
    cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Load magic database
    if (magic_load(cookie, NULL) != 0) {
        magic_close(cookie);
        return 0;
    }

    // Test magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        (void)result; // Suppress unused variable warning
    }

    // Test magic_file with dummy file
    if (Size > 0) {
        write_dummy_file(Data, Size);
        result = magic_file(cookie, "./dummy_file");
        (void)result;
    }

    // Test magic_descriptor
    if (Size > 0) {
        fd = open("./dummy_file", O_RDONLY);
        if (fd != -1) {
            result = magic_descriptor(cookie, fd);
            (void)result;
            close(fd);
        }
    }

    // Test magic_setflags and magic_getflags
    flags = magic_getflags(cookie);
    (void)flags;
    
    // Try setting various flags
    ret = magic_setflags(cookie, MAGIC_MIME_TYPE);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_MIME_ENCODING);
    (void)ret;
    // MAGIC_MIME_COMPRESS is not a standard flag in many libmagic versions, removed
    ret = magic_setflags(cookie, MAGIC_RAW);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_PRESERVE_ATIME);
    (void)ret;
    
    flags = magic_getflags(cookie);
    (void)flags;

    // Test magic_getparam with various parameters
    if (Size > 0) {
        // Use data to fuzz parameter selection if needed, but we test known constants
        ret = magic_getparam(cookie, MAGIC_PARAM_INDIR_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_NAME_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_ELF_SHNUM_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_ELF_PHNUM_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_ELF_NOTES_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_REGEX_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_MAGWARN_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_ENCODING_MAX, &param_val);
        (void)ret;
        ret = magic_getparam(cookie, MAGIC_PARAM_ELF_SHSIZE_MAX, &param_val);
        (void)ret;
    }

    // Cleanup
    if (fd != -1) {
        close(fd);
    }
    magic_close(cookie);
    unlink("./dummy_file");

    return 0;
}