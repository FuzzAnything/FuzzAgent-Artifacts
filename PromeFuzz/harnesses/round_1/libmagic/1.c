// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getpath at magic.c:254:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "magic.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // 1. magic_getpath
    const char *path = magic_getpath(NULL, 0);
    (void)path;

    // 2. magic_version (first call)
    int version1 = magic_version();

    // 3. magic_version (second call)
    int version2 = magic_version();

    // 4. magic_open
    int flags = 0;
    if (Size > 0) flags = Data[0] % 256;
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        unlink("./dummy_file");
        return 0;
    }

    // 5. magic_check (requires separate magic_set)
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        int check_result = magic_check(check_cookie, "./dummy_file");
        (void)check_result;
        magic_close(check_cookie);
    }

    // 6. magic_compile
    int compile_result = magic_compile(cookie, "./dummy_file");
    (void)compile_result;

    // 7. magic_list (requires separate magic_set)
    magic_t list_cookie = magic_open(flags);
    if (list_cookie) {
        int list_result = magic_list(list_cookie, "./dummy_file");
        (void)list_result;
        magic_close(list_cookie);
    }

    // 8. magic_error
    const char *error_msg = magic_error(cookie);
    (void)error_msg;

    // 9. magic_close
    magic_close(cookie);

    // Clean up dummy file
    unlink("./dummy_file");

    // Use version variables to avoid optimization
    if (version1 != version2) {
        __builtin_trap();
    }

    return 0;
}