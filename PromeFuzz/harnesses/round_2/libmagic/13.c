// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Initialize magic handle
    magic_t ms = magic_open(0);
    if (ms == NULL) return 0;
    
    // Load default magic database
    if (magic_load(ms, NULL) != 0) {
        magic_close(ms);
        return 0;
    }
    
    // Use fuzzer data to create dummy file
    write_dummy_file(Data, Size);
    
    // Test magic_file
    const char *result = magic_file(ms, "./dummy_file");
    (void)result; // Result may be NULL on error
    
    // Test magic_getflags
    int flags = magic_getflags(ms);
    (void)flags;
    
    // Test magic_setflags - use first byte for flags value
    int setflags_result = magic_setflags(ms, (int)Data[0]);
    (void)setflags_result;
    
    // Test magic_errno
    int err = magic_errno(ms);
    (void)err;
    
    // Test magic_setparam and magic_getparam
    // Use different parameters based on input data
    size_t param_value;
    int param_id;
    
    // Cycle through different parameter types using input data
    for (int i = 0; i < 4 && i < (int)Size; i++) {
        switch (i % 4) {
            case 0: param_id = MAGIC_PARAM_BYTES_MAX; break;
            case 1: param_id = MAGIC_PARAM_INDIR_MAX; break;
            case 2: param_id = MAGIC_PARAM_NAME_MAX; break;
            case 3: param_id = MAGIC_PARAM_REGEX_MAX; break;
            default: param_id = MAGIC_PARAM_BYTES_MAX;
        }
        
        param_value = (size_t)Data[i];
        
        // Set parameter
        magic_setparam(ms, param_id, &param_value);
        
        // Get parameter
        size_t get_value;
        magic_getparam(ms, param_id, &get_value);
        (void)get_value;
    }
    
    // Clean up
    magic_close(ms);
    remove_dummy_file();
    
    return 0;
}