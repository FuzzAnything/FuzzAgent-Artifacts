// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
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

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to choose which function to test
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: { // magic_list
            magic_t ms = magic_open(0);
            if (ms) {
                write_dummy_file(Data, Size);
                magic_list(ms, "./dummy_file");
                magic_close(ms);
                cleanup_dummy_file();
            }
            break;
        }
        
        case 1: { // magic_load
            magic_t ms = magic_open(0);
            if (ms) {
                write_dummy_file(Data, Size);
                magic_load(ms, "./dummy_file");
                magic_close(ms);
                cleanup_dummy_file();
            }
            break;
        }
        
        case 2: { // magic_check
            magic_t ms = magic_open(0);
            if (ms) {
                write_dummy_file(Data, Size);
                magic_check(ms, "./dummy_file");
                magic_close(ms);
                cleanup_dummy_file();
            }
            break;
        }
        
        case 3: { // magic_open
            int flags = Data[1 % Size] & 0xFF;
            magic_t ms = magic_open(flags);
            if (ms) {
                magic_close(ms);
            }
            break;
        }
        
        case 4: { // magic_file
            magic_t ms = magic_open(0);
            if (ms) {
                // Load default database
                magic_load(ms, NULL);
                write_dummy_file(Data, Size);
                const char *result = magic_file(ms, "./dummy_file");
                // Result can be NULL on error, that's OK
                (void)result;
                magic_close(ms);
                cleanup_dummy_file();
            }
            break;
        }
        
        case 5: { // magic_error
            magic_t ms = magic_open(0);
            if (ms) {
                // Try to cause an error by loading invalid data
                write_dummy_file(Data, Size);
                magic_load(ms, "./dummy_file");
                const char *error = magic_error(ms);
                // Error can be NULL if no error occurred, that's OK
                (void)error;
                magic_close(ms);
                cleanup_dummy_file();
            }
            break;
        }
    }
    
    return 0;
}