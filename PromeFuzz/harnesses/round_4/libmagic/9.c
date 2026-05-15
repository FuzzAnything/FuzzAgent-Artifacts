// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <unistd.h>
#include <fcntl.h>

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize magic handle
    magic_t cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Try to load magic database - use default paths
    if (magic_load(cookie, NULL) != 0) {
        // If loading fails, we can still test some functions
        // but magic_buffer will likely return NULL
    }

    // Use fuzz data to generate parameters
    if (Size >= sizeof(size_t)) {
        size_t param_value;
        memcpy(&param_value, Data, sizeof(size_t));
        
        // Test magic_setparam with various parameter IDs
        // Use modulo to get valid-ish parameter IDs (0-7 are likely valid)
        int param_id = Data[0] % 8;
        magic_setparam(cookie, param_id, &param_value);
        
        // Test magic_getparam
        size_t get_value;
        magic_getparam(cookie, param_id, &get_value);
    }

    // Test magic_setflags with fuzz-derived flags
    if (Size >= 2) {
        int flags = Data[1];
        magic_setflags(cookie, flags);
    }

    // Test magic_getflags
    magic_getflags(cookie);

    // Test magic_buffer with the fuzz data
    if (Size > 0) {
        const char *result = magic_buffer(cookie, Data, Size);
        // result may be NULL, that's OK for fuzzing
        (void)result;
    }

    // Test magic_error - check if there were any errors
    const char *error_msg = magic_error(cookie);
    (void)error_msg;

    // Clean up
    magic_close(cookie);

    return 0;
}