// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_getpath at magic.c:254:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Write fuzzer data to dummy file for functions that need a file
    write_dummy_file(Data, Size);
    
    // Use first byte for flags
    int flags = Data[0];
    
    // 1. Test magic_open
    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        cleanup_dummy_file();
        return 0;
    }
    
    // 2. Test magic_load with dummy file
    magic_load(cookie, "./dummy_file");
    
    // 3. Test magic_compile with dummy file
    magic_compile(cookie, "./dummy_file");
    
    // 4. Test magic_getpath
    const char *path = magic_getpath("./dummy_file", flags & 1);
    (void)path; // Use result to avoid unused variable warning
    
    // 5. Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(flags);
    if (check_cookie != NULL) {
        magic_check(check_cookie, "./dummy_file");
        magic_close(check_cookie);
    }
    
    // 6. Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(flags);
    if (list_cookie != NULL) {
        magic_list(list_cookie, "./dummy_file");
        magic_close(list_cookie);
    }
    
    // Cleanup
    magic_close(cookie);
    cleanup_dummy_file();
    
    return 0;
}