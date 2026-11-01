// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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
    if (fd == -1) return;
    if (size > 0) {
        write(fd, data, size);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create a dummy file with the fuzz input
    write_dummy_file(Data, Size);

    // Test magic_list
    magic_t m_list = magic_open(MAGIC_MIME_TYPE);
    if (m_list) {
        magic_list(m_list, "./dummy_file");
        magic_close(m_list);
    }

    // Test magic_check
    magic_t m_check = magic_open(MAGIC_MIME_TYPE);
    if (m_check) {
        magic_check(m_check, "./dummy_file");
        magic_close(m_check);
    }

    // Test magic_compile
    magic_t m_compile = magic_open(MAGIC_MIME_TYPE);
    if (m_compile) {
        magic_compile(m_compile, "./dummy_file");
        magic_close(m_compile);
    }

    // Test magic_load
    magic_t m_load = magic_open(MAGIC_MIME_TYPE);
    if (m_load) {
        magic_load(m_load, "./dummy_file");
        magic_close(m_load);
    }

    // Test magic_file
    magic_t m_file = magic_open(MAGIC_MIME_TYPE);
    if (m_file) {
        magic_load(m_file, "./dummy_file");
        const char *result = magic_file(m_file, "./dummy_file");
        (void)result; // Suppress unused variable warning
        magic_close(m_file);
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}