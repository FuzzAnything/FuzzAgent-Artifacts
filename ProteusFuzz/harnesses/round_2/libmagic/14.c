// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_descriptor at magic.c:403:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
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
#include <stdio.h>

static void cleanup_magic(magic_t cookie) {
    if (cookie != NULL) {
        magic_close(cookie);
    }
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    magic_t cookie = NULL;
    int ret;
    size_t param_val;
    int fd;
    const char *result;

    if (Size == 0) {
        return 0;
    }

    // Initialize magic cookie
    cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Load default magic files
    if (magic_load(cookie, NULL) != 0) {
        cleanup_magic(cookie);
        return 0;
    }

    // Fuzz magic_buffer
    result = magic_buffer(cookie, Data, Size);
    if (result != NULL) {
        // Just consume the result to avoid unused variable warnings
        (void)strlen(result);
    }

    // Fuzz magic_setflags with various flag combinations derived from input
    int flags = 0;
    if (Size > 0) flags = Data[0];
    if (Size > 1) flags |= (Data[1] << 8);
    if (Size > 2) flags |= (Data[2] << 16);
    if (Size > 3) flags |= (Data[3] << 24);
    
    ret = magic_setflags(cookie, flags);
    (void)ret;

    // Fuzz magic_getflags
    int current_flags = magic_getflags(cookie);
    (void)current_flags;

    // Fuzz magic_setparam with various parameters
    // MAGIC_PARAM_REGEX_MAX, MAGIC_PARAM_INDIR_MAX, etc.
    size_t test_size = 0;
    if (Size > 4) {
        test_size = *(const size_t *)(Data + 4);
    }
    // Try setting different parameters
    ret = magic_setparam(cookie, MAGIC_PARAM_REGEX_MAX, &test_size);
    (void)ret;
    
    ret = magic_setparam(cookie, MAGIC_PARAM_INDIR_MAX, &test_size);
    (void)ret;
    
    ret = magic_setparam(cookie, MAGIC_PARAM_BYTES_MAX, &test_size);
    (void)ret;

    // Fuzz magic_getparam
    ret = magic_getparam(cookie, MAGIC_PARAM_REGEX_MAX, &param_val);
    (void)ret;
    (void)param_val;

    ret = magic_getparam(cookie, MAGIC_PARAM_INDIR_MAX, &param_val);
    (void)ret;
    (void)param_val;

    // Fuzz magic_descriptor using a temporary file
    const char *dummy_file = "./dummy_file";
    fd = open(dummy_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        ssize_t written = write(fd, Data, Size);
        (void)written;
        close(fd);
        
        // Reopen for reading to test magic_descriptor
        fd = open(dummy_file, O_RDONLY);
        if (fd >= 0) {
            result = magic_descriptor(cookie, fd);
            if (result != NULL) {
                (void)strlen(result);
            }
            close(fd);
        }
        
        unlink(dummy_file);
    }

    // Cleanup
    cleanup_magic(cookie);
    
    return 0;
}