// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_text at sqlite3.c:78491:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
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

static void custom_function(sqlite3_context* context, int argc, sqlite3_value** argv) {
    // Call functions in the specified order
    const unsigned char* text_result = sqlite3_value_text(argv[0]);
    const void* blob_result = sqlite3_value_blob(argv[0]);
    int bytes_result = sqlite3_value_bytes(argv[0]);
    
    // Use the bytes result as the int64 value to return
    sqlite3_result_int64(context, (sqlite3_int64)bytes_result);
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3* db = NULL;
    static sqlite3_stmt* stmt = NULL;
    
    // Initialize database connection once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Create a custom scalar function
        sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, 
                               custom_function, NULL, NULL);
        
        // Prepare a statement that uses our function
        const char* sql = "SELECT fuzz_func(?)";
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        initialized = 1;
    }
    
    if (!db || !stmt) {
        return 0;
    }
    
    // Bind the fuzzer input as a blob parameter
    sqlite3_reset(stmt);
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
    
    // Execute the statement to trigger our custom function
    sqlite3_step(stmt);
    
    // Clean up for this iteration
    sqlite3_clear_bindings(stmt);
    
    return 0;
}