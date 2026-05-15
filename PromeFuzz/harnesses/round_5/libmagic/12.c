// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to decide which function to test
    uint8_t selector = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Write remaining data to dummy file for functions that need it
    write_dummy_file(remaining_data, remaining_size);
    
    // Test different API functions based on selector
    switch (selector % 6) {
        case 0: {  // magic_list
            magic_t cookie = magic_open(MAGIC_NONE);
            if (cookie) {
                magic_list(cookie, "./dummy_file");
                const char *err = magic_error(cookie);
                (void)err;  // Use error to avoid unused variable warning
                magic_close(cookie);
            }
            break;
        }
        
        case 1: {  // magic_error
            magic_t cookie = magic_open(MAGIC_NONE);
            if (cookie) {
                // Try to load invalid data to generate error
                magic_load(cookie, "./dummy_file");
                const char *err = magic_error(cookie);
                (void)err;
                magic_close(cookie);
            }
            break;
        }
        
        case 2: {  // magic_load
            magic_t cookie = magic_open(MAGIC_NONE);
            if (cookie) {
                magic_load(cookie, "./dummy_file");
                const char *err = magic_error(cookie);
                (void)err;
                magic_close(cookie);
            }
            break;
        }
        
        case 3: {  // magic_check
            magic_t cookie = magic_open(MAGIC_NONE);
            if (cookie) {
                magic_check(cookie, "./dummy_file");
                const char *err = magic_error(cookie);
                (void)err;
                magic_close(cookie);
            }
            break;
        }
        
        case 4: {  // magic_open with various flags
            // Use remaining data to generate flags
            int flags = 0;
            if (remaining_size > 0) {
                flags = remaining_data[0] % 16;  // Limit to reasonable flag range
            }
            magic_t cookie = magic_open(flags);
            if (cookie) {
                const char *err = magic_error(cookie);
                (void)err;
                magic_close(cookie);
            }
            break;
        }
        
        case 5: {  // magic_compile
            magic_t cookie = magic_open(MAGIC_NONE);
            if (cookie) {
                magic_compile(cookie, "./dummy_file");
                const char *err = magic_error(cookie);
                (void)err;
                magic_close(cookie);
            }
            break;
        }
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}