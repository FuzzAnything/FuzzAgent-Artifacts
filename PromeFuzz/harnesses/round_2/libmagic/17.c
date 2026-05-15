// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
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

static int write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return -1;
    
    ssize_t written = write(fd, data, size);
    close(fd);
    return (written == (ssize_t)size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    magic_t cookie = magic_open(0);
    if (cookie == NULL) return 0;
    
    uint8_t selector = Data[0] % 6;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0: {
            // Test magic_getparam
            if (fuzz_size >= sizeof(int)) {
                int param;
                memcpy(&param, fuzz_data, sizeof(int));
                size_t value;
                magic_getparam(cookie, param, &value);
            }
            break;
        }
        case 1: {
            // Test magic_load
            if (write_dummy_file(fuzz_data, fuzz_size) == 0) {
                magic_load(cookie, "./dummy_file");
                unlink("./dummy_file");
            }
            break;
        }
        case 2: {
            // Test magic_getflags
            magic_getflags(cookie);
            break;
        }
        case 3: {
            // Test magic_setparam
            if (fuzz_size >= sizeof(int) + sizeof(size_t)) {
                int param;
                memcpy(&param, fuzz_data, sizeof(int));
                size_t value;
                memcpy(&value, fuzz_data + sizeof(int), sizeof(size_t));
                magic_setparam(cookie, param, &value);
            }
            break;
        }
        case 4: {
            // Test magic_errno
            magic_errno(cookie);
            break;
        }
        case 5: {
            // Test magic_setflags
            if (fuzz_size >= sizeof(int)) {
                int flags;
                memcpy(&flags, fuzz_data, sizeof(int));
                magic_setflags(cookie, flags);
            }
            break;
        }
    }
    
    // Additional calls to explore more states
    if (Size > 10) {
        int flags = magic_getflags(cookie);
        if (flags != -1) {
            magic_setflags(cookie, flags ^ (Data[1] & 0xFF));
        }
        
        if (write_dummy_file(Data, Size % 1024) == 0) {
            magic_load(cookie, NULL);
            unlink("./dummy_file");
        }
        
        size_t param_value;
        magic_getparam(cookie, MAGIC_PARAM_BYTES_MAX, &param_value);
        
        magic_errno(cookie);
    }
    
    magic_close(cookie);
    return 0;
}