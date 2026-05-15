// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Create dummy file with fuzzer data
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Use first byte to determine flags for magic_open
    int flags = Data[0] & 0xFF;
    const uint8_t *db_data = Data + 1;
    size_t db_size = Size - 1;

    // Test magic_open and magic_load
    magic_t ms = magic_open(flags);
    if (ms != NULL) {
        // Try loading with NULL (default database)
        magic_load(ms, NULL);
        
        // Try loading with dummy file as database
        if (db_size > 0) {
            // Write database data to a temporary file
            int db_fd = open("./dummy_db", O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (db_fd >= 0) {
                write(db_fd, db_data, db_size);
                close(db_fd);
                magic_load(ms, "./dummy_db");
                unlink("./dummy_db");
            }
        }

        // Test magic_file with dummy file
        const char *result = magic_file(ms, "./dummy_file");
        if (result == NULL) {
            // Check error if magic_file failed
            const char *error = magic_error(ms);
            (void)error; // Use error to avoid unused variable warning
        }

        // Test magic_error even if no error occurred
        const char *error_msg = magic_error(ms);
        (void)error_msg; // Use error_msg to avoid unused variable warning

        // Clean up first handle
        magic_close(ms);
    }

    // Test magic_check with a separate magic_set
    if (Size > 1) {
        magic_t check_ms = magic_open(flags);
        if (check_ms != NULL) {
            // Try checking with NULL (default database)
            magic_check(check_ms, NULL);
            
            // Try checking with dummy database file
            if (db_size > 0) {
                int db_fd = open("./dummy_db_check", O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (db_fd >= 0) {
                    write(db_fd, db_data, db_size);
                    close(db_fd);
                    magic_check(check_ms, "./dummy_db_check");
                    unlink("./dummy_db_check");
                }
            }
            
            // Check for errors after magic_check
            const char *check_error = magic_error(check_ms);
            (void)check_error; // Use check_error to avoid unused variable warning
            
            magic_close(check_ms);
        }
    }

    // Test magic_file with NULL filename (reads from stdin)
    magic_t stdin_ms = magic_open(flags);
    if (stdin_ms != NULL) {
        magic_load(stdin_ms, NULL);
        // Note: We cannot easily test stdin reading in this fuzzer context
        // as it would require setting up stdin with the fuzzer data.
        // We'll just create the handle and close it.
        magic_close(stdin_ms);
    }

    // Test magic_open with various flags combinations
    int extra_flags[] = {MAGIC_NONE, MAGIC_DEBUG, MAGIC_SYMLINK, MAGIC_COMPRESS, 
                         MAGIC_DEVICES, MAGIC_MIME, MAGIC_CONTINUE, MAGIC_CHECK, 
                         MAGIC_PRESERVE_ATIME, MAGIC_RAW, MAGIC_ERROR};
    
    for (int i = 0; i < (int)(sizeof(extra_flags)/sizeof(extra_flags[0])); i++) {
        magic_t test_ms = magic_open(extra_flags[i]);
        if (test_ms != NULL) {
            magic_load(test_ms, NULL);
            magic_file(test_ms, "./dummy_file");
            magic_close(test_ms);
        }
    }

    // Test magic_close with NULL (should be safe)
    magic_close(NULL);

    cleanup_dummy_file();
    return 0;
}