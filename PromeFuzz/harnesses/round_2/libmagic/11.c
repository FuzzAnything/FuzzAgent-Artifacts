// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
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

static void create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create dummy file for functions that need it
    create_dummy_file(Data, Size);
    
    // Test magic_open with different flags
    int flags = Data[0] % 16;
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        remove_dummy_file();
        return 0;
    }
    
    // Test magic_load_buffers
    if (Size > 8) {
        size_t buffer_count = 2;
        void *buffers[2];
        size_t buffer_sizes[2];
        
        // Split input into two buffers
        size_t split = Size / 2;
        buffers[0] = (void *)Data;
        buffer_sizes[0] = split;
        buffers[1] = (void *)(Data + split);
        buffer_sizes[1] = Size - split;
        
        magic_load_buffers(cookie, buffers, buffer_sizes, buffer_count);
    }
    
    // Test magic_buffer
    if (Size > 0) {
        const char *result = magic_buffer(cookie, Data, Size);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test magic_compile
    magic_compile(cookie, "./dummy_file");
    
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
    
    // Cleanup
    magic_close(cookie);
    remove_dummy_file();
    
    return 0;
}