// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
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
#include <unistd.h>
#include <fcntl.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzz data to dummy file
    write_dummy_file(Data, Size);
    
    // Create main magic handle
    magic_t ms = magic_open(MAGIC_NONE);
    if (!ms) {
        remove_dummy_file();
        return 0;
    }
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    
    // Test magic_load with different scenarios
    if (selector & 0x01) {
        // Try loading with NULL (default database)
        magic_load(ms, NULL);
    } else {
        // Try loading with dummy path (will likely fail)
        magic_load(ms, "./dummy_file");
    }
    
    // Test magic_file
    const char *result = magic_file(ms, "./dummy_file");
    (void)result; // Use result to avoid unused variable warning
    
    // Test magic_descriptor
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        result = magic_descriptor(ms, fd);
        (void)result;
        close(fd);
    }
    
    // Test magic_error and magic_errno
    const char *error_msg = magic_error(ms);
    (void)error_msg;
    int err_code = magic_errno(ms);
    (void)err_code;
    
    // Test magic_check with separate handle
    magic_t check_ms = magic_open(MAGIC_CHECK);
    if (check_ms) {
        if (selector & 0x02) {
            magic_check(check_ms, NULL);
        } else {
            magic_check(check_ms, "./dummy_file");
        }
        magic_close(check_ms);
    }
    
    // Clean up
    magic_close(ms);
    remove_dummy_file();
    
    return 0;
}