// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_appendf at sqlite3.c:19484:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    // Create a sqlite3_str object for testing
    sqlite3_str *str = sqlite3_str_new(NULL);
    if (!str) {
        return 0;
    }

    // Use fuzzer input to determine operations
    // We'll use a simple state machine based on input bytes
    size_t pos = 0;
    
    while (pos < Size) {
        uint8_t op = Data[pos++] % 6;  // 6 operations to test
        
        switch (op) {
            case 0: {  // sqlite3_str_appendchar
                if (pos + 2 > Size) goto cleanup;
                int N = Data[pos++] % 256;  // Limit N to reasonable size
                char C = (char)Data[pos++];
                sqlite3_str_appendchar(str, N, C);
                break;
            }
            
            case 1: {  // sqlite3_str_length
                int len = sqlite3_str_length(str);
                (void)len;  // Use result to avoid optimization
                break;
            }
            
            case 2: {  // sqlite3_str_append
                if (pos + 1 > Size) goto cleanup;
                int N = Data[pos++] % 256;  // Limit N
                if (pos + N > Size) N = Size - pos;
                if (N > 0) {
                    const char *zIn = (const char*)(Data + pos);
                    sqlite3_str_append(str, zIn, N);
                    pos += N;
                }
                break;
            }
            
            case 3: {  // sqlite3_str_truncate
                if (pos + 1 > Size) goto cleanup;
                int N = Data[pos++] % 256;
                sqlite3_str_truncate(str, N);
                break;
            }
            
            case 4: {  // sqlite3_str_appendf
                // Use a simple format string with data from input
                if (pos + 10 > Size) goto cleanup;
                
                // Create format string with some data
                char format[64];
                int a = Data[pos++] % 100;
                int b = Data[pos++] % 100;
                const char *text = "test";
                
                snprintf(format, sizeof(format), "Value: %d, Text: %s, Hex: %x", a, text, b);
                sqlite3_str_appendf(str, "%s", format);
                break;
            }
            
            case 5: {  // sqlite3_str_errcode
                int err = sqlite3_str_errcode(str);
                (void)err;  // Use result to avoid optimization
                break;
            }
        }
    }

cleanup:
    // Get the final string value
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }
    
    return 0;
}