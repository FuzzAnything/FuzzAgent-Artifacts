// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "magic.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return -1;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Test magic_version
    (void)magic_version();

    // Test magic_open with various flags
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] % 256;
    }
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        unlink("./dummy_file");
        return 0;
    }

    // Test magic_compile
    magic_compile(cookie, "./dummy_file");

    // Test magic_load with both dummy file and NULL
    magic_load(cookie, "./dummy_file");
    magic_load(cookie, NULL);

    // Test magic_check with separate cookie
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        magic_close(check_cookie);
    }

    // Test magic_list with separate cookie
    magic_t list_cookie = magic_open(flags);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        magic_close(list_cookie);
    }

    // Cleanup
    magic_close(cookie);
    unlink("./dummy_file");

    return 0;
}