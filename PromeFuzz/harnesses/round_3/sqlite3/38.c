// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    // This function will be called by SQLite and will exercise the target APIs
    if (argc < 1) return;
    
    sqlite3_value *val = argv[0];
    
    // Call sqlite3_value_bytes first
    int bytes = sqlite3_value_bytes(val);
    
    // Then call sqlite3_value_blob
    const void *blob = sqlite3_value_blob(val);
    
    // Finally call sqlite3_result_int with a value derived from the input
    int result = 0;
    if (blob != NULL && bytes > 0) {
        // Use first byte of blob to determine integer result
        result = *((const unsigned char*)blob);
    }
    sqlite3_result_int(ctx, result);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register our dummy function
        sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, 
                               dummy_function, NULL, NULL);
        initialized = 1;
    }
    
    if (db == NULL) return 0;
    
    // Create a statement that calls our function with a bound value
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT dummy_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    // Bind the fuzzed data as a blob
    if (sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1024 ? 1024 : Size), 
                         SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return 0;
    }
    
    // Execute the statement - this will trigger our dummy_function
    sqlite3_step(stmt);
    
    // Clean up
    sqlite3_finalize(stmt);
    
    return 0;
}