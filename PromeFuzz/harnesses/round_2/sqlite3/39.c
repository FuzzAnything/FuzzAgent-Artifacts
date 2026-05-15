// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static void dummy_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    // This function is called by SQLite and will invoke the target functions
    if (argc < 1) return;
    
    sqlite3_value *value = argv[0];
    
    // 1. Call sqlite3_value_bytes
    int bytes = sqlite3_value_bytes(value);
    
    // 2. Call sqlite3_value_blob
    const void *blob = sqlite3_value_blob(value);
    
    // 3. Call sqlite3_result_int with the byte count
    sqlite3_result_int(context, bytes);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    
    // Initialize database once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Register our test function
        sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, 
                               dummy_function, NULL, NULL);
        initialized = 1;
    }
    
    if (Size == 0) {
        // Use default test data for empty input
        const char *default_data = "test";
        Size = 4;
        Data = (const uint8_t *)default_data;
    }
    
    // Create a protected sqlite3_value using sqlite3_bind_value approach
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT fuzz_func(?)";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Bind the fuzzer data as a blob
        sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_STATIC);
        
        // Execute the statement which will call our dummy_function
        sqlite3_step(stmt);
        
        // Clean up
        sqlite3_finalize(stmt);
    }
    
    // Also test with different value types
    sqlite3_stmt *stmt2 = NULL;
    const char *sql2 = "SELECT fuzz_func(?)";
    
    if (sqlite3_prepare_v2(db, sql2, -1, &stmt2, NULL) == SQLITE_OK) {
        // Try binding as text too
        sqlite3_bind_text(stmt2, 1, (const char *)Data, (int)Size, SQLITE_STATIC);
        
        // Execute
        sqlite3_step(stmt2);
        
        // Clean up
        sqlite3_finalize(stmt2);
    }
    
    // Test with integer binding
    sqlite3_stmt *stmt3 = NULL;
    const char *sql3 = "SELECT fuzz_func(?)";
    
    if (sqlite3_prepare_v2(db, sql3, -1, &stmt3, NULL) == SQLITE_OK) {
        // Convert first 4 bytes to integer if possible
        int int_value = 0;
        if (Size >= sizeof(int)) {
            memcpy(&int_value, Data, sizeof(int));
        } else if (Size > 0) {
            int_value = Data[0];
        }
        sqlite3_bind_int(stmt3, 1, int_value);
        
        // Execute
        sqlite3_step(stmt3);
        
        // Clean up
        sqlite3_finalize(stmt3);
    }
    
    return 0;
}