// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_errno at magic.c:577:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    magic_t cookie = magic_open(0);
    if (cookie == NULL) {
        return 0;
    }

    // Use first byte to determine which functions to call
    uint8_t selector = Data[0];
    size_t offset = 1;

    // magic_getflags
    if (selector & 0x01) {
        int flags = magic_getflags(cookie);
        (void)flags; // Use result to avoid unused variable warning
    }

    // magic_setflags - use next 4 bytes as flags if available
    if (selector & 0x02 && offset + 4 <= Size) {
        int flags;
        memcpy(&flags, Data + offset, sizeof(int));
        offset += sizeof(int);
        magic_setflags(cookie, flags);
    }

    // magic_load - either default or from dummy file
    if (selector & 0x04) {
        if (selector & 0x08) {
            // Load from dummy file
            write_dummy_file(Data + offset, Size - offset);
            magic_load(cookie, "./dummy_file");
            remove_dummy_file();
        } else {
            // Load default database
            magic_load(cookie, NULL);
        }
    }

    // magic_setparam and magic_getparam
    if (selector & 0x10) {
        // Try different parameter types based on available data
        size_t param_value;
        
        // Use available bytes for parameter value
        if (offset + sizeof(size_t) <= Size) {
            memcpy(&param_value, Data + offset, sizeof(size_t));
            offset += sizeof(size_t);
        } else if (offset < Size) {
            // Partial read if not enough bytes
            param_value = 0;
            memcpy(&param_value, Data + offset, Size - offset);
            offset = Size;
        } else {
            param_value = Size; // Fallback value
        }

        // Try setting different parameters
        int params[] = {
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
        
        int param_count = sizeof(params) / sizeof(params[0]);
        int param_idx = selector % param_count;
        
        // magic_setparam
        magic_setparam(cookie, params[param_idx], &param_value);
        
        // magic_getparam
        size_t retrieved_value;
        magic_getparam(cookie, params[param_idx], &retrieved_value);
    }

    // magic_errno - call after operations that might fail
    if (selector & 0x20) {
        int err = magic_errno(cookie);
        (void)err; // Use result to avoid unused variable warning
    }

    // Additional test: try setting and getting flags in sequence
    if (selector & 0x40 && offset + 4 <= Size) {
        int new_flags;
        memcpy(&new_flags, Data + offset, sizeof(int));
        offset += sizeof(int);
        
        int set_result = magic_setflags(cookie, new_flags);
        if (set_result == 0) {
            int retrieved_flags = magic_getflags(cookie);
            (void)retrieved_flags; // Use result
        }
    }

    // Test error handling by passing NULL to functions that should handle it
    if (selector & 0x80) {
        // magic_getflags with NULL
        magic_getflags(NULL);
        
        // magic_setflags with NULL
        magic_setflags(NULL, 0);
        
        // magic_errno with NULL
        magic_errno(NULL);
    }

    magic_close(cookie);
    return 0;
}