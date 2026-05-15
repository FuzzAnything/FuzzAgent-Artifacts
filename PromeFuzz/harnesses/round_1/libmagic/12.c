// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_descriptor at magic.c:403:1 in magic.h
// magic_buffer at magic.c:551:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <string.h>

#include "magic.h"

static int write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return -1;
    }
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return (written == size) ? 0 : -1;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create dummy file with fuzzed data
    if (write_dummy_file(Data, Size) != 0) {
        return 0;
    }

    // Initialize magic handle
    magic_t ms = magic_open(0);
    if (ms == NULL) {
        remove("./dummy_file");
        return 0;
    }

    // Test magic_load with default database
    int load_result = magic_load(ms, NULL);
    
    // Test magic_file on dummy file
    const char *file_result = magic_file(ms, "./dummy_file");
    
    // Test magic_descriptor on dummy file
    int fd = open("./dummy_file", O_RDONLY);
    if (fd >= 0) {
        const char *desc_result = magic_descriptor(ms, fd);
        close(fd);
    }
    
    // Test magic_buffer with fuzzed data directly
    const char *buffer_result = magic_buffer(ms, Data, Size);
    
    // Test error handling functions
    int err_code = magic_errno(ms);
    const char *err_str = magic_error(ms);
    
    // Clean up
    magic_close(ms);
    remove("./dummy_file");
    
    return 0;
}