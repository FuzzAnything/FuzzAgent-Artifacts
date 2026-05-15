// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
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
    // Initialize magic cookie
    magic_t cookie = magic_open(0);
    if (!cookie) {
        return 0;
    }

    // Use first byte to determine which operations to perform
    uint8_t selector = Size > 0 ? Data[0] : 0;
    size_t data_offset = 1;
    
    // Try loading magic database (with NULL for default)
    if (selector & 0x01) {
        magic_load(cookie, NULL);
    } else if (Size > data_offset) {
        // Use next bytes as potential database path (truncated to reasonable length)
        size_t path_len = Size - data_offset;
        if (path_len > 256) path_len = 256;
        char *db_path = malloc(path_len + 1);
        if (db_path) {
            memcpy(db_path, Data + data_offset, path_len);
            db_path[path_len] = '\0';
            magic_load(cookie, db_path);
            free(db_path);
        }
    }
    
    // Get and set flags
    int flags = magic_getflags(cookie);
    if (flags != -1) {
        // Try setting flags with fuzzer data
        if (Size > data_offset + 4) {
            int new_flags;
            memcpy(&new_flags, Data + data_offset, 4);
            magic_setflags(cookie, new_flags);
        }
        
        // Try toggling some common flags
        magic_setflags(cookie, flags | MAGIC_CONTINUE);
        magic_setflags(cookie, flags | MAGIC_MIME);
        magic_setflags(cookie, flags | MAGIC_SYMLINK);
    }
    
    // Get and set parameters
    size_t param_val;
    int params_to_try[] = {
        MAGIC_PARAM_INDIR_MAX,
        MAGIC_PARAM_NAME_MAX,
        MAGIC_PARAM_ELF_PHNUM_MAX,
        MAGIC_PARAM_ELF_SHNUM_MAX,
        MAGIC_PARAM_ELF_NOTES_MAX,
        MAGIC_PARAM_REGEX_MAX,
        MAGIC_PARAM_BYTES_MAX,
        MAGIC_PARAM_ENCODING_MAX
    };
    
    for (size_t i = 0; i < sizeof(params_to_try)/sizeof(params_to_try[0]); i++) {
        if (magic_getparam(cookie, params_to_try[i], &param_val) == 0) {
            // Try setting parameter with fuzzer data
            if (Size > data_offset + sizeof(size_t)) {
                size_t new_val;
                memcpy(&new_val, Data + data_offset, sizeof(size_t));
                magic_setparam(cookie, params_to_try[i], &new_val);
                data_offset += sizeof(size_t);
            }
        }
    }
    
    // Create dummy file and use magic_file
    write_dummy_file(Data, Size);
    const char *result = magic_file(cookie, "./dummy_file");
    (void)result; // Result may be NULL on error, but we don't care for fuzzing
    
    // Try with NULL filename (should read from stdin)
    // We can't easily test stdin reading in fuzzer, so skip
    
    // Try loading again with different flags
    magic_setflags(cookie, MAGIC_MIME_TYPE | MAGIC_MIME_ENCODING);
    if (selector & 0x02) {
        magic_load(cookie, NULL);
    }
    
    // Test magic_file again with different configuration
    result = magic_file(cookie, "./dummy_file");
    (void)result;
    
    // Clean up
    remove_dummy_file();
    magic_close(cookie);
    
    return 0;
}