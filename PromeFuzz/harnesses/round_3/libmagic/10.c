// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write fuzzer data to dummy file for functions that need it
    write_dummy_file(Data, Size);
    
    // Test magic_open with various flags
    int flags = Data[0] % 256;
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        // Try with different flags if first fails
        cookie = magic_open(MAGIC_NONE);
    }
    
    if (cookie) {
        // Test magic_load with different database paths
        const char *db_path = NULL;
        if (Size > 1 && (Data[1] % 2)) {
            db_path = "./dummy_file";
        }
        int load_result = magic_load(cookie, db_path);
        
        // Test magic_error after load
        if (load_result == -1) {
            const char *error_msg = magic_error(cookie);
            (void)error_msg; // Use to avoid unused variable warning
        }
        
        // Test magic_compile
        if (Size > 2 && (Data[2] % 2)) {
            const char *compile_path = "./dummy_file";
            magic_compile(cookie, compile_path);
        }
        
        // Clean up first cookie
        magic_close(cookie);
    }
    
    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(MAGIC_NONE);
    if (check_cookie) {
        const char *check_path = NULL;
        if (Size > 3 && (Data[3] % 2)) {
            check_path = "./dummy_file";
        }
        int check_result = magic_check(check_cookie, check_path);
        
        // Test magic_error after check
        if (check_result == -1) {
            const char *error_msg = magic_error(check_cookie);
            (void)error_msg;
        }
        
        magic_close(check_cookie);
    }
    
    // Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(MAGIC_NONE);
    if (list_cookie) {
        const char *list_path = NULL;
        if (Size > 4 && (Data[4] % 2)) {
            list_path = "./dummy_file";
        }
        int list_result = magic_list(list_cookie, list_path);
        
        // Test magic_error after list
        if (list_result == -1) {
            const char *error_msg = magic_error(list_cookie);
            (void)error_msg;
        }
        
        magic_close(list_cookie);
    }
    
    // Remove dummy file
    remove("./dummy_file");
    
    return 0;
}