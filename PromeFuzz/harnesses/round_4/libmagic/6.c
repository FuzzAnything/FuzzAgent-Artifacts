// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
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

/* Include the required header file */
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    /* Write data to the dummy file for testing */
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        if (size > 0) {
            write(fd, data, size);
        }
        close(fd);
    }
}

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    /* Write fuzzer input to dummy file */
    write_dummy_file(Data, Size);
    
    /* Test magic_open with various flags derived from input */
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & 0xFF;  /* Use first byte for flags */
    }
    
    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        /* If magic_open fails, we can't proceed with other calls */
        return 0;
    }
    
    /* Test magic_load with default database */
    int load_result = magic_load(cookie, NULL);
    
    /* Test magic_file on the dummy file */
    const char *file_result = magic_file(cookie, "./dummy_file");
    
    /* Test magic_error to check for any errors */
    const char *error_msg = magic_error(cookie);
    
    /* Test magic_check with a SEPARATE magic_set as required */
    magic_t check_cookie = magic_open(flags);
    if (check_cookie != NULL) {
        int check_result = magic_check(check_cookie, NULL);
        /* Also test with the dummy file as database */
        if (Size > 0) {
            magic_check(check_cookie, "./dummy_file");
        }
        magic_close(check_cookie);
    }
    
    /* Clean up the main cookie */
    magic_close(cookie);
    
    /* Remove the dummy file to avoid accumulation */
    unlink("./dummy_file");
    
    return 0;
}