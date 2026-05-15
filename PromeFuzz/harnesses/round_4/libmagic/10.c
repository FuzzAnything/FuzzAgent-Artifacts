// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

static void write_to_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    write_to_dummy_file(Data, Size);
    
    // Create main magic cookie
    magic_t cookie = magic_open(0);
    if (cookie == NULL) return 0;
    
    // Use first byte to select which functions to test
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            // Test magic_load with various paths
            const char *paths[] = {NULL, "./dummy_file", "/etc/magic"};
            for (int i = 0; i < 3; i++) {
                magic_load(cookie, paths[i]);
            }
            break;
        }
        case 1: {
            // Test magic_check with separate magic_set
            magic_t check_cookie = magic_open(0);
            if (check_cookie) {
                const char *paths[] = {NULL, "./dummy_file", "/etc/magic"};
                for (int i = 0; i < 3; i++) {
                    magic_check(check_cookie, paths[i]);
                }
                magic_close(check_cookie);
            }
            break;
        }
        case 2: {
            // Test magic_descriptor with file descriptor
            int fd = open("./dummy_file", O_RDONLY);
            if (fd >= 0) {
                const char *result = magic_descriptor(cookie, fd);
                (void)result; // Use result to avoid unused variable warning
                close(fd);
            }
            break;
        }
        case 3: {
            // Test magic_compile
            const char *paths[] = {NULL, "./dummy_file", "/etc/magic"};
            for (int i = 0; i < 3; i++) {
                magic_compile(cookie, paths[i]);
            }
            break;
        }
        case 4: {
            // Test magic_errno after various operations
            magic_load(cookie, NULL);
            int err = magic_errno(cookie);
            (void)err; // Use err to avoid unused variable warning
            
            magic_compile(cookie, "./dummy_file");
            err = magic_errno(cookie);
            (void)err;
            break;
        }
        case 5: {
            // Test magic_list with separate magic_set
            magic_t list_cookie = magic_open(0);
            if (list_cookie) {
                const char *paths[] = {NULL, "./dummy_file", "/etc/magic"};
                for (int i = 0; i < 3; i++) {
                    magic_list(list_cookie, paths[i]);
                }
                magic_close(list_cookie);
            }
            break;
        }
    }
    
    // Clean up
    magic_close(cookie);
    unlink("./dummy_file");
    
    return 0;
}