// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        return -1;
    }
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    magic_t cookie = magic_open(MAGIC_NONE);
    if (cookie == NULL) {
        return 0;
    }

    if (write_dummy_file(Data, Size) != 0) {
        magic_close(cookie);
        return 0;
    }

    int load_result = magic_load(cookie, NULL);
    if (load_result != 0) {
        const char *err = magic_error(cookie);
        (void)err;
        magic_close(cookie);
        return 0;
    }

    const char *file_result = magic_file(cookie, "./dummy_file");
    if (file_result == NULL) {
        const char *err = magic_error(cookie);
        (void)err;
    }

    const char *err_after_file = magic_error(cookie);
    (void)err_after_file;

    magic_close(cookie);

    return 0;
}