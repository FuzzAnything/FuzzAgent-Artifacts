// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
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
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize magic cookie
    magic_t cookie = magic_open(MAGIC_NONE);
    if (!cookie) return 0;
    
    // Use first byte to determine fuzzing mode
    uint8_t mode = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (mode % 6) {
        case 0: {
            // Test magic_getflags
            int flags = magic_getflags(cookie);
            (void)flags; // Use result to avoid unused warning
            break;
        }
        case 1: {
            // Test magic_error
            const char *error = magic_error(cookie);
            (void)error; // Use result to avoid unused warning
            break;
        }
        case 2: {
            // Test magic_setflags with fuzzed flags
            if (fuzz_size >= sizeof(int)) {
                int flags;
                memcpy(&flags, fuzz_data, sizeof(int));
                magic_setflags(cookie, flags);
            }
            break;
        }
        case 3: {
            // Test magic_load with dummy file
            write_dummy_file(fuzz_data, fuzz_size);
            magic_load(cookie, "./dummy_file");
            remove_dummy_file();
            break;
        }
        case 4: {
            // Test magic_setparam
            if (fuzz_size >= sizeof(int) + sizeof(size_t)) {
                int param_id;
                size_t param_value;
                memcpy(&param_id, fuzz_data, sizeof(int));
                memcpy(&param_value, fuzz_data + sizeof(int), sizeof(size_t));
                magic_setparam(cookie, param_id, &param_value);
            }
            break;
        }
        case 5: {
            // Test magic_getparam
            if (fuzz_size >= sizeof(int)) {
                int param_id;
                size_t param_value;
                memcpy(&param_id, fuzz_data, sizeof(int));
                magic_getparam(cookie, param_id, &param_value);
            }
            break;
        }
    }
    
    // Also test with NULL handle for some functions
    if (mode % 3 == 0) {
        magic_getflags(NULL);
        magic_error(NULL);
        magic_setflags(NULL, 0);
        magic_load(NULL, NULL);
        magic_setparam(NULL, 0, NULL);
        magic_getparam(NULL, 0, NULL);
    }
    
    // Test multiple combinations
    if (fuzz_size > 0) {
        // Set some flags then get them
        int test_flags = fuzz_data[0] % 256;
        magic_setflags(cookie, test_flags);
        int retrieved = magic_getflags(cookie);
        (void)retrieved;
        
        // Try to load with NULL (default database)
        if (fuzz_size > 1 && fuzz_data[1] % 2 == 0) {
            magic_load(cookie, NULL);
        }
        
        // Test error after failed operation
        if (fuzz_size > 2 && fuzz_data[2] % 2 == 0) {
            magic_load(cookie, "/nonexistent/path");
            const char *err = magic_error(cookie);
            (void)err;
        }
    }
    
    magic_close(cookie);
    return 0;
}