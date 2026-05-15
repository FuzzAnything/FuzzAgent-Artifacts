// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
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

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine flags
    int flags = Data[0];
    const uint8_t *db_data = Data + 1;
    size_t db_size = Size - 1;
    
    // Write database content to dummy file
    write_dummy_file(db_data, db_size);
    
    // Test magic_open
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        remove_dummy_file();
        return 0;
    }
    
    // Test magic_load with dummy file
    if (db_size > 0) {
        magic_load(cookie, "./dummy_file");
        
        // Test magic_error after load
        const char *error = magic_error(cookie);
        (void)error; // Use error to avoid unused variable warning
    }
    
    // Test magic_compile with dummy file
    if (db_size > 0) {
        magic_compile(cookie, "./dummy_file");
        
        // Test magic_error after compile
        const char *error = magic_error(cookie);
        (void)error;
    }
    
    // Test magic_check with separate cookie
    if (db_size > 0) {
        magic_t check_cookie = magic_open(flags);
        if (check_cookie) {
            magic_check(check_cookie, "./dummy_file");
            
            // Test magic_error after check
            const char *error = magic_error(check_cookie);
            (void)error;
            
            magic_close(check_cookie);
        }
    }
    
    // Test magic_list with separate cookie
    if (db_size > 0) {
        magic_t list_cookie = magic_open(flags);
        if (list_cookie) {
            magic_list(list_cookie, "./dummy_file");
            
            // Test magic_error after list
            const char *error = magic_error(list_cookie);
            (void)error;
            
            magic_close(list_cookie);
        }
    }
    
    // Clean up
    magic_close(cookie);
    remove_dummy_file();
    
    return 0;
}