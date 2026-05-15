// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_subtype at sqlite3.c:78474:25 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
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
    // This is a dummy SQL function implementation
    // It doesn't need to do anything meaningful for fuzzing
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_value *value = NULL;
    sqlite3_value *dup_value = NULL;
    const void *blob_ptr = NULL;
    int blob_size = 0;
    int int_val = 0;
    unsigned int subtype = 0;
    const char *sql = "CREATE TABLE IF NOT EXISTS test (data BLOB);";
    const char *insert_sql = "INSERT INTO test (data) VALUES (?);";
    const char *select_sql = "SELECT data FROM test LIMIT 1;";
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Create a table
    if (sqlite3_exec(db, sql, NULL, NULL, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Prepare insert statement
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Bind the fuzzer data as a BLOB
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, Data, (int)Size, SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    // Execute insert
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    
    // Prepare select statement
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        goto cleanup;
    }
    
    // Execute select
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get an unprotected value from the column
        value = sqlite3_column_value(stmt, 0);
        
        if (value) {
            // Create a protected duplicate of the value
            dup_value = sqlite3_value_dup(value);
            
            if (dup_value) {
                // Test sqlite3_value_blob
                blob_ptr = sqlite3_value_blob(dup_value);
                
                // Test sqlite3_value_bytes
                blob_size = sqlite3_value_bytes(dup_value);
                
                // Test sqlite3_value_int
                int_val = sqlite3_value_int(dup_value);
                
                // Test sqlite3_value_subtype
                subtype = sqlite3_value_subtype(dup_value);
                
                // Test sqlite3_result_value by creating a custom function
                // First create a function that uses sqlite3_result_value
                sqlite3_create_function(
                    db, 
                    "test_func", 
                    1, 
                    SQLITE_UTF8 | SQLITE_SUBTYPE, 
                    NULL, 
                    dummy_function, 
                    NULL, 
                    NULL
                );
                
                // Prepare a statement that calls our function
                sqlite3_finalize(stmt);
                stmt = NULL;
                const char *func_sql = "SELECT test_func(?)";
                if (sqlite3_prepare_v2(db, func_sql, -1, &stmt, NULL) == SQLITE_OK) {
                    // Bind the value to the function parameter
                    sqlite3_bind_value(stmt, 1, dup_value);
                    sqlite3_step(stmt);
                }
                
                // Free the duplicated value
                sqlite3_value_free(dup_value);
                dup_value = NULL;
            }
        }
    }
    
cleanup:
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}