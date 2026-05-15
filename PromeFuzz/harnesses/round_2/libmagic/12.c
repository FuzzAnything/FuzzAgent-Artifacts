// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    int fd = open("./dummy_file", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd >= 0) {
        write(fd, data, size);
        close(fd);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    magic_t ms = magic_open(0);
    if (!ms) return 0;
    
    // Use first byte to select different code paths
    uint8_t selector = Data[0];
    
    // Try different flag combinations
    int flags[] = {MAGIC_NONE, MAGIC_DEBUG, MAGIC_SYMLINK, MAGIC_COMPRESS, 
                   MAGIC_DEVICES, MAGIC_MIME_TYPE, MAGIC_CONTINUE};
    int flag_idx = selector % (sizeof(flags)/sizeof(flags[0]));
    
    // Load magic database
    if (magic_load(ms, NULL) != 0) {
        magic_close(ms);
        return 0;
    }
    
    // Test magic_getflags
    int current_flags = magic_getflags(ms);
    (void)current_flags;
    
    // Test magic_setflags with various flag combinations
    magic_setflags(ms, flags[flag_idx]);
    
    // Test magic_file with dummy file
    write_dummy_file(Data, Size);
    const char *result = magic_file(ms, "./dummy_file");
    (void)result;
    
    // Test magic_error (check if any error occurred)
    const char *error = magic_error(ms);
    (void)error;
    
    // Test magic_getparam with different parameters
    size_t param_value;
    int params[] = {MAGIC_PARAM_BYTES_MAX, MAGIC_PARAM_INDIR_MAX, 
                    MAGIC_PARAM_NAME_MAX, MAGIC_PARAM_ELF_PHNUM_MAX};
    for (int i = 0; i < sizeof(params)/sizeof(params[0]); i++) {
        if (magic_getparam(ms, params[i], &param_value) == 0) {
            // Use param_value to avoid optimization
            volatile size_t tmp = param_value;
            (void)tmp;
        }
    }
    
    // Test magic_setparam with fuzzed values
    size_t set_value = Size;
    for (int i = 0; i < sizeof(params)/sizeof(params[0]); i++) {
        magic_setparam(ms, params[i], &set_value);
    }
    
    // Test with NULL filename (should read from stdin)
    if (Size > 10 && (selector & 0x1)) {
        const char *null_result = magic_file(ms, NULL);
        (void)null_result;
    }
    
    // Test with empty string filename
    if (Size > 20 && (selector & 0x2)) {
        const char *empty_result = magic_file(ms, "");
        (void)empty_result;
    }
    
    // Clean up
    remove_dummy_file();
    magic_close(ms);
    
    return 0;
}