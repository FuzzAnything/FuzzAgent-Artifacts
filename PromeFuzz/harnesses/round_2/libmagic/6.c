// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    int flags = Data[0] & 0xFF;
    
    magic_t cookie = magic_open(flags);
    if (cookie) {
        magic_load(cookie, NULL);
        magic_load(cookie, "./dummy_file");
        
        magic_compile(cookie, "./dummy_file");
        
        const char *error = magic_error(cookie);
        if (error) {
            volatile const char *err_copy = error;
            (void)err_copy;
        }
        
        magic_close(cookie);
    }
    
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        
        const char *error = magic_error(check_cookie);
        if (error) {
            volatile const char *err_copy = error;
            (void)err_copy;
        }
        
        magic_close(check_cookie);
    }
    
    magic_t list_cookie = magic_open(flags);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        
        const char *error = magic_error(list_cookie);
        if (error) {
            volatile const char *err_copy = error;
            (void)err_copy;
        }
        
        magic_close(list_cookie);
    }
    
    remove_dummy_file();
    return 0;
}