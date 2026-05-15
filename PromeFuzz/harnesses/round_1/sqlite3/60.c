// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_free(void *ptr) {
    // Do nothing - used for sqlite3_bind_blob64
}

int LLVMFuzzerTestOneInput_60(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_stmt *stmt3 = NULL;
    int rc;
    
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a table for testing
    const char *create_table_sql = "CREATE TABLE test (id INTEGER, data BLOB);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare statements for testing
    const char *insert_sql1 = "INSERT INTO test VALUES (?, ?);";
    const char *insert_sql2 = "INSERT INTO test VALUES (?, ?);";
    const char *select_sql = "SELECT * FROM test LIMIT 1;";
    
    if (sqlite3_prepare_v2(db, insert_sql1, -1, &stmt1, NULL) != SQLITE_OK ||
        sqlite3_prepare_v2(db, insert_sql2, -1, &stmt2, NULL) != SQLITE_OK ||
        sqlite3_prepare_v2(db, select_sql, -1, &stmt3, NULL) != SQLITE_OK) {
        if (stmt1) sqlite3_finalize(stmt1);
        if (stmt2) sqlite3_finalize(stmt2);
        if (stmt3) sqlite3_finalize(stmt3);
        sqlite3_close(db);
        return 0;
    }
    
    // Use fuzzer input to generate parameters
    if (Size > 0) {
        // Get parameter count for stmt1
        int param_count = sqlite3_bind_parameter_count(stmt1);
        
        if (param_count > 0) {
            // Use first byte for parameter index (mod param_count to stay in bounds)
            int param_idx = Data[0] % param_count;
            if (param_idx == 0) param_idx = 1; // SQLite parameters are 1-indexed
            
            // Use next bytes for size values
            sqlite3_uint64 blob_size = 0;
            if (Size >= 9) {
                memcpy(&blob_size, Data + 1, sizeof(sqlite3_uint64));
            } else if (Size >= 2) {
                blob_size = Data[1];
            }
            
            // Test sqlite3_bind_zeroblob64
            sqlite3_bind_zeroblob64(stmt1, param_idx, blob_size);
            
            // Test sqlite3_bind_blob64 with fuzzer data
            if (Size > 9) {
                size_t data_size = Size - 9;
                if (data_size > 0) {
                    const void *blob_data = Data + 9;
                    sqlite3_bind_blob64(stmt1, param_idx, blob_data, 
                                       (data_size < blob_size) ? data_size : blob_size,
                                       dummy_free);
                }
            }
            
            // Execute and reset
            sqlite3_step(stmt1);
            sqlite3_reset(stmt1);
            
            // Test sqlite3_bind_value
            if (stmt3) {
                if (sqlite3_step(stmt3) == SQLITE_ROW) {
                    sqlite3_value *value = sqlite3_column_value(stmt3, 0);
                    if (value) {
                        sqlite3_bind_value(stmt1, param_idx, value);
                    }
                    sqlite3_reset(stmt3);
                }
            }
            
            // Test sqlite3_transfer_bindings
            if (stmt2) {
                sqlite3_transfer_bindings(stmt1, stmt2);
                sqlite3_step(stmt2);
                sqlite3_reset(stmt2);
            }
            
            // Test sqlite3_reset on both statements
            sqlite3_reset(stmt1);
            if (stmt2) sqlite3_reset(stmt2);
            
            // Get parameter count again
            sqlite3_bind_parameter_count(stmt1);
            if (stmt2) sqlite3_bind_parameter_count(stmt2);
        }
    }
    
    // Cleanup
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (stmt3) sqlite3_finalize(stmt3);
    sqlite3_close(db);
    
    return 0;
}