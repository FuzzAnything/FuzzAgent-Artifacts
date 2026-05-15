// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Write fuzzer data to dummy file
    write_dummy_file(Data, Size);

    // Test magic_open with various flags
    int flags = Data[0] & 0xFF;
    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        remove_dummy_file();
        return 0;
    }

    // Test magic_load with default database
    magic_load(cookie, NULL);
    
    // Test magic_load with dummy file as database
    magic_load(cookie, "./dummy_file");
    
    // Test magic_file with dummy file
    const char *result = magic_file(cookie, "./dummy_file");
    (void)result; // Use result to avoid unused variable warning
    
    // Test magic_error
    const char *error = magic_error(cookie);
    (void)error; // Use error to avoid unused variable warning

    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(flags);
    if (check_cookie != NULL) {
        magic_check(check_cookie, "./dummy_file");
        magic_close(check_cookie);
    }

    // Clean up main cookie
    magic_close(cookie);

    // Test magic_open with different flags
    int alt_flags = (Data[0] >> 8) & 0xFF;
    magic_t alt_cookie = magic_open(alt_flags);
    if (alt_cookie != NULL) {
        // Test with empty database path
        magic_load(alt_cookie, "");
        
        // Test magic_file with NULL (stdin simulation)
        // We'll write to stdin via file descriptor 0
        int saved_stdin = dup(0);
        int pipefd[2];
        if (pipe(pipefd) == 0) {
            write(pipefd[1], Data, Size < 1024 ? Size : 1024);
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
            
            magic_file(alt_cookie, NULL);
            
            dup2(saved_stdin, 0);
            close(saved_stdin);
        }
        
        magic_close(alt_cookie);
    }

    // Test edge case: magic_close with NULL
    magic_close(NULL);

    // Clean up dummy file
    remove_dummy_file();
    
    return 0;
}