// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

static void test_magic_functions(magic_t cookie, const uint8_t *data, size_t size) {
    // Test magic_getflags
    int flags = magic_getflags(cookie);
    (void)flags; // Use result to avoid unused variable warning
    
    // Test magic_setflags with various flag combinations
    if (size >= sizeof(int)) {
        int new_flags = *(int*)data;
        magic_setflags(cookie, new_flags);
    }
    
    // Test magic_load with different database paths
    if (size > 0) {
        // Try with NULL (default database)
        magic_load(cookie, NULL);
        
        // Try with dummy file path if we have enough data
        if (size >= 4 && write_dummy_file(data, size % 1024) == 0) {
            magic_load(cookie, "./dummy_file");
            unlink("./dummy_file");
        }
    }
    
    // Test magic_setparam and magic_getparam with various parameters
    if (size >= sizeof(int) + sizeof(size_t)) {
        int param_id = *(int*)data;
        size_t param_value = *(size_t*)(data + sizeof(int));
        
        // Try setting parameter
        magic_setparam(cookie, param_id, &param_value);
        
        // Try getting parameter
        size_t retrieved_value;
        magic_getparam(cookie, param_id, &retrieved_value);
    }
    
    // Test magic_errno
    int err = magic_errno(cookie);
    (void)err; // Use result to avoid unused variable warning
}

int LLVMFuzzerTestOneInput_19(const uint8_t *data, size_t size) {
    // Create magic cookie with different flags
    magic_t cookie1 = magic_open(0);
    magic_t cookie2 = magic_open(MAGIC_CONTINUE | MAGIC_ERROR);
    magic_t cookie3 = magic_open(MAGIC_MIME | MAGIC_MIME_ENCODING);
    
    if (cookie1) {
        test_magic_functions(cookie1, data, size);
        magic_close(cookie1);
    }
    
    if (cookie2) {
        test_magic_functions(cookie2, data, size);
        magic_close(cookie2);
    }
    
    if (cookie3) {
        test_magic_functions(cookie3, data, size);
        magic_close(cookie3);
    }
    
    // Test with NULL cookie for error cases
    magic_getflags(NULL);
    if (size >= sizeof(int)) {
        magic_setflags(NULL, *(int*)data);
    }
    magic_load(NULL, NULL);
    
    if (size >= sizeof(int) + sizeof(size_t)) {
        size_t dummy_value = 0;
        magic_setparam(NULL, *(int*)data, &dummy_value);
        magic_getparam(NULL, *(int*)data, &dummy_value);
    }
    
    magic_errno(NULL);
    
    return 0;
}