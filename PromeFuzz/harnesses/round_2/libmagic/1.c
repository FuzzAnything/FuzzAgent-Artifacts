// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_error at magic.c:569:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "magic.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    static const char *dummy_filename = "./dummy_file";
    
    // Write fuzzer input to dummy file
    FILE *fp = fopen(dummy_filename, "wb");
    if (!fp) {
        return 0;
    }
    if (Size > 0) {
        fwrite(Data, 1, Size, fp);
    }
    fclose(fp);
    
    // Create magic cookie with various flags to explore different states
    int flags = 0;
    if (Size > 0) {
        // Use first byte to determine some flags
        uint8_t flag_byte = Data[0];
        if (flag_byte & 0x01) flags |= MAGIC_NONE;
        if (flag_byte & 0x02) flags |= MAGIC_DEBUG;
        if (flag_byte & 0x04) flags |= MAGIC_SYMLINK;
        if (flag_byte & 0x08) flags |= MAGIC_COMPRESS;
        if (flag_byte & 0x10) flags |= MAGIC_DEVICES;
        if (flag_byte & 0x20) flags |= MAGIC_MIME_TYPE;
        if (flag_byte & 0x40) flags |= MAGIC_CONTINUE;
        if (flag_byte & 0x80) flags |= MAGIC_CHECK;
    }
    
    magic_t ms = magic_open(flags);
    if (ms == NULL) {
        unlink(dummy_filename);
        return 0;
    }
    
    // Load magic database - try both NULL and specific paths
    int load_result;
    if (Size > 1 && (Data[1] % 3 == 0)) {
        // Try loading with NULL (default database)
        load_result = magic_load(ms, NULL);
    } else if (Size > 1 && (Data[1] % 3 == 1)) {
        // Try with empty string
        load_result = magic_load(ms, "");
    } else {
        // Try with magic.mgc
        load_result = magic_load(ms, "magic.mgc");
    }
    
    // Check for error after load
    const char *error_msg = magic_error(ms);
    (void)error_msg; // Use variable to avoid unused warning
    
    // Identify file type
    const char *result = magic_file(ms, dummy_filename);
    (void)result; // Use variable to avoid unused warning
    
    // Check for error after file identification
    error_msg = magic_error(ms);
    (void)error_msg;
    
    // Clean up
    magic_close(ms);
    unlink(dummy_filename);
    
    return 0;
}