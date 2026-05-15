// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_bind_pointer at sqlite3.c:80042:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - just a dummy destructor for testing
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "CREATE TABLE IF NOT EXISTS test (a, b, c, d, e, f)";
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create the test table
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    
    // Prepare a simple statement with parameters
    rc = sqlite3_prepare_v2(db, "INSERT INTO test VALUES (?, ?, ?, ?, ?, ?)", -1, &stmt, NULL);
    if (rc != SQLITE_OK || !stmt) {
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count <= 0) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer data to determine which functions to call and with what parameters
    // We'll use the first byte to determine the operation
    uint8_t op_selector = Data[0];
    
    // Calculate parameter index (ensure it's within bounds)
    int param_idx = (op_selector % param_count) + 1;
    if (param_idx < 1) param_idx = 1;
    
    // Test different binding functions based on fuzzer input
    switch (op_selector % 6) {
        case 0:
            // Test sqlite3_bind_int64
            if (Size >= sizeof(sqlite3_int64) + 1) {
                sqlite3_int64 int_val;
                memcpy(&int_val, Data + 1, sizeof(sqlite3_int64));
                sqlite3_bind_int64(stmt, param_idx, int_val);
            }
            break;
            
        case 1:
            // Test sqlite3_bind_value - we need to create a value using SQLite API
            // First prepare a statement that returns a value
            {
                sqlite3_stmt *val_stmt = NULL;
                const char *val_sql = "SELECT ?";
                if (sqlite3_prepare_v2(db, val_sql, -1, &val_stmt, NULL) == SQLITE_OK && val_stmt) {
                    // Bind some data to the value statement
                    if (Size > 1) {
                        sqlite3_bind_text(val_stmt, 1, (const char*)(Data + 1), 
                                         Size > 100 ? 100 : Size - 1, SQLITE_TRANSIENT);
                    }
                    
                    // Step to get the value
                    if (sqlite3_step(val_stmt) == SQLITE_ROW) {
                        // Get the value from the column
                        const sqlite3_value *value = sqlite3_column_value(val_stmt, 0);
                        sqlite3_bind_value(stmt, param_idx, value);
                    }
                    sqlite3_finalize(val_stmt);
                }
            }
            break;
            
        case 2:
            // Test sqlite3_bind_pointer
            if (Size > 1) {
                void *ptr = (void*)(Data + 1);
                const char *type = "test_type";
                sqlite3_bind_pointer(stmt, param_idx, ptr, type, dummy_destructor);
            }
            break;
            
        case 3:
            // Test sqlite3_bind_parameter_count (just call it again)
            sqlite3_bind_parameter_count(stmt);
            break;
            
        case 4:
            // Test sqlite3_bind_text16
            if (Size > 1) {
                const void *text = (const void*)(Data + 1);
                int text_len = Size - 1;
                if (text_len > 0) {
                    sqlite3_bind_text16(stmt, param_idx, text, text_len, SQLITE_TRANSIENT);
                }
            }
            break;
            
        case 5:
            // Test sqlite3_bind_text64
            if (Size > 1) {
                const char *text = (const char*)(Data + 1);
                sqlite3_uint64 text_len = Size - 1;
                if (text_len > 0) {
                    sqlite3_bind_text64(stmt, param_idx, text, text_len, 
                                       SQLITE_TRANSIENT, SQLITE_UTF8);
                }
            }
            break;
    }
    
    // Try to step the statement (ignore errors)
    sqlite3_step(stmt);
    
    // Reset the statement for potential reuse
    sqlite3_reset(stmt);
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}