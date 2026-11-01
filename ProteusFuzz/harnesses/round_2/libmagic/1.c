// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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
#include <stdio.h>
#include <string.h>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    magic_t cookie;
    const char *result;
    const char *error_msg;
    int ret;

    if (Size == 0) {
        return 0;
    }

    // Step 1: magic_open
    cookie = magic_open(MAGIC_MIME_TYPE);
    if (cookie == NULL) {
        return 0;
    }

    // Step 2: magic_load
    ret = magic_load(cookie, NULL);
    // If load fails, we can still check error, but we proceed to test error handling
    if (ret == -1) {
        error_msg = magic_error(cookie);
        (void)error_msg; // Use to avoid unused warning
    }

    // Write fuzz data to dummy file for magic_file
    write_dummy_file(Data, Size);

    // Step 3: magic_file
    result = magic_file(cookie, "./dummy_file");
    (void)result; // Result is not used, just exercising the path

    // Step 4: magic_error (after magic_file)
    error_msg = magic_error(cookie);
    (void)error_msg;

    // Step 5: magic_close
    magic_close(cookie);

    return 0;
}