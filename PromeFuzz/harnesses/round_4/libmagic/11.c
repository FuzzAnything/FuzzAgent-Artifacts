// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
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
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write fuzzer input to dummy file for functions that need a file
    write_dummy_file(Data, Size);

    // Test magic_load
    magic_t cookie = magic_open(0);
    if (cookie) {
        // Try loading with NULL (default database)
        magic_load(cookie, NULL);
        
        // Try loading with dummy file
        magic_load(cookie, "./dummy_file");
        
        // Check for errors
        magic_errno(cookie);
        
        magic_close(cookie);
    }

    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        magic_errno(check_cookie);
        magic_close(check_cookie);
    }

    // Test magic_getpath
    // Use first byte as action flag, rest as path string if available
    int action = 0;
    const char *path = NULL;
    
    if (Size > 1) {
        // Use first byte for action
        action = Data[0] & 1; // 0 or 1
        
        // Use remaining bytes as path string (null-terminated)
        char *path_buf = malloc(Size);
        if (path_buf) {
            memcpy(path_buf, Data + 1, Size - 1);
            path_buf[Size - 1] = '\0';
            path = path_buf;
        }
    }
    
    magic_getpath(path, action);
    
    if (Size > 1) {
        free((void*)path);
    }

    // Test magic_compile
    magic_t compile_cookie = magic_open(0);
    if (compile_cookie) {
        magic_compile(compile_cookie, "./dummy_file");
        magic_errno(compile_cookie);
        magic_close(compile_cookie);
    }

    // Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(0);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        magic_errno(list_cookie);
        magic_close(list_cookie);
    }

    // Clean up
    remove_dummy_file();
    
    return 0;
}