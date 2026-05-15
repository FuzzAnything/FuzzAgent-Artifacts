// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing, just for testing
}

static int create_test_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    // Create a test table with various column types
    const char *create_table_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "data BLOB, "
        "value REAL, "
        "ptr_value POINTER"
        ");";
    
    rc = sqlite3_exec(*db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    // Prepare a statement with parameters for binding
    const char *insert_sql = 
        "INSERT INTO test_table (name, data, value, ptr_value) "
        "VALUES (?, ?, ?, ?);";
    
    rc = sqlite3_prepare_v2(*db, insert_sql, -1, stmt, NULL);
    return rc;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    // Create test database and prepared statement
    int rc = create_test_database(&db, &stmt);
    if (rc != SQLITE_OK || !stmt) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data to determine which bindings to test
    size_t data_index = 0;
    
    // Test sqlite3_bind_null on some parameters
    for (int i = 1; i <= param_count && i <= 4 && data_index < Size; i++) {
        if ((Data[data_index++] % 2) == 0) {
            sqlite3_bind_null(stmt, i);
        }
    }
    
    // Test sqlite3_bind_zeroblob
    if (data_index < Size && param_count >= 2) {
        int blob_size = Data[data_index++] % 1024; // Limit size
        sqlite3_bind_zeroblob(stmt, 2, blob_size);
    }
    
    // Test sqlite3_bind_pointer
    if (data_index < Size && param_count >= 4) {
        void *test_ptr = (void*)(uintptr_t)Data[data_index];
        const char *ptr_type = "test_pointer";
        sqlite3_bind_pointer(stmt, 4, test_ptr, ptr_type, dummy_destructor);
        data_index = (data_index + 1) % Size;
    }
    
    // Test sqlite3_bind_value - we need to create a valid sqlite3_value
    // We'll create a simple SELECT statement to get a sqlite3_value
    if (data_index < Size && param_count >= 3) {
        // First insert a row to have data to select
        sqlite3_stmt *select_stmt = NULL;
        const char *select_sql = "SELECT name FROM test_table LIMIT 1;";
        
        if (sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL) == SQLITE_OK) {
            // Insert a dummy row first
            sqlite3_stmt *insert_dummy = NULL;
            const char *insert_dummy_sql = "INSERT INTO test_table (name) VALUES ('test');";
            
            if (sqlite3_prepare_v2(db, insert_dummy_sql, -1, &insert_dummy, NULL) == SQLITE_OK) {
                sqlite3_step(insert_dummy);
                sqlite3_finalize(insert_dummy);
                
                // Now select the value
                if (sqlite3_step(select_stmt) == SQLITE_ROW) {
                    const sqlite3_value *value = sqlite3_column_value(select_stmt, 0);
                    if (value) {
                        sqlite3_bind_value(stmt, 3, value);
                    }
                }
            }
            sqlite3_finalize(select_stmt);
        }
    }
    
    // Try to execute the statement
    sqlite3_step(stmt);
    
    // Test sqlite3_clear_bindings
    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);
    
    // Try executing again with cleared bindings
    sqlite3_step(stmt);
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}