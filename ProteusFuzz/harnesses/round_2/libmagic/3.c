// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    magic_t m;
    const char *err;
    int ret;

    // Test 1: magic_open and basic error handling
    m = magic_open(MAGIC_MIME_TYPE);
    if (m == NULL) {
        return 0;
    }

    // Test magic_error on a valid handle (should be NULL if no error)
    err = magic_error(m);
    (void)err; // Suppress unused warning

    // Test 2: magic_check with a separate handle as per hint
    magic_t m_check = magic_open(MAGIC_MIME_TYPE);
    if (m_check != NULL) {
        // Write input to dummy file to act as a magic database for testing
        write_dummy_file(Data, Size);
        ret = magic_check(m_check, "./dummy_file");
        (void)ret;
        magic_close(m_check);
    }

    // Test 3: magic_compile
    // We use the dummy file as the magic file to compile
    ret = magic_compile(m, "./dummy_file");
    (void)ret;

    // Test 4: magic_list with a separate handle as per hint
    magic_t m_list = magic_open(MAGIC_MIME_TYPE);
    if (m_list != NULL) {
        ret = magic_list(m_list, "./dummy_file");
        (void)ret;
        magic_close(m_list);
    }

    // Test 5: magic_file
    // We write the input data to a dummy file and try to identify it
    write_dummy_file(Data, Size);
    const char *file_type = magic_file(m, "./dummy_file");
    (void)file_type;

    // Test 6: magic_error after potential failure
    err = magic_error(m);
    (void)err;

    // Cleanup
    magic_close(m);
    cleanup_dummy_file();

    return 0;
}