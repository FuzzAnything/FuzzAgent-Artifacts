// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "magic.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return -1;
    }
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Write fuzzer input to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Use first byte to determine flags for magic_open
    int flags = Data[0] & 0xFF;
    
    // Test magic_open
    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        // If magic_open fails, we can still test magic_check with a separate handle
        magic_t check_cookie = magic_open(MAGIC_NONE);
        if (check_cookie) {
            // Test magic_check with dummy file as database
            magic_check(check_cookie, "./dummy_file");
            magic_error(check_cookie);
            magic_close(check_cookie);
        }
        return 0;
    }

    // Test magic_load with dummy file as database
    int load_ret = magic_load(cookie, "./dummy_file");
    
    // Test magic_error after load
    const char *error_msg = magic_error(cookie);
    
    // Test magic_file with dummy file
    const char *file_type = magic_file(cookie, "./dummy_file");
    
    // Test magic_error after file check
    error_msg = magic_error(cookie);

    // Test magic_check with a SEPARATE handle as required
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        int check_ret = magic_check(check_cookie, "./dummy_file");
        error_msg = magic_error(check_cookie);
        magic_close(check_cookie);
    }

    // Clean up
    magic_close(cookie);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}