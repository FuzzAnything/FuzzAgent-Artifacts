// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:78603:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:78509:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:78462:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void fuzz_sqlite3_value_functions(sqlite3_value* value) {
    // Test sqlite3_value_frombind
    int from_bind = sqlite3_value_frombind(value);
    (void)from_bind; // Suppress unused variable warning
    
    // Test sqlite3_value_bytes
    int bytes = sqlite3_value_bytes(value);
    (void)bytes;
    
    // Test sqlite3_value_type
    int type = sqlite3_value_type(value);
    (void)type;
    
    // Test sqlite3_value_int
    int int_val = sqlite3_value_int(value);
    (void)int_val;
    
    // Test sqlite3_value_bytes16
    int bytes16 = sqlite3_value_bytes16(value);
    (void)bytes16;
    
    // Test sqlite3_value_blob
    const void* blob = sqlite3_value_blob(value);
    (void)blob;
}

static sqlite3_value* create_test_value(const uint8_t* data, size_t size) {
    // Create a simple in-memory database
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    
    // Create a table
    char* err_msg = NULL;
    const char* create_table_sql = "CREATE TABLE test (id INTEGER, data BLOB);";
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return NULL;
    }
    
    // Prepare an INSERT statement with a bound parameter
    sqlite3_stmt* stmt = NULL;
    const char* insert_sql = "INSERT INTO test (id, data) VALUES (1, ?);";
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Bind the fuzzer data as a BLOB parameter
    if (size > 0) {
        sqlite3_bind_blob(stmt, 1, data, (int)(size > 1024 ? 1024 : size), SQLITE_STATIC);
    } else {
        sqlite3_bind_null(stmt, 1);
    }
    
    // Execute the INSERT
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    // Prepare a SELECT statement to get a protected value
    const char* select_sql = "SELECT data FROM test WHERE id = 1;";
    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    
    // Execute and get the value
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Get an unprotected value from column
        sqlite3_value* unprotected_value = sqlite3_column_value(stmt, 0);
        
        // Create a protected value by duplicating the unprotected value
        sqlite3_value* protected_value = sqlite3_value_dup(unprotected_value);
        
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        
        return protected_value;
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return NULL;
}

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    // Create a test value from fuzzer input
    sqlite3_value* test_value = create_test_value(Data, Size);
    
    if (test_value) {
        // Test all the target functions with the protected value
        fuzz_sqlite3_value_functions(test_value);
        
        // Free the duplicated value
        sqlite3_value_free(test_value);
    }
    
    // Also test with NULL value
    fuzz_sqlite3_value_functions(NULL);
    
    // Try to create a value with various SQLite data types
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        // Test with different value types using SQL functions
        const char* test_sql[] = {
            "SELECT NULL",
            "SELECT 42",
            "SELECT 3.14159",
            "SELECT 'test string'",
            "SELECT x'010203'"
        };
        
        for (int i = 0; i < 5; i++) {
            sqlite3_stmt* stmt = NULL;
            if (sqlite3_prepare_v2(db, test_sql[i], -1, &stmt, NULL) == SQLITE_OK) {
                if (sqlite3_step(stmt) == SQLITE_ROW) {
                    sqlite3_value* col_value = sqlite3_column_value(stmt, 0);
                    sqlite3_value* dup_value = sqlite3_value_dup(col_value);
                    if (dup_value) {
                        fuzz_sqlite3_value_functions(dup_value);
                        sqlite3_value_free(dup_value);
                    }
                }
                sqlite3_finalize(stmt);
            }
        }
        
        sqlite3_close(db);
    }
    
    return 0;
}