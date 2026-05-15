// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getparam at magic.c:656:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    magic_t cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Use first byte to determine which operations to perform
    uint8_t selector = Size > 0 ? Data[0] : 0;
    
    // Test magic_setflags with various flag combinations
    int flags = (selector & 0x0F) | ((selector << 4) & 0xF0);
    magic_setflags(cookie, flags);
    
    // Test magic_getflags
    int current_flags = magic_getflags(cookie);
    
    // Test magic_setparam with different parameters
    size_t param_value = Size;
    int param_ids[] = {
        MAGIC_PARAM_INDIR_MAX,
        MAGIC_PARAM_NAME_MAX,
        MAGIC_PARAM_ELF_PHNUM_MAX,
        MAGIC_PARAM_ELF_SHNUM_MAX,
        MAGIC_PARAM_ELF_NOTES_MAX,
        MAGIC_PARAM_REGEX_MAX,
        MAGIC_PARAM_BYTES_MAX,
        MAGIC_PARAM_ENCODING_MAX,
        MAGIC_PARAM_ELF_SHSIZE_MAX
    };
    
    int param_idx = selector % (sizeof(param_ids)/sizeof(param_ids[0]));
    magic_setparam(cookie, param_ids[param_idx], &param_value);
    
    // Test magic_getparam
    size_t retrieved_value = 0;
    magic_getparam(cookie, param_ids[param_idx], &retrieved_value);
    
    // Test magic_load with different inputs
    if (Size > 1) {
        // Write fuzz data to dummy file and try to load it as magic database
        write_dummy_file(Data + 1, Size - 1);
        magic_load(cookie, "./dummy_file");
        remove_dummy_file();
        
        // Also try loading default database
        magic_load(cookie, NULL);
    }
    
    // Test magic_errno after operations
    int err = magic_errno(cookie);
    
    // Additional test: try setting flags again with different value
    if (Size > 2) {
        int new_flags = Data[1] | (Data[2] << 8);
        magic_setflags(cookie, new_flags);
    }
    
    // Test another parameter setting - SAFELY extract size_t value
    if (Size >= 3 + sizeof(size_t)) {
        size_t another_value = 0;
        memcpy(&another_value, Data + 3, sizeof(size_t));
        int another_param_idx = (selector + 1) % (sizeof(param_ids)/sizeof(param_ids[0]));
        magic_setparam(cookie, param_ids[another_param_idx], &another_value);
        
        size_t another_retrieved = 0;
        magic_getparam(cookie, param_ids[another_param_idx], &another_retrieved);
    } else if (Size > 3) {
        // If we have some data but not enough for full size_t, use what we have
        size_t another_value = 0;
        memcpy(&another_value, Data + 3, Size - 3);
        int another_param_idx = (selector + 1) % (sizeof(param_ids)/sizeof(param_ids[0]));
        magic_setparam(cookie, param_ids[another_param_idx], &another_value);
        
        size_t another_retrieved = 0;
        magic_getparam(cookie, param_ids[another_param_idx], &another_retrieved);
    }
    
    // Test error handling by passing invalid parameters
    if (Size > 4) {
        // Try setting invalid flags
        magic_setflags(cookie, -1);
        
        // Try setting invalid parameter
        int invalid_param = -1;
        size_t invalid_value = 0;
        magic_setparam(cookie, invalid_param, &invalid_value);
        
        // Try getting invalid parameter
        magic_getparam(cookie, invalid_param, &invalid_value);
    }
    
    magic_close(cookie);
    return 0;
}