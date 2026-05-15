// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
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

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    write_dummy_file(Data, Size);
    
    // Fuzz magic_open
    int flags = Data[0] & 0xFF;
    magic_t cookie = magic_open(flags);
    if (cookie == NULL) {
        cleanup_dummy_file();
        return 0;
    }
    
    // Fuzz magic_load
    int load_result;
    if (Size > 1 && (Data[1] & 1)) {
        load_result = magic_load(cookie, "./dummy_file");
    } else {
        load_result = magic_load(cookie, NULL);
    }
    
    // Fuzz magic_error
    if (load_result == -1) {
        const char *error_msg = magic_error(cookie);
        (void)error_msg; // Use variable to avoid unused warning
    }
    
    // Fuzz magic_load_buffers
    if (Size > 2) {
        size_t num_buffers = (Data[2] % 4) + 1;
        void **buffers = malloc(num_buffers * sizeof(void *));
        size_t *sizes = malloc(num_buffers * sizeof(size_t));
        
        if (buffers && sizes) {
            size_t offset = 0;
            for (size_t i = 0; i < num_buffers && offset < Size; i++) {
                size_t chunk_size = (Size - offset) / (num_buffers - i);
                if (chunk_size == 0) chunk_size = 1;
                if (chunk_size > Size - offset) chunk_size = Size - offset;
                
                buffers[i] = (void *)(Data + offset);
                sizes[i] = chunk_size;
                offset += chunk_size;
            }
            
            magic_load_buffers(cookie, buffers, sizes, num_buffers);
            
            free(buffers);
            free(sizes);
        }
    }
    
    magic_close(cookie);
    
    // Fuzz magic_list with separate magic_set
    if (Size > 3) {
        int list_flags = Data[3] & 0xFF;
        magic_t list_cookie = magic_open(list_flags);
        if (list_cookie != NULL) {
            magic_list(list_cookie, "./dummy_file");
            magic_close(list_cookie);
        }
    }
    
    // Fuzz magic_check with separate magic_set
    if (Size > 4) {
        int check_flags = Data[4] & 0xFF;
        magic_t check_cookie = magic_open(check_flags);
        if (check_cookie != NULL) {
            magic_check(check_cookie, "./dummy_file");
            magic_close(check_cookie);
        }
    }
    
    cleanup_dummy_file();
    return 0;
}