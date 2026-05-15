// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_load at magic.c:317:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
// magic_getparam at magic.c:656:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
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

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    magic_t cookie = magic_open(0);
    if (!cookie) {
        return 0;
    }

    // Use first byte to determine which functions to call
    uint8_t selector = Size > 0 ? Data[0] : 0;
    size_t data_offset = Size > 0 ? 1 : 0;
    size_t remaining_size = Size - data_offset;

    // Test magic_getflags
    int flags = magic_getflags(cookie);
    (void)flags; // Suppress unused warning

    // Test magic_setflags with various flag combinations
    int test_flags = selector;
    magic_setflags(cookie, test_flags);

    // Test magic_setparam and magic_getparam with different parameters
    size_t param_value = remaining_size;
    if (remaining_size >= sizeof(size_t)) {
        memcpy(&param_value, Data + data_offset, sizeof(size_t));
        data_offset += sizeof(size_t);
        remaining_size -= sizeof(size_t);
    }

    // Try different parameter IDs based on selector
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
    
    int param_idx = selector % (sizeof(param_ids) / sizeof(param_ids[0]));
    magic_setparam(cookie, param_ids[param_idx], &param_value);
    
    size_t retrieved_value = 0;
    magic_getparam(cookie, param_ids[param_idx], &retrieved_value);

    // Write remaining data to dummy file for magic_file testing
    if (remaining_size > 0) {
        write_dummy_file(Data + data_offset, remaining_size);
        
        // Test magic_load with NULL (default database)
        magic_load(cookie, NULL);
        
        // Test magic_file with the dummy file
        const char *result = magic_file(cookie, "./dummy_file");
        (void)result; // Suppress unused warning
        
        // Clean up dummy file
        remove_dummy_file();
    }

    // Test magic_file with NULL filename (should read from stdin)
    if (selector & 0x80) {
        const char *stdin_result = magic_file(cookie, NULL);
        (void)stdin_result;
    }

    // Test with different flag combinations
    int more_flags[] = {MAGIC_NONE, MAGIC_DEBUG, MAGIC_SYMLINK, MAGIC_COMPRESS, 
                       MAGIC_DEVICES, MAGIC_MIME_TYPE, MAGIC_CONTINUE, MAGIC_CHECK};
    for (int i = 0; i < sizeof(more_flags) / sizeof(more_flags[0]); i++) {
        if (selector & (1 << (i % 8))) {
            magic_setflags(cookie, more_flags[i]);
            magic_getflags(cookie);
        }
    }

    // Test magic_load with different paths (though most will fail)
    const char *db_paths[] = {NULL, "./dummy_file", "/etc/magic", "/usr/share/misc/magic"};
    for (int i = 0; i < sizeof(db_paths) / sizeof(db_paths[0]); i++) {
        if (selector & (1 << (i % 8))) {
            magic_load(cookie, db_paths[i]);
        }
    }

    magic_close(cookie);
    return 0;
}