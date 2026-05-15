// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
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
    // This function will be called with the fuzzer-provided data
    if (argc > 0) {
        // First: sqlite3_value_bytes
        int bytes = sqlite3_value_bytes(argv[0]);
        
        // Second: sqlite3_value_blob
        const void *blob = sqlite3_value_blob(argv[0]);
        
        // Third: sqlite3_result_int
        sqlite3_result_int(ctx, bytes);
    }
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register our test function
        sqlite3_create_function(db, "dummy_func", 1, SQLITE_UTF8, NULL, 
                                dummy_function, NULL, NULL);
        
        // Prepare a statement that calls our function
        const char *sql = "SELECT dummy_func(?)";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        initialized = 1;
    }
    
    if (db == NULL || stmt == NULL) {
        return 0;
    }
    
    // Reset the statement for reuse
    sqlite3_reset(stmt);
    
    // Bind the fuzzer data as a BLOB parameter
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
    } else {
        // Bind NULL for empty input
        sqlite3_bind_null(stmt, 1);
    }
    
    // Execute the statement - this will call our dummy_function
    // which in turn calls the target API functions
    sqlite3_step(stmt);
    
    // Clean up bindings
    sqlite3_clear_bindings(stmt);
    
    return 0;
}