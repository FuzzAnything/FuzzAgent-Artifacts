// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include "magic.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return -1;
    }
    fwrite(data, 1, size, fp);
    fclose(fp);
    return 0;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    // Write fuzz data to dummy file
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }
    
    // Create main magic handle
    magic_t ms = magic_open(MAGIC_NONE);
    if (!ms) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Test magic_load with default database
    if (magic_load(ms, NULL) == -1) {
        // If load fails, check error functions
        const char *err = magic_error(ms);
        int errno_val = magic_errno(ms);
        (void)err;
        (void)errno_val;
    }
    
    // Test magic_file
    const char *result = magic_file(ms, "./dummy_file");
    if (result) {
        // Do nothing with result, just ensure call succeeded
        (void)result;
    } else {
        // Check error on failure
        const char *err = magic_error(ms);
        int errno_val = magic_errno(ms);
        (void)err;
        (void)errno_val;
    }
    
    // Test magic_descriptor
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        const char *desc_result = magic_descriptor(ms, fd);
        if (desc_result) {
            (void)desc_result;
        } else {
            const char *err = magic_error(ms);
            int errno_val = magic_errno(ms);
            (void)err;
            (void)errno_val;
        }
        close(fd);
    }
    
    // Test magic_check with separate handle
    magic_t check_ms = magic_open(MAGIC_CHECK);
    if (check_ms) {
        int check_result = magic_check(check_ms, NULL);
        (void)check_result;
        // Check error functions after magic_check
        const char *check_err = magic_error(check_ms);
        int check_errno = magic_errno(check_ms);
        (void)check_err;
        (void)check_errno;
        magic_close(check_ms);
    }
    
    // Cleanup
    magic_close(ms);
    unlink("./dummy_file");
    
    return 0;
}