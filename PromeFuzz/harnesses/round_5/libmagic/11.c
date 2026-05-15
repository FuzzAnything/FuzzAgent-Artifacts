// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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
#include <sys/stat.h>

#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write data to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    // Create main magic handle
    magic_t ms = magic_open(0);
    if (!ms) return 0;
    
    // Use first byte to decide which operations to perform
    uint8_t selector = Data[0];
    
    // Try different operations based on selector
    switch (selector % 7) {
        case 0: {
            // Test magic_load with NULL (default database)
            magic_load(ms, NULL);
            break;
        }
        case 1: {
            // Test magic_load with dummy file as database
            magic_load(ms, "./dummy_file");
            break;
        }
        case 2: {
            // Test magic_file
            const char *result = magic_file(ms, "./dummy_file");
            (void)result; // Consume result
            break;
        }
        case 3: {
            // Test magic_descriptor
            int fd = open("./dummy_file", O_RDONLY);
            if (fd >= 0) {
                const char *result = magic_descriptor(ms, fd);
                (void)result; // Consume result
                close(fd);
            }
            break;
        }
        case 4: {
            // Test magic_error after potential error
            magic_load(ms, "./dummy_file"); // Might fail
            const char *err = magic_error(ms);
            (void)err; // Consume result
            break;
        }
        case 5: {
            // Test magic_errno after potential error
            magic_load(ms, "./dummy_file"); // Might fail
            int err = magic_errno(ms);
            (void)err; // Consume result
            break;
        }
        case 6: {
            // Test magic_check with separate magic handle
            magic_t check_ms = magic_open(0);
            if (check_ms) {
                magic_check(check_ms, "./dummy_file");
                magic_close(check_ms);
            }
            break;
        }
    }
    
    // Additional exploration: try magic_file with NULL (stdin)
    if ((selector >> 1) % 2 == 0) {
        const char *result = magic_file(ms, NULL);
        (void)result; // Consume result
    }
    
    // Additional exploration: try magic_load with various paths
    if ((selector >> 2) % 2 == 0 && Size > 10) {
        // Use part of input as path (null-terminated)
        size_t path_len = Size > 100 ? 100 : Size;
        char *path = malloc(path_len + 1);
        if (path) {
            memcpy(path, Data, path_len);
            path[path_len] = '\0';
            magic_load(ms, path);
            free(path);
        }
    }
    
    // Clean up
    magic_close(ms);
    remove_dummy_file();
    
    return 0;
}