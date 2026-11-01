// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) return;
    if (size > 0) {
        write(fd, data, size);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    magic_t m1 = NULL;
    magic_t m2 = NULL;
    magic_t m3 = NULL;
    magic_t m4 = NULL;

    // Test magic_open with different flags
    m1 = magic_open(MAGIC_NONE);
    m2 = magic_open(MAGIC_MIME_TYPE);
    m3 = magic_open(MAGIC_MIME_ENCODING);
    m4 = magic_open(MAGIC_RAW);

    if (m1 == NULL && m2 == NULL && m3 == NULL && m4 == NULL) {
        return 0;
    }

    // Prepare dummy file for magic_file and magic_check/magic_list/magic_compile
    write_dummy_file(Data, Size);

    // Test magic_check on m1
    if (m1 != NULL) {
        magic_check(m1, "./dummy_file");
        magic_error(m1);
        magic_close(m1);
        m1 = NULL;
    }

    // Test magic_list on m2
    if (m2 != NULL) {
        magic_list(m2, "./dummy_file");
        magic_error(m2);
        magic_close(m2);
        m2 = NULL;
    }

    // Test magic_compile on m3
    if (m3 != NULL) {
        magic_compile(m3, "./dummy_file");
        magic_error(m3);
        magic_close(m3);
        m3 = NULL;
    }

    // Test magic_file on m4
    if (m4 != NULL) {
        const char *result = magic_file(m4, "./dummy_file");
        if (result == NULL) {
            magic_error(m4);
        }
        magic_close(m4);
        m4 = NULL;
    }

    // Test error handling with NULL handle
    magic_error(NULL);
    magic_check(NULL, "./dummy_file");
    magic_list(NULL, "./dummy_file");
    magic_compile(NULL, "./dummy_file");
    magic_file(NULL, "./dummy_file");

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}