// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    int flags = Data[0] % 256;
    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        cleanup_dummy_file();
        return 0;
    }

    int load_result = magic_load(cookie, NULL);
    if (load_result == -1) {
        const char *error = magic_error(cookie);
    }

    const char *file_result = magic_file(cookie, "./dummy_file");
    if (file_result == NULL) {
        const char *error = magic_error(cookie);
    }

    magic_t check_cookie = magic_open(flags);
    if (check_cookie != NULL) {
        int check_result = magic_check(check_cookie, "./dummy_file");
        if (check_result == -1) {
            const char *error = magic_error(check_cookie);
        }
        magic_close(check_cookie);
    }

    const char *error = magic_error(cookie);
    magic_close(cookie);
    cleanup_dummy_file();

    return 0;
}