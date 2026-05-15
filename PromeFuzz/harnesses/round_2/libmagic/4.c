// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Write fuzzer input to dummy file for file-based operations
    write_dummy_file(Data, Size);
    
    // Test magic_open
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] & (MAGIC_NONE | MAGIC_DEBUG | MAGIC_SYMLINK | 
                          MAGIC_COMPRESS | MAGIC_DEVICES | MAGIC_MIME | 
                          MAGIC_CONTINUE | MAGIC_CHECK | MAGIC_PRESERVE_ATIME | 
                          MAGIC_RAW | MAGIC_ERROR | MAGIC_MIME_ENCODING | 
                          MAGIC_MIME_TYPE | MAGIC_APPLE | MAGIC_EXTENSION | 
                          MAGIC_COMPRESS_TRANSP | MAGIC_NODESC);
    }
    
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Test magic_load with dummy file
    if (Size > 1) {
        magic_load(cookie, "./dummy_file");
        // Check for errors
        const char *error = magic_error(cookie);
        (void)error; // Use error to avoid unused variable warning
    } else {
        // Test with default database
        magic_load(cookie, NULL);
    }
    
    // Test magic_load_buffers
    if (Size > 4) {
        // Split input into buffers for testing
        size_t num_buffers = 1;
        if (Size > 10) {
            num_buffers = 2;
        }
        
        void **buffers = malloc(num_buffers * sizeof(void*));
        size_t *buffer_sizes = malloc(num_buffers * sizeof(size_t));
        
        if (buffers && buffer_sizes) {
            // Use first half of data for first buffer
            size_t first_size = Size / 2;
            buffers[0] = malloc(first_size);
            if (buffers[0]) {
                memcpy(buffers[0], Data, first_size);
                buffer_sizes[0] = first_size;
            }
            
            if (num_buffers > 1) {
                // Use second half for second buffer
                size_t second_size = Size - first_size;
                buffers[1] = malloc(second_size);
                if (buffers[1]) {
                    memcpy(buffers[1], Data + first_size, second_size);
                    buffer_sizes[1] = second_size;
                }
            }
            
            magic_load_buffers(cookie, buffers, buffer_sizes, num_buffers);
            
            // Cleanup buffers
            for (size_t i = 0; i < num_buffers; i++) {
                free(buffers[i]);
            }
            free(buffers);
            free(buffer_sizes);
        }
    }
    
    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(flags);
    if (check_cookie) {
        magic_check(check_cookie, "./dummy_file");
        magic_close(check_cookie);
    }
    
    // Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(flags);
    if (list_cookie) {
        magic_list(list_cookie, "./dummy_file");
        magic_close(list_cookie);
    }
    
    // Test magic_error after various operations
    const char *last_error = magic_error(cookie);
    (void)last_error; // Use last_error to avoid unused variable warning
    
    // Cleanup
    magic_close(cookie);
    cleanup_dummy_file();
    
    return 0;
}