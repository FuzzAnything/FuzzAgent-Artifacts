// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    magic_t cookie = magic_open(MAGIC_NONE);
    if (cookie == NULL) {
        return 0;
    }

    // Try to load the default database
    if (magic_load(cookie, NULL) != 0) {
        // If loading fails, check error and try to continue or exit gracefully
        const char *err = magic_error(cookie);
        (void)err; // Suppress unused variable warning if not used
        magic_close(cookie);
        return 0;
    }

    // Write the fuzz input to a dummy file
    write_dummy_file(Data, Size);

    // Invoke magic_file to identify the dummy file
    const char *result = magic_file(cookie, "./dummy_file");
    (void)result; // Suppress unused variable warning

    // Check for errors after magic_file
    const char *err_after_file = magic_error(cookie);
    (void)err_after_file;

    // Clean up
    magic_close(cookie);

    return 0;
}