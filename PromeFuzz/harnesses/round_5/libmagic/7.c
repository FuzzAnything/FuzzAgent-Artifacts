// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
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
#include <sys/stat.h>
#include <sys/types.h>

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

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    // Write data to dummy file for functions that need it
    write_dummy_file(Data, Size);
    
    // Test magic_open with various flags
    int flags[] = {0, MAGIC_MIME, MAGIC_SYMLINK, MAGIC_COMPRESS, 
                   MAGIC_DEVICES, MAGIC_CONTINUE, MAGIC_CHECK};
    int num_flags = sizeof(flags) / sizeof(flags[0]);
    
    // Use first byte to select flag (if available)
    int flag_idx = 0;
    if (Size > 0) {
        flag_idx = Data[0] % num_flags;
    }
    
    // Create magic cookie
    magic_t cookie = magic_open(flags[flag_idx]);
    if (cookie == NULL) {
        remove_dummy_file();
        return 0;
    }
    
    // Test magic_load with default database
    int load_result = magic_load(cookie, NULL);
    
    // Test magic_buffer with the input data
    const char *buffer_result = NULL;
    if (Size > 0) {
        buffer_result = magic_buffer(cookie, Data, Size);
    }
    
    // Test magic_file with dummy file
    const char *file_result = magic_file(cookie, "./dummy_file");
    
    // Test magic_error (should be NULL if no error occurred)
    const char *error_msg = magic_error(cookie);
    
    // Clean up first cookie
    if (cookie) {
        magic_close(cookie);
    }
    
    // Test magic_check with a SEPARATE magic_set as required
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        // Use second byte to decide whether to check default database or dummy file
        const char *db_to_check = NULL;
        if (Size > 1 && (Data[1] % 2 == 0)) {
            db_to_check = "./dummy_file";
        }
        
        int check_result = magic_check(check_cookie, db_to_check);
        const char *check_error = magic_error(check_cookie);
        magic_close(check_cookie);
    }
    
    // Test another magic_open with different flag
    if (Size > 2) {
        int alt_flag_idx = Data[2] % num_flags;
        magic_t alt_cookie = magic_open(flags[alt_flag_idx]);
        if (alt_cookie) {
            // Try loading with dummy file as database
            magic_load(alt_cookie, "./dummy_file");
            
            // Test magic_buffer again with different size
            size_t buf_size = Size > 100 ? 100 : Size;
            if (buf_size > 0) {
                magic_buffer(alt_cookie, Data, buf_size);
            }
            
            magic_close(alt_cookie);
        }
    }
    
    // Test with NULL filename for magic_file (should read from stdin)
    magic_t null_cookie = magic_open(0);
    if (null_cookie) {
        magic_load(null_cookie, NULL);
        // Note: We can't easily test stdin reading in fuzzer
        magic_close(null_cookie);
    }
    
    remove_dummy_file();
    return 0;
}