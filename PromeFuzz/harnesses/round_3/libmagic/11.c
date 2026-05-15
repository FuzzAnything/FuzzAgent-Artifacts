// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
    if (Size == 0) return 0;
    
    write_dummy_file(Data, Size);
    
    // Extract a null-terminated string from the input for path parameters
    char *path_str = malloc(Size + 1);
    if (!path_str) return 0;
    memcpy(path_str, Data, Size);
    path_str[Size] = '\0';
    
    // Test magic_open with various flags
    int flags = Data[0] % 256;
    magic_t cookie = magic_open(flags);
    if (cookie) {
        // Test magic_load with different paths
        magic_load(cookie, NULL);
        magic_load(cookie, "./dummy_file");
        magic_load(cookie, path_str);
        
        // Test magic_compile
        magic_compile(cookie, NULL);
        magic_compile(cookie, "./dummy_file");
        magic_compile(cookie, path_str);
        
        magic_close(cookie);
    }
    
    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        magic_check(check_cookie, NULL);
        magic_check(check_cookie, "./dummy_file");
        magic_check(check_cookie, path_str);
        magic_close(check_cookie);
    }
    
    // Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(0);
    if (list_cookie) {
        magic_list(list_cookie, NULL);
        magic_list(list_cookie, "./dummy_file");
        magic_list(list_cookie, path_str);
        magic_close(list_cookie);
    }
    
    // Test magic_getpath with various inputs
    magic_getpath(NULL, 0);
    magic_getpath("./dummy_file", 0);
    magic_getpath(path_str, 0);
    magic_getpath(NULL, Data[0] % 256);
    magic_getpath("./dummy_file", Data[0] % 256);
    magic_getpath(path_str, Data[0] % 256);
    
    free(path_str);
    remove_dummy_file();
    
    return 0;
}