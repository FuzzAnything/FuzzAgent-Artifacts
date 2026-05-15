// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_file at magic.c:414:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
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

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize main magic handle
    magic_t cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Use first byte to decide which function to test
    uint8_t selector = 0;
    if (Size > 0) {
        selector = Data[0] % 6;
        Data++;
        Size--;
    }

    switch (selector) {
        case 0: { // magic_descriptor
            write_dummy_file(Data, Size);
            int fd = open("./dummy_file", O_RDONLY);
            if (fd >= 0) {
                const char *result = magic_descriptor(cookie, fd);
                (void)result; // Use result to avoid unused warning
                close(fd);
            }
            remove_dummy_file();
            break;
        }
        
        case 1: { // magic_load
            // Try loading with default database (NULL)
            magic_load(cookie, NULL);
            
            // Also try loading with dummy file if we have data
            if (Size > 0) {
                write_dummy_file(Data, Size);
                magic_load(cookie, "./dummy_file");
                remove_dummy_file();
            }
            break;
        }
        
        case 2: { // magic_buffer
            if (Size > 0) {
                const char *result = magic_buffer(cookie, Data, Size);
                (void)result;
            }
            break;
        }
        
        case 3: { // magic_check
            // Create separate magic handle as required
            magic_t check_cookie = magic_open(0);
            if (check_cookie) {
                if (Size > 0) {
                    write_dummy_file(Data, Size);
                    magic_check(check_cookie, "./dummy_file");
                    remove_dummy_file();
                } else {
                    // Try with NULL
                    magic_check(check_cookie, NULL);
                }
                magic_close(check_cookie);
            }
            break;
        }
        
        case 4: { // magic_file
            if (Size > 0) {
                write_dummy_file(Data, Size);
                const char *result = magic_file(cookie, "./dummy_file");
                (void)result;
                remove_dummy_file();
            } else {
                // Try with NULL (reads from stdin)
                const char *result = magic_file(cookie, NULL);
                (void)result;
            }
            break;
        }
        
        case 5: { // magic_error
            // Call some operation that might set error
            if (Size > 0) {
                const char *result = magic_buffer(cookie, Data, Size);
                (void)result;
            }
            // Then get error
            const char *error = magic_error(cookie);
            (void)error;
            break;
        }
    }

    // Clean up
    magic_close(cookie);
    
    return 0;
}