// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Use first byte for flags
    int flags = Size > 0 ? Data[0] : 0;
    
    // Create main magic cookie
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        return 0;
    }
    
    // Test magic_load with dummy file
    if (write_dummy_file(Data, Size) == 0) {
        magic_load(cookie, "./dummy_file");
        // Get error if any
        const char *err = magic_error(cookie);
        (void)err; // Use error to avoid unused variable warning
        unlink("./dummy_file");
    }
    
    // Test magic_load with NULL (default database)
    magic_load(cookie, NULL);
    
    // Test magic_load_buffers
    if (Size > 0) {
        void *buffer = (void *)Data;
        size_t buffer_size = Size;
        void *buffers[1] = {buffer};
        size_t sizes[1] = {buffer_size};
        magic_load_buffers(cookie, buffers, sizes, 1);
    }
    
    // Test magic_check with separate cookie
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        if (write_dummy_file(Data, Size) == 0) {
            magic_check(check_cookie, "./dummy_file");
            const char *check_err = magic_error(check_cookie);
            (void)check_err;
            unlink("./dummy_file");
        }
        magic_close(check_cookie);
    }
    
    // Test magic_list with separate cookie
    magic_t list_cookie = magic_open(flags);
    if (list_cookie) {
        if (write_dummy_file(Data, Size) == 0) {
            magic_list(list_cookie, "./dummy_file");
            const char *list_err = magic_error(list_cookie);
            (void)list_err;
            unlink("./dummy_file");
        }
        magic_close(list_cookie);
    }
    
    // Clean up main cookie
    magic_close(cookie);
    
    return 0;
}