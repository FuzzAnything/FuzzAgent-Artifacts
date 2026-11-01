// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
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
#include <unistd.h>
#include <fcntl.h>
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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a dummy file for magic_file and other file-based operations
    write_dummy_file(Data, Size);

    // Test magic_open with different flags
    int flags = MAGIC_ERROR | MAGIC_MIME_TYPE;
    magic_t m = magic_open(flags);
    if (m == NULL) {
        return 0;
    }

    // Test magic_load
    magic_load(m, "./dummy_file");

    // Test magic_file
    const char *result = magic_file(m, "./dummy_file");
    if (result) {
        (void)result; // Use result to avoid unused warning
    }

    magic_close(m);

    // Test magic_open for magic_check
    magic_t m_check = magic_open(0);
    if (m_check != NULL) {
        magic_check(m_check, "./dummy_file");
        magic_close(m_check);
    }

    // Test magic_open for magic_compile
    magic_t m_compile = magic_open(0);
    if (m_compile != NULL) {
        magic_compile(m_compile, "./dummy_file");
        magic_close(m_compile);
    }

    // Test magic_open for magic_list
    magic_t m_list = magic_open(0);
    if (m_list != NULL) {
        magic_list(m_list, "./dummy_file");
        magic_close(m_list);
    }

    // Clean up dummy file
    unlink("./dummy_file");

    return 0;
}