// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getpath at magic.c:254:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_version at magic.c:607:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // Write fuzz data to dummy file
    write_dummy_file(Data, Size);
    
    // 1. magic_getpath
    const char *path = magic_getpath(NULL, 0);
    
    // 2. magic_version (called twice as specified)
    int version1 = magic_version();
    int version2 = magic_version();
    
    // 3. magic_open
    magic_t cookie = magic_open(0);
    if (!cookie) {
        remove_dummy_file();
        return 0;
    }
    
    // 4. magic_check - requires separate magic_set
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        int check_result = magic_check(check_cookie, "./dummy_file");
        // 5. magic_compile
        int compile_result = magic_compile(cookie, "./dummy_file");
        magic_close(check_cookie);
    }
    
    // 6. magic_list - requires separate magic_set
    magic_t list_cookie = magic_open(0);
    if (list_cookie) {
        int list_result = magic_list(list_cookie, "./dummy_file");
        magic_close(list_cookie);
    }
    
    // 7. magic_error
    const char *error_msg = magic_error(cookie);
    
    // 8. magic_close
    magic_close(cookie);
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}