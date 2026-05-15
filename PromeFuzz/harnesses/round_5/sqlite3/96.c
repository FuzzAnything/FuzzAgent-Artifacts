// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:78474:25 in sqlite3.h
// sqlite3_value_text16le at sqlite3.c:78501:24 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_text16 at sqlite3.c:78495:24 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_result_value at sqlite3.c:78892:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void test_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    if (argc < 1) return;
    
    sqlite3_value *value = argv[0];
    
    // Test sqlite3_value_blob
    const void *blob_data = sqlite3_value_blob(value);
    int blob_bytes = sqlite3_value_bytes(value);
    
    // Test sqlite3_value_subtype
    unsigned int subtype = sqlite3_value_subtype(value);
    
    // Test sqlite3_value_text16le
    const void *text16le_data = sqlite3_value_text16le(value);
    int text16le_bytes = sqlite3_value_bytes16(value);
    
    // Test sqlite3_value_int
    int int_val = sqlite3_value_int(value);
    
    // Test sqlite3_value_text16
    const void *text16_data = sqlite3_value_text16(value);
    int text16_bytes = sqlite3_value_bytes16(value);
    
    // Test sqlite3_result_value
    sqlite3_result_value(context, value);
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    
    // Initialize database and function once
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        // Create a test table
        char *create_err = NULL;
        const char *create_sql = "CREATE TABLE test (data BLOB);";
        if (sqlite3_exec(db, create_sql, NULL, NULL, &create_err) != SQLITE_OK) {
            sqlite3_free(create_err);
            sqlite3_close(db);
            return 0;
        }
        
        // Register our test function
        if (sqlite3_create_function(db, "test_func", 1, SQLITE_UTF8 | SQLITE_SUBTYPE, NULL, 
                                    test_function, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        // Prepare a statement to call our function
        const char *select_sql = "SELECT test_func(?)";
        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        initialized = 1;
    }
    
    if (!db || !stmt) return 0;
    
    // Reset the statement
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    
    // Bind the fuzzer input as a BLOB
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_TRANSIENT);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    // Execute the statement to trigger our test function
    sqlite3_step(stmt);
    
    return 0;
}