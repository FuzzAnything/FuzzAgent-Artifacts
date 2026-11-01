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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    magic_t cookie;
    int ret;
    const char *err_msg;
    const char *file_type;

    if (Size == 0) {
        return 0;
    }

    // Step 1: Prepare environment and invoke magic_open
    // Use different flags to explore more states
    int flags = MAGIC_ERROR;
    if (Size > 0) {
        flags = (Data[0] & 0x0F) | MAGIC_ERROR;
        if (flags == 0) flags = MAGIC_ERROR;
    }

    cookie = magic_open(flags);
    if (cookie == NULL) {
        return 0;
    }

    // Step 2: Invoke magic_load
    // Attempt to load a default database, or a dummy one if needed
    // Usually, libmagic looks for the system database.
    // We pass NULL to let it use the default, or a specific path if configured.
    // For fuzzing, we rely on the system default or a pre-configured one.
    // If the system doesn't have a default, this might fail, which is fine for fuzzing.
    ret = magic_load(cookie, NULL);
    
    // Step 3: Invoke magic_error after magic_load to check for errors
    err_msg = magic_error(cookie);
    (void)err_msg; // Suppress unused variable warning if not used in logic

    // Step 4: Prepare file for magic_file
    // Write the fuzz input to a dummy file
    write_dummy_file(Data, Size);

    // Step 5: Invoke magic_file
    // This attempts to identify the type of "./dummy_file"
    file_type = magic_file(cookie, "./dummy_file");
    (void)file_type; // Result is not critical for the driver, just execution path

    // Step 6: Invoke magic_error again to check for errors from magic_file
    err_msg = magic_error(cookie);
    (void)err_msg;

    // Step 7: Cleanup with magic_close
    magic_close(cookie);

    return 0;
}