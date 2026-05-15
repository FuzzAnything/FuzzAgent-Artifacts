// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return;
    fwrite(data, 1, size, fp);
    fclose(fp);
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    // Test magic_open with various flags derived from input
    int flags = 0;
    if (Size >= 1) {
        flags = Data[0] & (MAGIC_NONE | MAGIC_DEBUG | MAGIC_SYMLINK | 
                          MAGIC_COMPRESS | MAGIC_DEVICES | MAGIC_MIME_TYPE |
                          MAGIC_CONTINUE | MAGIC_CHECK | MAGIC_PRESERVE_ATIME);
    }
    
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        remove_dummy_file();
        return 0;
    }

    // Test magic_load with default database
    int load_ret = magic_load(cookie, NULL);
    (void)load_ret;

    // Test magic_file with dummy file
    const char *file_result = magic_file(cookie, "./dummy_file");
    (void)file_result;

    // Test magic_buffer with input data
    const char *buffer_result = magic_buffer(cookie, Data, Size);
    (void)buffer_result;

    // Test magic_error (check if any error occurred)
    const char *error_msg = magic_error(cookie);
    (void)error_msg;

    // Test magic_check with a separate magic_set
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        int check_ret = magic_check(check_cookie, NULL);
        (void)check_ret;
        const char *check_error = magic_error(check_cookie);
        (void)check_error;
        magic_close(check_cookie);
    }

    // Cleanup
    magic_close(cookie);
    remove_dummy_file();

    return 0;
}