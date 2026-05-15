// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    // Step 1: Call sqlite3_compileoption_used
    // We need to create a null-terminated string from the input data
    char *opt_name = NULL;
    if (Size > 0) {
        opt_name = (char *)malloc(Size + 1);
        if (!opt_name) return 0;
        memcpy(opt_name, Data, Size);
        opt_name[Size] = '\0';
    } else {
        // If input is empty, use a dummy string
        opt_name = strdup("");
        if (!opt_name) return 0;
    }
    
    int compile_option_result = sqlite3_compileoption_used(opt_name);
    (void)compile_option_result; // Use result to avoid unused variable warning
    
    free(opt_name);
    
    // Step 2: Call sqlite3_open_v2
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    
    // Use the first 4 bytes of input for flags, or default if not enough data
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size >= 4) {
        // Use the first 4 bytes as flags (little-endian)
        flags = (Data[0]) | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }
    
    // Open database with the specified flags
    int open_result = sqlite3_open_v2(filename, &db, flags, NULL);
    (void)open_result; // Use result to avoid unused variable warning
    
    // Step 3: Call sqlite3_close
    if (db) {
        int close_result = sqlite3_close(db);
        (void)close_result; // Use result to avoid unused variable warning
    }
    
    return 0;
}