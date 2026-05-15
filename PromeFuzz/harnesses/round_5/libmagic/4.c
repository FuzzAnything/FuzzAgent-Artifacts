// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
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
    if (Size < 1) return 0;
    
    magic_t cookie = magic_open(0);
    if (!cookie) return 0;
    
    // Use first byte to determine which operations to perform
    uint8_t selector = Data[0];
    size_t offset = 1;
    
    // Test magic_setflags with various flag combinations
    if (Size > offset + sizeof(int)) {
        int flags = *(int*)(Data + offset);
        magic_setflags(cookie, flags);
        offset += sizeof(int);
    }
    
    // Test magic_load with different database paths
    if (selector & 0x01) {
        // Try loading default database
        magic_load(cookie, NULL);
        
        // Try loading dummy file as database
        write_dummy_file(Data, Size);
        magic_load(cookie, "./dummy_file");
        cleanup_dummy_file();
    }
    
    // Test magic_setparam and magic_getparam with various parameters
    if (Size > offset + sizeof(int) + sizeof(size_t)) {
        int param_id = *(int*)(Data + offset);
        offset += sizeof(int);
        
        size_t param_value = *(size_t*)(Data + offset);
        offset += sizeof(size_t);
        
        // Set parameter
        magic_setparam(cookie, param_id, &param_value);
        
        // Get parameter back
        size_t retrieved_value;
        magic_getparam(cookie, param_id, &retrieved_value);
        
        // Try with different parameter IDs based on fuzzer data
        int alt_param_id = param_id % 10;  // Limit to reasonable range
        magic_setparam(cookie, alt_param_id, &param_value);
        magic_getparam(cookie, alt_param_id, &retrieved_value);
    }
    
    // Test magic_error after operations
    const char *error_msg = magic_error(cookie);
    (void)error_msg;  // Use variable to avoid unused warning
    
    // Test magic_getflags
    int current_flags = magic_getflags(cookie);
    (void)current_flags;  // Use variable to avoid unused warning
    
    // Additional test: Try setting flags again with different values
    if (Size > offset + sizeof(int)) {
        int new_flags = *(int*)(Data + offset);
        magic_setflags(cookie, new_flags);
    }
    
    // Test with invalid/null handle for some functions
    if (selector & 0x02) {
        // Test magic_getflags with NULL (should return -1)
        magic_getflags(NULL);
        
        // Test magic_error with NULL
        magic_error(NULL);
    }
    
    // Create another magic handle for additional testing
    magic_t cookie2 = magic_open(MAGIC_CONTINUE | MAGIC_ERROR);
    if (cookie2) {
        // Test interleaved operations between two handles
        if (Size > sizeof(int)) {
            int flags2 = *(int*)Data;
            magic_setflags(cookie2, flags2);
            magic_load(cookie2, NULL);
        }
        
        // Test parameter operations on second handle
        size_t test_value = Size;
        magic_setparam(cookie2, MAGIC_PARAM_BYTES_MAX, &test_value);
        
        size_t retrieved;
        magic_getparam(cookie2, MAGIC_PARAM_BYTES_MAX, &retrieved);
        
        magic_close(cookie2);
    }
    
    // Clean up
    magic_close(cookie);
    
    return 0;
}