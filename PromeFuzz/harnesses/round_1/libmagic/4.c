// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for functions that need it
    write_dummy_file(Data, Size);
    
    // Test magic_open with various flags
    int flags[] = {0, MAGIC_NONE, MAGIC_DEBUG, MAGIC_SYMLINK, 
                   MAGIC_COMPRESS, MAGIC_DEVICES, MAGIC_MIME_TYPE,
                   MAGIC_CONTINUE, MAGIC_CHECK, MAGIC_PRESERVE_ATIME,
                   MAGIC_RAW, MAGIC_ERROR, MAGIC_MIME_ENCODING,
                   MAGIC_MIME | MAGIC_CONTINUE, MAGIC_APPLE, MAGIC_EXTENSION};
    
    for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
        magic_t cookie = magic_open(flags[i]);
        if (cookie) {
            // Test magic_load with NULL (default database)
            magic_load(cookie, NULL);
            
            // Test magic_load with dummy file as database
            magic_load(cookie, "./dummy_file");
            
            // Test magic_compile with dummy file
            magic_compile(cookie, "./dummy_file");
            
            // Test magic_error after operations
            const char *error = magic_error(cookie);
            (void)error; // Use error to avoid unused variable warning
            
            magic_close(cookie);
        }
    }
    
    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(MAGIC_NONE);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        magic_close(check_cookie);
    }
    
    // Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(MAGIC_NONE);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        magic_close(list_cookie);
    }
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}