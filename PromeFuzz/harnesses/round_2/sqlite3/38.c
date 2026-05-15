// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:173698:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:173707:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *err_msg = NULL;
    char *formatted_str = NULL;
    int errcode, extended_errcode;
    
    // Try to open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    
    // Call sqlite3_errcode first
    errcode = sqlite3_errcode(db);
    
    // Call sqlite3_mprintf with fuzzer data
    if (Size > 0) {
        // Create a null-terminated copy of the input for formatting
        char *input_copy = malloc(Size + 1);
        if (input_copy) {
            memcpy(input_copy, Data, Size);
            input_copy[Size] = '\0';
            
            // Use the fuzzer input in the format string
            formatted_str = sqlite3_mprintf("Fuzz input: %s", input_copy);
            free(input_copy);
        } else {
            // If allocation fails, use a simple format string
            formatted_str = sqlite3_mprintf("Fuzz input size: %zu", Size);
        }
    } else {
        formatted_str = sqlite3_mprintf("Empty fuzz input");
    }
    
    // Call sqlite3_errmsg
    err_msg = sqlite3_errmsg(db);
    
    // Call sqlite3_extended_errcode
    extended_errcode = sqlite3_extended_errcode(db);
    
    // Clean up
    if (formatted_str) {
        sqlite3_free(formatted_str);
    }
    
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif