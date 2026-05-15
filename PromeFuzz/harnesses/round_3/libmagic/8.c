// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
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

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Write fuzz data to dummy file for magic_file
    write_dummy_file(Data, Size);
    
    // Test magic_open with various flags
    int flags[] = {MAGIC_NONE, MAGIC_DEBUG, MAGIC_SYMLINK, MAGIC_COMPRESS,
                   MAGIC_DEVICES, MAGIC_MIME_TYPE, MAGIC_CONTINUE,
                   MAGIC_CHECK, MAGIC_PRESERVE_ATIME, MAGIC_RAW,
                   MAGIC_ERROR, MAGIC_MIME_ENCODING, MAGIC_MIME,
                   MAGIC_APPLE, MAGIC_EXTENSION, MAGIC_COMPRESS_TRANSP};
    
    for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
        magic_t cookie = magic_open(flags[i]);
        if (cookie == NULL) {
            continue;
        }
        
        // Test magic_load with NULL (default database)
        int load_ret = magic_load(cookie, NULL);
        
        // Test magic_file with dummy file
        const char *file_result = magic_file(cookie, "./dummy_file");
        if (file_result == NULL) {
            // Get error if magic_file failed
            const char *error = magic_error(cookie);
            (void)error; // Use error to avoid unused variable warning
        }
        
        // Test magic_buffer with fuzz data
        const char *buffer_result = magic_buffer(cookie, Data, Size);
        if (buffer_result == NULL) {
            const char *error = magic_error(cookie);
            (void)error;
        }
        
        // Clean up
        magic_close(cookie);
    }
    
    // Test magic_check with separate magic_set
    for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
        magic_t check_cookie = magic_open(flags[i]);
        if (check_cookie == NULL) {
            continue;
        }
        
        // Test with NULL database path (default)
        int check_ret = magic_check(check_cookie, NULL);
        (void)check_ret;
        
        // Test with specific database path (may fail)
        check_ret = magic_check(check_cookie, "/etc/magic");
        (void)check_ret;
        
        // Get error if magic_check failed
        const char *error = magic_error(check_cookie);
        (void)error;
        
        magic_close(check_cookie);
    }
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}