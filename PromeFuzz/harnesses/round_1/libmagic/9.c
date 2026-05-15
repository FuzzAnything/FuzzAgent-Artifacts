// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_buffer at magic.c:551:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

static void cleanup_file(const char *filename) {
    if (filename) {
        unlink(filename);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize magic handle
    magic_t ms = magic_open(MAGIC_NONE);
    if (ms == NULL) {
        return 0;
    }

    // Test magic_load with fuzzer-provided database file
    const char *magic_db = "./dummy_magic";
    FILE *fp = fopen(magic_db, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        // Try loading the fuzzer-generated magic database
        magic_load(ms, magic_db);
        
        // Check for errors
        int err = magic_errno(ms);
        const char *error_str = magic_error(ms);
        (void)err;
        (void)error_str;
        
        cleanup_file(magic_db);
    }

    // Reinitialize with default database for other tests
    magic_close(ms);
    ms = magic_open(MAGIC_NONE);
    if (ms == NULL) {
        return 0;
    }
    
    // Load default database
    if (magic_load(ms, NULL) == -1) {
        magic_close(ms);
        return 0;
    }

    // Test magic_buffer with fuzzer data
    const char *buffer_result = magic_buffer(ms, Data, Size);
    (void)buffer_result;

    // Test magic_file with fuzzer data written to file
    const char *filename = "./dummy_file";
    fp = fopen(filename, "wb");
    if (fp) {
        // Write partial data to explore different states
        size_t write_size = Size > 1024 ? 1024 : Size;
        fwrite(Data, 1, write_size, fp);
        fclose(fp);
        
        const char *file_result = magic_file(ms, filename);
        (void)file_result;
        
        // Test magic_descriptor
        int fd = open(filename, O_RDONLY);
        if (fd >= 0) {
            const char *desc_result = magic_descriptor(ms, fd);
            (void)desc_result;
            close(fd);
        }
        
        cleanup_file(filename);
    }

    // Test magic_file with NULL filename (stdin simulation)
    const char *null_file_result = magic_file(ms, NULL);
    (void)null_file_result;

    // Test error functions
    int last_err = magic_errno(ms);
    const char *last_error = magic_error(ms);
    (void)last_err;
    (void)last_error;

    // Test with different flags for more state exploration
    magic_t ms2 = magic_open(MAGIC_MIME);
    if (ms2) {
        if (magic_load(ms2, NULL) != -1) {
            // Test magic_buffer with offset data
            size_t test_size = Size > 256 ? 256 : Size;
            const void *test_buffer = Data;
            if (Size > 256) {
                test_buffer = Data + 256;
            }
            const char *mime_result = magic_buffer(ms2, test_buffer, test_size);
            (void)mime_result;
        }
        magic_close(ms2);
    }

    // Clean up
    magic_close(ms);
    return 0;
}