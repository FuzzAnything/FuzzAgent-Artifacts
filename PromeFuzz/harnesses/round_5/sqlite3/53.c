// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_value_free at sqlite3.c:78635:17 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_dup at sqlite3.c:78609:27 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - just a dummy destructor for pointer binding
}

static int create_test_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    // Create a test table with various column types
    const char *create_table_sql = 
        "CREATE TABLE test_table ("
        "id INTEGER PRIMARY KEY, "
        "text_col TEXT, "
        "int_col INTEGER, "
        "real_col REAL, "
        "blob_col BLOB"
        ");";
    
    rc = sqlite3_exec(*db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    // Prepare an INSERT statement with multiple parameters
    const char *insert_sql = 
        "INSERT INTO test_table (text_col, int_col, real_col, blob_col) "
        "VALUES (?, ?, ?, ?);";
    
    rc = sqlite3_prepare_v2(*db, insert_sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    return SQLITE_OK;
}

static sqlite3_value* create_test_value(sqlite3 *db, const uint8_t *data, size_t size) {
    // Create a value by executing a simple SELECT statement and extracting the value
    sqlite3_value *value = NULL;
    sqlite3_stmt *temp_stmt = NULL;
    const char *sql = NULL;
    
    if (size == 0) {
        // Create a NULL value
        sql = "SELECT NULL";
    } else if (size % 4 == 0) {
        // Treat as integer
        sql = "SELECT 42";
    } else if (size % 3 == 0) {
        // Treat as real
        sql = "SELECT 3.14";
    } else if (size % 2 == 0) {
        // Treat as text
        sql = "SELECT 'test_text'";
    } else {
        // Treat as blob
        sql = "SELECT x'010203'";
    }
    
    if (sqlite3_prepare_v2(db, sql, -1, &temp_stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(temp_stmt) == SQLITE_ROW) {
            value = sqlite3_column_value(temp_stmt, 0);
            // Duplicate the value so we can use it independently
            value = sqlite3_value_dup(value);
        }
        sqlite3_finalize(temp_stmt);
    }
    
    return value;
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Create test database and prepared statement
    rc = create_test_database(&db, &stmt);
    if (rc != SQLITE_OK) {
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
    
    // Use fuzzer data to determine binding operations
    for (int i = 1; i <= param_count && i <= 4; i++) {
        // Use different binding methods based on fuzzer input
        size_t index = (i - 1) % (Size > 0 ? Size : 1);
        
        switch (Data[index] % 6) {
            case 0:
                // Bind NULL
                sqlite3_bind_null(stmt, i);
                break;
                
            case 1:
                // Bind value
                if (Size > 0 && db) {
                    sqlite3_value *value = create_test_value(db, Data, Size);
                    if (value) {
                        sqlite3_bind_value(stmt, i, value);
                        sqlite3_value_free(value);
                    }
                }
                break;
                
            case 2:
                // Bind pointer
                sqlite3_bind_pointer(stmt, i, (void*)Data, "test_pointer", dummy_destructor);
                break;
                
            case 3:
                // Bind zeroblob
                sqlite3_bind_zeroblob(stmt, i, (int)(Size % 1000));
                break;
                
            case 4:
                // Clear bindings and rebind
                sqlite3_clear_bindings(stmt);
                sqlite3_bind_null(stmt, i);
                break;
                
            case 5:
                // Test parameter count again
                sqlite3_bind_parameter_count(stmt);
                // Bind a simple value
                sqlite3_bind_null(stmt, i);
                break;
        }
    }
    
    // Try to step the statement (execute it)
    rc = sqlite3_step(stmt);
    
    // Reset statement for potential reuse
    sqlite3_reset(stmt);
    
    // Clear all bindings
    sqlite3_clear_bindings(stmt);
    
    // Test with different parameter indices (including out of bounds)
    if (Size > 0) {
        int test_idx = Data[0] % 10;
        sqlite3_bind_null(stmt, test_idx);
        
        // Test negative index
        sqlite3_bind_null(stmt, -1);
        
        // Test large index
        sqlite3_bind_null(stmt, 1000);
    }
    
    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}