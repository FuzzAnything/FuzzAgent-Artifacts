// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_version at magic.c:607:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file for functions that need it
    write_dummy_file(Data, Size);
    
    // 1. Test magic_version
    (void)magic_version();
    
    // 2. Test magic_getpath with various inputs
    const char *path1 = magic_getpath(NULL, 0);
    (void)path1;
    
    if (Size > 0) {
        // Use first byte to determine path length (max 255)
        size_t path_len = Data[0] % 256;
        if (path_len > Size - 1) path_len = Size - 1;
        if (path_len > 0) {
            char *custom_path = (char*)malloc(path_len + 1);
            if (custom_path) {
                memcpy(custom_path, Data + 1, path_len);
                custom_path[path_len] = '\0';
                const char *path2 = magic_getpath(custom_path, 0);
                (void)path2;
                free(custom_path);
            }
        }
    }
    
    // 3. Test magic_load with main magic handle
    magic_t magic_cookie = magic_open(0);
    if (magic_cookie) {
        // Try loading with NULL (default database)
        magic_load(magic_cookie, NULL);
        
        // Try loading with dummy file
        magic_load(magic_cookie, "./dummy_file");
        
        // Clean up
        magic_close(magic_cookie);
    }
    
    // 4. Test magic_check with separate magic handle
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        // Check default database
        magic_check(check_cookie, NULL);
        
        // Check dummy file
        magic_check(check_cookie, "./dummy_file");
        
        // Clean up
        magic_close(check_cookie);
    }
    
    // 5. Test magic_compile with main magic handle
    magic_t compile_cookie = magic_open(0);
    if (compile_cookie) {
        // Try compiling dummy file
        magic_compile(compile_cookie, "./dummy_file");
        
        // Clean up
        magic_close(compile_cookie);
    }
    
    // 6. Test magic_list with separate magic handle
    magic_t list_cookie = magic_open(0);
    if (list_cookie) {
        // List entries from default database
        magic_list(list_cookie, NULL);
        
        // List entries from dummy file
        magic_list(list_cookie, "./dummy_file");
        
        // Clean up
        magic_close(list_cookie);
    }
    
    // Remove dummy file if it exists
    remove("./dummy_file");
    
    return 0;
}