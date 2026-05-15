// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    /* Create test string object using the proper API */
    sqlite3_str* str = sqlite3_str_new(NULL);
    if (!str) return 0;
    
    /* Use first byte to determine operation type */
    uint8_t op = Data[0] % 6;
    size_t offset = 1;
    
    switch (op) {
        case 0: { /* sqlite3_str_appendf */
            if (Size - offset < 1) break;
            
            /* Create format string from data */
            size_t fmt_len = (Size - offset) > 100 ? 100 : (Size - offset);
            char* fmt = (char*)malloc(fmt_len + 1);
            if (!fmt) break;
            
            memcpy(fmt, Data + offset, fmt_len);
            fmt[fmt_len] = '\0';
            
            /* Append formatted text */
            sqlite3_str_appendf(str, fmt, "test", 42, 3.14);
            
            free(fmt);
            break;
        }
        
        case 1: { /* sqlite3_str_append */
            if (Size - offset < 1) break;
            
            size_t append_len = (Size - offset) > 200 ? 200 : (Size - offset);
            if (append_len > 0) {
                /* Ensure string has at least append_len non-zero bytes */
                char* append_data = (char*)malloc(append_len);
                if (!append_data) break;
                
                memcpy(append_data, Data + offset, append_len);
                /* Ensure no zero bytes in the middle for proper testing */
                for (size_t i = 0; i < append_len; i++) {
                    if (append_data[i] == 0) append_data[i] = 'A';
                }
                
                sqlite3_str_append(str, append_data, (int)append_len);
                free(append_data);
            }
            break;
        }
        
        case 2: { /* sqlite3_str_errcode - just check error state */
            int err = sqlite3_str_errcode(str);
            (void)err; /* Use result to avoid unused warning */
            break;
        }
        
        case 3: { /* sqlite3_str_length - get current length */
            int len = sqlite3_str_length(str);
            (void)len; /* Use result to avoid unused warning */
            break;
        }
        
        case 4: { /* sqlite3_str_truncate */
            if (Size - offset < 1) break;
            
            /* Use second byte as truncation length */
            int trunc_len = Data[offset] % 256;
            sqlite3_str_truncate(str, trunc_len);
            break;
        }
        
        case 5: { /* sqlite3_str_appendchar */
            if (Size - offset >= 2) {
                int count = Data[offset] % 256;
                char ch = (char)Data[offset + 1];
                sqlite3_str_appendchar(str, count, ch);
            }
            break;
        }
    }
    
    /* Check error state after operation */
    int err = sqlite3_str_errcode(str);
    (void)err;
    
    /* Get length after operation */
    int len = sqlite3_str_length(str);
    (void)len;
    
    /* Perform additional operations to explore more states */
    if (Size > 10) {
        /* Append some characters */
        sqlite3_str_appendchar(str, 5, 'X');
        
        /* Truncate to random length */
        int trunc_len = Data[Size - 1] % (len + 10);
        sqlite3_str_truncate(str, trunc_len);
        
        /* Append more data if available */
        if (Size > 20) {
            size_t extra_len = (Size - 10) > 50 ? 50 : (Size - 10);
            char* extra = (char*)malloc(extra_len);
            if (extra) {
                memcpy(extra, Data + 10, extra_len);
                for (size_t i = 0; i < extra_len; i++) {
                    if (extra[i] == 0) extra[i] = 'B';
                }
                sqlite3_str_append(str, extra, (int)extra_len);
                free(extra);
            }
        }
    }
    
    /* Clean up using the proper API */
    char* result = sqlite3_str_finish(str);
    if (result) {
        free(result);
    }
    
    return 0;
}