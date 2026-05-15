// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Write fuzzer data to dummy file
    write_dummy_file(Data, Size);
    
    // Create main magic handle
    magic_t cookie = magic_open(0);
    if (!cookie) {
        remove_dummy_file();
        return 0;
    }
    
    // Fuzz magic_setflags
    int flags = 0;
    if (Size >= sizeof(int)) {
        memcpy(&flags, Data, sizeof(int));
    } else if (Size > 0) {
        flags = Data[0];
    }
    magic_setflags(cookie, flags);
    
    // Check for errors after setflags
    const char *error_msg = magic_error(cookie);
    int error_code = magic_errno(cookie);
    (void)error_msg; // Use to avoid unused variable warning
    (void)error_code;
    
    // Fuzz magic_load with dummy file
    magic_load(cookie, "./dummy_file");
    
    // Check for errors after load
    error_msg = magic_error(cookie);
    error_code = magic_errno(cookie);
    (void)error_msg;
    (void)error_code;
    
    // Fuzz magic_check with separate handle
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        
        // Check for errors after check
        error_msg = magic_error(check_cookie);
        error_code = magic_errno(check_cookie);
        (void)error_msg;
        (void)error_code;
        
        magic_close(check_cookie);
    }
    
    // Fuzz magic_list with separate handle
    magic_t list_cookie = magic_open(0);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        
        // Check for errors after list
        error_msg = magic_error(list_cookie);
        error_code = magic_errno(list_cookie);
        (void)error_msg;
        (void)error_code;
        
        magic_close(list_cookie);
    }
    
    // Try magic_load with NULL (default database)
    magic_load(cookie, NULL);
    
    // Check for errors after NULL load
    error_msg = magic_error(cookie);
    error_code = magic_errno(cookie);
    (void)error_msg;
    (void)error_code;
    
    // Try different flag combinations
    if (Size > 1) {
        int alt_flags = Data[Size-1];
        magic_setflags(cookie, alt_flags);
        
        // Check for errors after alternate flags
        error_msg = magic_error(cookie);
        error_code = magic_errno(cookie);
        (void)error_msg;
        (void)error_code;
    }
    
    // Clean up
    magic_close(cookie);
    remove_dummy_file();
    
    return 0;
}