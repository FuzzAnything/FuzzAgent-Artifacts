// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_operations(sqlite3_str *str, const uint8_t *data, size_t size) {
    if (!str || size == 0) return;
    
    size_t pos = 0;
    
    while (pos < size) {
        uint8_t op = data[pos++];
        
        switch (op % 6) {
            case 0: { /* sqlite3_str_appendf */
                if (pos + 1 >= size) return;
                uint8_t fmt_len = data[pos++] % 32;
                if (fmt_len == 0) fmt_len = 1;
                
                if (pos + fmt_len > size) return;
                
                char format[33];
                size_t copy_len = (fmt_len < 32) ? fmt_len : 31;
                memcpy(format, &data[pos], copy_len);
                format[copy_len] = '\0';
                pos += fmt_len;
                
                /* Use a simple format string with some data */
                if (pos < size) {
                    uint8_t val = data[pos++];
                    sqlite3_str_appendf(str, format, val);
                }
                break;
            }
            
            case 1: { /* sqlite3_str_append */
                if (pos + 1 >= size) return;
                uint8_t append_len = data[pos++] % 64;
                if (append_len == 0) append_len = 1;
                
                if (pos + append_len > size) return;
                
                sqlite3_str_append(str, (const char*)&data[pos], append_len);
                pos += append_len;
                break;
            }
            
            case 2: { /* sqlite3_str_errcode */
                (void)sqlite3_str_errcode(str);
                break;
            }
            
            case 3: { /* sqlite3_str_length */
                (void)sqlite3_str_length(str);
                break;
            }
            
            case 4: { /* sqlite3_str_truncate */
                if (pos >= size) return;
                int trunc_len = data[pos++] % 256;
                sqlite3_str_truncate(str, trunc_len);
                break;
            }
            
            case 5: { /* sqlite3_str_appendchar */
                if (pos + 1 >= size) return;
                uint8_t count = data[pos++] % 64;
                char ch = data[pos++] % 128;
                sqlite3_str_appendchar(str, count, ch);
                break;
            }
        }
        
        /* Check error state periodically */
        if ((op % 8) == 0) {
            int err = sqlite3_str_errcode(str);
            if (err != SQLITE_OK) {
                /* Reset on error to continue fuzzing */
                sqlite3_str_truncate(str, 0);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_62(const uint8_t *data, size_t size) {
    /* Create a dynamic string object */
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) return 0;
    
    /* Perform fuzzing operations */
    fuzz_operations(str, data, size);
    
    /* Clean up */
    char *result = sqlite3_str_finish(str);
    if (result) {
        free(result);
    }
    
    return 0;
}