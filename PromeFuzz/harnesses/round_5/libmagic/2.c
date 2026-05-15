// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
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
#include <unistd.h>
#include "magic.h"

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Write fuzz data to dummy file
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) {
        return 0;
    }
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    // Step 1: magic_open
    // Use first byte to determine flags for more diverse exploration
    int flags = 0;
    if (Size > 0) {
        // Common libmagic flags: MAGIC_NONE, MAGIC_DEBUG, MAGIC_SYMLINK, etc.
        // Using bits from input to set different flag combinations
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
    
    magic_t cookie = magic_open(flags);
    if (!cookie) {
        unlink("./dummy_file");
        return 0;
    }
    
    // Step 2: magic_load with NULL for default database
    int load_result = magic_load(cookie, NULL);
    
    // Step 3: magic_error after load
    const char *error_msg = magic_error(cookie);
    // error_msg may be NULL if no error
    
    // Step 4: magic_file with our dummy file
    const char *file_type = magic_file(cookie, "./dummy_file");
    // file_type may be NULL on error
    
    // Step 5: magic_error after file identification
    error_msg = magic_error(cookie);
    // error_msg may be NULL if no error
    
    // Step 6: magic_close
    magic_close(cookie);
    
    // Clean up dummy file
    unlink("./dummy_file");
    
    return 0;
}