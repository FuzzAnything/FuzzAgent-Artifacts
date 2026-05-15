// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // Test magic_load
    magic_t cookie = magic_open(0);
    if (cookie) {
        // Try loading with NULL (default database)
        magic_load(cookie, NULL);
        
        // Try loading with dummy file as database
        magic_load(cookie, "./dummy_file");
        
        // Test magic_setflags with various flag values from input
        int flags = Data[0];
        magic_setflags(cookie, flags);
        
        // Test magic_errno and magic_error
        magic_errno(cookie);
        const char *err = magic_error(cookie);
        (void)err; // Use to avoid unused variable warning
        
        magic_close(cookie);
    }
    
    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        magic_errno(check_cookie);
        const char *check_err = magic_error(check_cookie);
        (void)check_err;
        magic_close(check_cookie);
    }
    
    // Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(0);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        magic_errno(list_cookie);
        const char *list_err = magic_error(list_cookie);
        (void)list_err;
        magic_close(list_cookie);
    }
    
    remove_dummy_file();
    return 0;
}