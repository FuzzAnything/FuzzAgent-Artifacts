// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function will be called to test the API functions
    if (argc > 0) {
        sqlite3_value *value = argv[0];
        
        // Call functions in the specified order
        const unsigned char *text_result = sqlite3_value_text(value);
        const void *blob_result = sqlite3_value_blob(value);
        int bytes_result = sqlite3_value_bytes(value);
        sqlite3_result_int64(context, (sqlite3_int64)bytes_result);
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
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
    
    if (db == NULL) {
        return 0;
    }
    
    // Prepare a statement that calls our function with a parameter
    sqlite3_stmt *stmt;
    const char *sql = "SELECT dummy_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        return 0;
    }
    
    // Bind the fuzzer input as a blob parameter
    if (sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return 0;
    }
    
    // Execute the statement - this will trigger our dummy_function
    sqlite3_step(stmt);
    
    // Clean up
    sqlite3_finalize(stmt);
    
    // Reset the database connection to clean up any allocated values
    sqlite3_reset(db);
    
    return 0;
}