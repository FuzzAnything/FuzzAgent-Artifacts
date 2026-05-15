// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_check at magic.c:348:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_compile at magic.c:340:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load_buffers at magic.c:329:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_list at magic.c:356:1 in magic.h
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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for functions that need file input
    write_dummy_file(Data, Size);
    
    // 1. Test magic_buffer
    magic_t cookie = magic_open(MAGIC_NONE);
    if (cookie) {
        // Try to load default database first
        if (magic_load(cookie, NULL) == 0) {
            const char *result = magic_buffer(cookie, Data, Size);
            (void)result; // Use result to avoid unused variable warning
        }
        magic_close(cookie);
    }
    
    // 2. Test magic_check with separate magic_set
    magic_t cookie2 = magic_open(MAGIC_NONE);
    if (cookie2) {
        int check_result = magic_check(cookie2, "./dummy_file");
        (void)check_result;
        magic_close(cookie2);
    }
    
    // 3. Test magic_load_buffers
    magic_t cookie3 = magic_open(MAGIC_NONE);
    if (cookie3) {
        // Create buffer array from fuzzer input
        void *buffers[1] = {(void *)Data};
        size_t sizes[1] = {Size};
        int load_result = magic_load_buffers(cookie3, buffers, sizes, 1);
        (void)load_result;
        
        // Test magic_error after load attempt
        const char *error_msg = magic_error(cookie3);
        (void)error_msg;
        
        magic_close(cookie3);
    }
    
    // 4. Test magic_compile
    magic_t cookie4 = magic_open(MAGIC_NONE);
    if (cookie4) {
        int compile_result = magic_compile(cookie4, "./dummy_file");
        (void)compile_result;
        magic_close(cookie4);
    }
    
    // 5. Test magic_error with another handle
    magic_t cookie5 = magic_open(MAGIC_NONE);
    if (cookie5) {
        // Try to load invalid data to trigger error
        void *invalid_buffers[1] = {(void *)Data};
        size_t invalid_sizes[1] = {Size};
        magic_load_buffers(cookie5, invalid_buffers, invalid_sizes, 1);
        
        const char *error = magic_error(cookie5);
        (void)error;
        magic_close(cookie5);
    }
    
    // 6. Test magic_list with separate magic_set
    magic_t cookie6 = magic_open(MAGIC_NONE);
    if (cookie6) {
        int list_result = magic_list(cookie6, "./dummy_file");
        (void)list_result;
        magic_close(cookie6);
    }
    
    // Clean up
    remove_dummy_file();
    
    return 0;
}