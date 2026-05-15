// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
// sqlite3_str_value at sqlite3.c:19221:18 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_append at sqlite3.c:19113:17 in sqlite3.h
// sqlite3_str_appendchar at sqlite3.c:19085:17 in sqlite3.h
// sqlite3_str_truncate at sqlite3.c:19213:17 in sqlite3.h
// sqlite3_str_length at sqlite3.c:19208:16 in sqlite3.h
// sqlite3_str_errcode at sqlite3.c:19203:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_xProgress(void *arg) {
    (void)arg;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    // Create a dummy sqlite3 connection for lookaside memory
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a dynamic string builder
    sqlite3_str *str = sqlite3_str_new(db);
    if (!str) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzz data to determine operations
    if (Size > 0) {
        // Use first byte to determine operation type
        uint8_t op = Data[0] % 6;
        size_t data_offset = 1;
        
        switch (op) {
            case 0: // sqlite3_str_appendchar
                if (Size >= data_offset + 2) {
                    int N = Data[data_offset] % 256;
                    char C = (char)Data[data_offset + 1];
                    sqlite3_str_appendchar(str, N, C);
                }
                break;
                
            case 1: // sqlite3_str_truncate
                if (Size >= data_offset + 1) {
                    int N = Data[data_offset] % 256;
                    sqlite3_str_truncate(str, N);
                }
                break;
                
            case 2: // sqlite3_str_length
                sqlite3_str_length(str);
                break;
                
            case 3: // sqlite3_str_append
                if (Size >= data_offset + 1) {
                    int N = Data[data_offset] % 256;
                    size_t remaining = Size - data_offset - 1;
                    if (remaining > 0) {
                        const char *zIn = (const char *)(Data + data_offset + 1);
                        int append_len = (N < (int)remaining) ? N : (int)remaining;
                        sqlite3_str_append(str, zIn, append_len);
                    }
                }
                break;
                
            case 4: // sqlite3_str_errcode
                sqlite3_str_errcode(str);
                break;
                
            case 5: // sqlite3_str_value
                sqlite3_str_value(str);
                break;
        }
    }
    
    // Perform some additional operations to explore more states
    if (Size > 1) {
        // Use second byte for another operation
        uint8_t op2 = Data[1] % 4;
        
        switch (op2) {
            case 0:
                sqlite3_str_appendchar(str, 5, 'X');
                break;
            case 1:
                sqlite3_str_truncate(str, sqlite3_str_length(str) / 2);
                break;
            case 2:
                if (Size > 10) {
                    sqlite3_str_append(str, (const char *)Data + 2, 
                                     (Size - 2 < 100) ? (int)(Size - 2) : 100);
                }
                break;
            case 3:
                // Chain multiple operations
                sqlite3_str_appendchar(str, 3, 'A');
                sqlite3_str_truncate(str, sqlite3_str_length(str));
                sqlite3_str_errcode(str);
                break;
        }
    }
    
    // Check error state and length
    int err = sqlite3_str_errcode(str);
    int len = sqlite3_str_length(str);
    (void)err;
    (void)len;
    
    // Get the string value
    char *value = sqlite3_str_value(str);
    if (value) {
        // Do something with the value to ensure it's used
        volatile char dummy = value[0];
        (void)dummy;
    }
    
    // Clean up
    char *result = sqlite3_str_finish(str);
    if (result) {
        sqlite3_free(result);
    }
    
    sqlite3_close(db);
    
    return 0;
}