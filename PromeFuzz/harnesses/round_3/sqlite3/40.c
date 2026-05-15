// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_compileoption_used at sqlite3.c:176021:16 in sqlite3.h
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
    // Step 1: Prepare environment for sqlite3_compileoption_used
    // Create a null-terminated string from fuzzer input for compile option name
    char *opt_name = NULL;
    if (Size > 0) {
        opt_name = (char *)malloc(Size + 1);
        if (!opt_name) return 0;
        memcpy(opt_name, Data, Size);
        opt_name[Size] = '\0';
        
        // Call sqlite3_compileoption_used with fuzzer-provided option name
        int result = sqlite3_compileoption_used(opt_name);
        (void)result; // Use result to avoid unused variable warning
    } else {
        // Test with NULL input
        sqlite3_compileoption_used(NULL);
    }
    
    // Step 2: Prepare for sqlite3_open_v2
    sqlite3 *db = NULL;
    const char *filename = "./dummy_file";
    int flags = 0;
    const char *zVfs = NULL;
    
    // Use fuzzer data to determine flags if we have enough bytes
    if (Size >= 4) {
        // Use first 4 bytes as flags (little-endian)
        flags = *(int *)Data;
    }
    
    // Try different flag combinations to explore more states
    int flag_variants[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_MEMORY,
        flags // Use fuzzer-provided flags
    };
    
    for (int i = 0; i < sizeof(flag_variants)/sizeof(flag_variants[0]); i++) {
        int open_result = sqlite3_open_v2(filename, &db, flag_variants[i], zVfs);
        (void)open_result; // Use result to avoid unused variable warning
        
        // Step 3: Call sqlite3_close
        if (db) {
            int close_result = sqlite3_close(db);
            (void)close_result; // Use result to avoid unused variable warning
            db = NULL;
        }
    }
    
    // Cleanup
    if (opt_name) {
        free(opt_name);
    }
    
    return 0;
}