// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    magic_t cookie;
    const char *result;
    const char *error_msg;
    int ret;

    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    // Step 1: magic_open
    // Try different flags to explore more states
    int flags = MAGIC_MIME_TYPE;
    if (Size > 1) {
        flags = Data[0] & 0x0F; // Use input to vary flags slightly
        if (flags == 0) flags = MAGIC_MIME_TYPE;
    }
    cookie = magic_open(flags);
    if (cookie == NULL) {
        return 0;
    }

    // Step 2: magic_load
    // Try loading with NULL and a valid path to test error handling
    ret = magic_load(cookie, NULL);
    if (ret == -1) {
        error_msg = magic_error(cookie);
        (void)error_msg; // Suppress unused warning
    }

    ret = magic_load(cookie, NULL);
    if (ret == -1) {
        error_msg = magic_error(cookie);
        (void)error_msg;
    }

    // Try loading a non-existent file to trigger errors
    ret = magic_load(cookie, "./non_existent_magic_file");
    if (ret == -1) {
        error_msg = magic_error(cookie);
        (void)error_msg;
    }

    // Try loading a potentially valid magic file (if it exists in the environment)
    // Fuzzers often run in environments where /usr/share/misc/magic.mgc might exist
    ret = magic_load(cookie, "/usr/share/misc/magic.mgc");
    if (ret == -1) {
        error_msg = magic_error(cookie);
        (void)error_msg;
    }

    // Step 3: magic_error (check for errors from load)
    error_msg = magic_error(cookie);
    (void)error_msg;

    // Step 4: magic_file
    // Test with the dummy file we created
    result = magic_file(cookie, "./dummy_file");
    (void)result;

    // Test with a non-existent file
    result = magic_file(cookie, "./non_existent_file");
    (void)result;

    // Test with NULL filename
    result = magic_file(cookie, NULL);
    (void)result;

    // Step 5: magic_error (check for errors from file)
    error_msg = magic_error(cookie);
    (void)error_msg;

    // Step 6: magic_close
    magic_close(cookie);

    return 0;
}