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
#include <unistd.h>
#include "magic.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to a temporary file
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);

    // Open magic cookie with various flags based on input
    int flags = 0;
    if (Size > 0) {
        // Use first byte to determine some flags for diversity
        uint8_t flag_selector = Data[0];
        if (flag_selector & 0x01) flags |= MAGIC_NONE;
        if (flag_selector & 0x02) flags |= MAGIC_DEBUG;
        if (flag_selector & 0x04) flags |= MAGIC_SYMLINK;
        if (flag_selector & 0x08) flags |= MAGIC_COMPRESS;
        if (flag_selector & 0x10) flags |= MAGIC_DEVICES;
        if (flag_selector & 0x20) flags |= MAGIC_MIME_TYPE;
        if (flag_selector & 0x40) flags |= MAGIC_CONTINUE;
        if (flag_selector & 0x80) flags |= MAGIC_CHECK;
    } else {
        flags = MAGIC_NONE;
    }

    magic_t ms = magic_open(flags);
    if (ms == NULL) {
        unlink(filename);
        return 0;
    }

    // Load magic database - use NULL for default database
    int load_result = magic_load(ms, NULL);
    
    // Check for errors after load
    const char *error_msg = magic_error(ms);
    if (error_msg != NULL) {
        // Error occurred, but we continue fuzzing
    }

    // Identify the file type
    const char *result = magic_file(ms, filename);
    if (result != NULL) {
        // Result available, but we don't need to do anything with it for fuzzing
    }

    // Check for errors after file identification
    error_msg = magic_error(ms);
    if (error_msg != NULL) {
        // Error occurred, but we continue fuzzing
    }

    // Clean up
    magic_close(ms);
    unlink(filename);

    return 0;
}