// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    magic_t m = NULL;
    magic_t m_check = NULL;
    int ret = 0;
    int flags = 0;
    const char *result = NULL;

    // Fuzz magic_open with different flags derived from input if Size > 0
    if (Size > 0) {
        flags = Data[0] & 0xFF;
    } else {
        flags = 0;
    }

    // Test magic_open
    m = magic_open(flags);
    if (m == NULL) {
        // Handle allocation failure gracefully
        return 0;
    }

    // Test magic_getflags
    int current_flags = magic_getflags(m);
    (void)current_flags; // Suppress unused variable warning

    // Test magic_check with a separate handle as per hint
    m_check = magic_open(flags);
    if (m_check != NULL) {
        // magic_check requires a valid magic file path; use dummy_file
        ret = magic_check(m_check, "./dummy_file");
        (void)ret;
        magic_close(m_check);
        m_check = NULL;
    }

    // Prepare dummy file for magic_load, magic_compile, magic_file
    write_dummy_file(Data, Size);

    // Test magic_load
    ret = magic_load(m, "./dummy_file");
    if (ret != -1) {
        // Test magic_compile
        ret = magic_compile(m, "./dummy_file");
        (void)ret;

        // Test magic_file
        result = magic_file(m, "./dummy_file");
        if (result != NULL) {
            // Result is a string, no cleanup needed for the string itself
            (void)result;
        }
    }

    // Cleanup
    if (m != NULL) {
        magic_close(m);
    }

    // Cleanup dummy file
    unlink("./dummy_file");

    return 0;
}