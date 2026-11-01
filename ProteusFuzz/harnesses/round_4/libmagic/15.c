// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <magic.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    if (size > 0 && data != NULL) {
        if (write(fd, data, size) != (ssize_t)size) {
            close(fd);
            return -1;
        }
    }
    
    close(fd);
    return 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int fd = -1;
    int ret;
    size_t param_val;
    const char *result;
    
    // Initialize magic handle
    cookie = magic_open(MAGIC_NONE);
    if (cookie == NULL) {
        return 0;
    }
    
    // Load magic database
    if (magic_load(cookie, NULL) != 0) {
        magic_close(cookie);
        return 0;
    }
    
    // Test 1: magic_buffer
    if (Size > 0) {
        result = magic_buffer(cookie, Data, Size);
        // result is const char*, no cleanup needed for the string itself
        (void)result;
    }
    
    // Test 2: magic_getparam
    // Try different valid parameters
    ret = magic_getparam(cookie, MAGIC_PARAM_INDIR_MAX, &param_val);
    (void)ret;
    ret = magic_getparam(cookie, MAGIC_PARAM_NAME_MAX, &param_val);
    (void)ret;
    ret = magic_getparam(cookie, MAGIC_PARAM_REGEX_MAX, &param_val);
    (void)ret;
    ret = magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &param_val);
    (void)ret;
    
    // Test 3: magic_getflags
    ret = magic_getflags(cookie);
    (void)ret;
    
    // Test 4: magic_setflags
    // Try setting different flags
    ret = magic_setflags(cookie, MAGIC_PRESERVE_ATIME);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_NO_CHECK_COMPRESS);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_NO_CHECK_TOKENS);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_NO_CHECK_SOFT);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_NO_CHECK_ENCODING);
    (void)ret;
    ret = magic_setflags(cookie, MAGIC_NO_CHECK_TEXT);
    (void)ret;
    
    // Test 5: magic_file
    // Create a dummy file and test
    if (write_dummy_file(Data, Size) == 0) {
        result = magic_file(cookie, "./dummy_file");
        (void)result;
    }
    
    // Test 6: magic_descriptor
    // Open the dummy file and test
    fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        result = magic_descriptor(cookie, fd);
        (void)result;
        close(fd);
    }
    
    // Cleanup
    magic_close(cookie);
    
    // Remove dummy file
    unlink("./dummy_file");
    
    return 0;
}