// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
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
#include <stdio.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create main magic cookie with flags from input
    int flags = Data[0];
    magic_t cookie = magic_open(flags);
    if (!cookie) return 0;
    
    // Test magic_load with various inputs
    if (Size > 1) {
        // Sometimes use default database, sometimes use dummy file
        if (Data[1] % 2 == 0) {
            magic_load(cookie, NULL);  // Default database
        } else {
            write_dummy_file(Data + 2, Size > 2 ? Size - 2 : 0);
            magic_load(cookie, "./dummy_file");
            remove_dummy_file();
        }
        
        // Check for errors
        const char *error = magic_error(cookie);
        (void)error;  // Use error to avoid unused variable warning
    }
    
    // Test magic_load_buffers with fuzz data
    if (Size > 10) {
        // Create buffer array from fuzz data
        size_t buffer_count = Data[2] % 4 + 1;  // 1-4 buffers
        void **buffers = malloc(buffer_count * sizeof(void *));
        size_t *sizes = malloc(buffer_count * sizeof(size_t));
        
        if (buffers && sizes) {
            size_t offset = 3;
            for (size_t i = 0; i < buffer_count && offset < Size; i++) {
                // Use remaining data for buffers
                size_t buffer_size = (Size - offset) / (buffer_count - i);
                if (buffer_size > 0) {
                    buffers[i] = malloc(buffer_size);
                    if (buffers[i]) {
                        memcpy(buffers[i], Data + offset, buffer_size);
                        sizes[i] = buffer_size;
                        offset += buffer_size;
                    } else {
                        sizes[i] = 0;
                    }
                } else {
                    buffers[i] = NULL;
                    sizes[i] = 0;
                }
            }
            
            magic_load_buffers(cookie, buffers, sizes, buffer_count);
            
            // Cleanup
            for (size_t i = 0; i < buffer_count; i++) {
                free(buffers[i]);
            }
            free(buffers);
            free(sizes);
        }
    }
    
    // Test magic_check with separate magic_set
    magic_t check_cookie = magic_open(0);
    if (check_cookie) {
        if (Size > 20) {
            write_dummy_file(Data + 10, Size - 10);
            magic_check(check_cookie, "./dummy_file");
            remove_dummy_file();
        } else {
            magic_check(check_cookie, NULL);
        }
        magic_close(check_cookie);
    }
    
    // Test magic_list with separate magic_set
    magic_t list_cookie = magic_open(0);
    if (list_cookie) {
        if (Size > 30) {
            write_dummy_file(Data + 20, Size - 20);
            magic_list(list_cookie, "./dummy_file");
            remove_dummy_file();
        } else {
            magic_list(list_cookie, NULL);
        }
        magic_close(list_cookie);
    }
    
    // Cleanup main cookie
    magic_close(cookie);
    
    return 0;
}