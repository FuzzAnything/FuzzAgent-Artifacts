// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
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

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    magic_t ms = magic_open(0);
    if (ms == NULL) {
        return 0;
    }
    
    // Load default magic database
    if (magic_load(ms, NULL) != 0) {
        magic_close(ms);
        return 0;
    }
    
    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t data_offset = 1;
    
    // Test magic_getparam
    if (selector & 0x01) {
        int param = MAGIC_PARAM_BYTES_MAX;
        size_t value = 0;
        if (data_offset < Size) {
            param = Data[data_offset] % 10; // Use modulo to get valid param range
            data_offset++;
        }
        magic_getparam(ms, param, &value);
    }
    
    // Test magic_file
    if (selector & 0x02) {
        write_dummy_file(Data + data_offset, Size - data_offset);
        const char *result = magic_file(ms, "./dummy_file");
        (void)result; // Use result to avoid unused variable warning
        cleanup_dummy_file();
    }
    
    // Test magic_getflags
    if (selector & 0x04) {
        int flags = magic_getflags(ms);
        (void)flags;
    }
    
    // Test magic_error
    if (selector & 0x08) {
        const char *error = magic_error(ms);
        (void)error;
    }
    
    // Test magic_setparam
    if (selector & 0x10) {
        int param = MAGIC_PARAM_BYTES_MAX;
        size_t value = 1024;
        if (data_offset + 1 < Size) {
            param = Data[data_offset] % 10;
            value = Data[data_offset + 1] * 1024;
            data_offset += 2;
        }
        magic_setparam(ms, param, &value);
    }
    
    // Test magic_setflags
    if (selector & 0x20) {
        int flags = 0;
        if (data_offset < Size) {
            flags = Data[data_offset] & (MAGIC_CONTINUE | MAGIC_MIME | MAGIC_SYMLINK);
            data_offset++;
        }
        magic_setflags(ms, flags);
    }
    
    // Additional test: call magic_file with NULL to read from stdin
    if (selector & 0x40) {
        // We can't easily simulate stdin in fuzzing, but we can test the API
        const char *result = magic_file(ms, NULL);
        (void)result;
    }
    
    // Clean up
    magic_close(ms);
    
    return 0;
}