// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;
    if (Size > 0) {
        write(fd, Data, Size);
    }
    close(fd);
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    magic_t m = NULL;
    magic_t m_check = NULL;
    magic_t m_list = NULL;
    int ret;

    // Test magic_check with a separate handle
    m_check = magic_open(MAGIC_NONE);
    if (m_check) {
        write_dummy_file(Data, Size);
        ret = magic_check(m_check, "./dummy_file");
        (void)ret; // Ignore return for fuzzing stability, just ensure no crash
        magic_close(m_check);
        m_check = NULL;
    }

    // Test magic_list with a separate handle
    m_list = magic_open(MAGIC_NONE);
    if (m_list) {
        write_dummy_file(Data, Size);
        ret = magic_list(m_list, "./dummy_file");
        (void)ret;
        magic_close(m_list);
        m_list = NULL;
    }

    // Test main workflow with magic_open, magic_compile, magic_load, magic_file
    m = magic_open(MAGIC_NONE);
    if (!m) return 0;

    // Write data to dummy file for magic_file and magic_load/compile
    write_dummy_file(Data, Size);

    // Test magic_compile
    ret = magic_compile(m, "./dummy_file");
    (void)ret;

    // Test magic_load
    ret = magic_load(m, "./dummy_file");
    (void)ret;

    // Test magic_file
    const char *result = magic_file(m, "./dummy_file");
    (void)result;

    // Cleanup
    magic_close(m);
    m = NULL;

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}