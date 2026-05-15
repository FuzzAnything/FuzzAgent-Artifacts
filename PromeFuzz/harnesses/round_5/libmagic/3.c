// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getpath at magic.c:254:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "src/magic.h"
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

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Write fuzzing data to dummy file
    write_dummy_file(Data, Size);
    
    // 1. magic_getpath
    const char *path_result = magic_getpath(NULL, 0);
    
    // 2. magic_version (first call)
    int version1 = magic_version();
    
    // 3. magic_version (second call)
    int version2 = magic_version();
    
    // 4. magic_open
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] % 256; // Use first byte for flags
    }
    magic_t cookie = magic_open(flags);
    
    if (cookie != NULL) {
        // 5. magic_check (requires separate magic_set)
        magic_t check_cookie = magic_open(flags);
        if (check_cookie != NULL) {
            magic_check(check_cookie, "./dummy_file");
            magic_close(check_cookie);
        }
        
        // 6. magic_compile
        magic_compile(cookie, "./dummy_file");
        
        // 7. magic_list (requires separate magic_set)
        magic_t list_cookie = magic_open(flags);
        if (list_cookie != NULL) {
            magic_list(list_cookie, "./dummy_file");
            magic_close(list_cookie);
        }
        
        // 8. magic_error
        const char *error_msg = magic_error(cookie);
        
        // 9. magic_close
        magic_close(cookie);
    }
    
    // Cleanup
    remove_dummy_file();
    
    return 0;
}