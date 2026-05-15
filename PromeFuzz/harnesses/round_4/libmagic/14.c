// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getpath at magic.c:254:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
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

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create null-terminated string from fuzz data for path arguments
    char *path_str = malloc(Size + 1);
    if (!path_str) return 0;
    memcpy(path_str, Data, Size);
    path_str[Size] = '\0';
    
    // Write data to dummy file for functions that need it
    write_dummy_file(Data, Size);
    
    // Test magic_getpath with various inputs
    int action = Data[0] % 3;  // 0, 1, or 2
    const char *path_result = magic_getpath(path_str, action);
    (void)path_result;  // Use result to avoid compiler warning
    
    // Test magic_load with a magic cookie
    magic_t cookie = magic_open(MAGIC_NONE);
    if (cookie) {
        // Try loading with different paths
        if (Size % 2 == 0) {
            magic_load(cookie, NULL);  // Default database
        } else {
            magic_load(cookie, "./dummy_file");  // Custom file
        }
        
        // Test magic_compile if we have enough data
        if (Size > 10) {
            magic_compile(cookie, "./dummy_file");
        }
        
        // Test magic_errno after operations
        int err = magic_errno(cookie);
        (void)err;  // Use result
        
        magic_close(cookie);
    }
    
    // Test magic_check with a separate magic_set
    magic_t check_cookie = magic_open(MAGIC_NONE);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        int check_err = magic_errno(check_cookie);
        (void)check_err;
        magic_close(check_cookie);
    }
    
    // Test magic_list with a separate magic_set
    magic_t list_cookie = magic_open(MAGIC_NONE);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        int list_err = magic_errno(list_cookie);
        (void)list_err;
        magic_close(list_cookie);
    }
    
    // Clean up
    free(path_str);
    unlink("./dummy_file");
    
    return 0;
}