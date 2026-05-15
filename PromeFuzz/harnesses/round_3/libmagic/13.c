// This fuzz driver is generated for library libmagic, aiming to fuzz the following functions:
// magic_getparam at magic.c:656:1 in magic.h
// magic_close at magic.c:306:1 in magic.h
// magic_open at magic.c:267:1 in magic.h
// magic_getflags at magic.c:585:1 in magic.h
// magic_setflags at magic.c:594:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_file at magic.c:414:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_load at magic.c:317:1 in magic.h
// magic_setparam at magic.c:613:1 in magic.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "magic.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void cleanup_dummy_file(void) {
    unlink("./dummy_file");
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    magic_t ms = magic_open(0);
    if (ms == NULL) return 0;
    
    // Use first byte to select which function to test
    uint8_t selector = Data[0];
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;
    
    switch (selector % 6) {
        case 0: { // magic_getflags
            int flags = magic_getflags(ms);
            (void)flags; // Use result to avoid unused variable warning
            break;
        }
        case 1: { // magic_setflags
            int flags = 0;
            if (func_size >= sizeof(int)) {
                memcpy(&flags, func_data, sizeof(int));
            }
            magic_setflags(ms, flags);
            break;
        }
        case 2: { // magic_file
            // Load default database first
            magic_load(ms, NULL);
            
            // Write data to dummy file
            write_dummy_file(func_data, func_size);
            
            const char *result = magic_file(ms, "./dummy_file");
            (void)result; // Use result to avoid unused variable warning
            
            cleanup_dummy_file();
            break;
        }
        case 3: { // magic_load
            if (func_size > 0) {
                // Create a null-terminated string from fuzz data
                char *db_path = malloc(func_size + 1);
                if (db_path) {
                    memcpy(db_path, func_data, func_size);
                    db_path[func_size] = '\0';
                    magic_load(ms, db_path);
                    free(db_path);
                }
            } else {
                magic_load(ms, NULL);
            }
            break;
        }
        case 4: { // magic_setparam
            if (func_size >= sizeof(int) + sizeof(size_t)) {
                int param = 0;
                size_t value = 0;
                memcpy(&param, func_data, sizeof(int));
                memcpy(&value, func_data + sizeof(int), sizeof(size_t));
                
                // Try common parameter types
                int params[] = {
                    MAGIC_PARAM_BYTES_MAX,
                    MAGIC_PARAM_INDIR_MAX,
                    MAGIC_PARAM_NAME_MAX,
                    MAGIC_PARAM_ELF_PHNUM_MAX,
                    MAGIC_PARAM_ELF_SHNUM_MAX,
                    MAGIC_PARAM_ELF_NOTES_MAX,
                    MAGIC_PARAM_REGEX_MAX,
                    MAGIC_PARAM_MAGWARN_MAX
                };
                
                int param_idx = param % (sizeof(params)/sizeof(params[0]));
                magic_setparam(ms, params[param_idx], &value);
            }
            break;
        }
        case 5: { // magic_getparam
            if (func_size >= sizeof(int)) {
                int param = 0;
                size_t value = 0;
                memcpy(&param, func_data, sizeof(int));
                
                // Try common parameter types
                int params[] = {
                    MAGIC_PARAM_BYTES_MAX,
                    MAGIC_PARAM_INDIR_MAX,
                    MAGIC_PARAM_NAME_MAX,
                    MAGIC_PARAM_ELF_PHNUM_MAX,
                    MAGIC_PARAM_ELF_SHNUM_MAX,
                    MAGIC_PARAM_ELF_NOTES_MAX,
                    MAGIC_PARAM_REGEX_MAX,
                    MAGIC_PARAM_MAGWARN_MAX
                };
                
                int param_idx = param % (sizeof(params)/sizeof(params[0]));
                magic_getparam(ms, params[param_idx], &value);
            }
            break;
        }
    }
    
    magic_close(ms);
    return 0;
}