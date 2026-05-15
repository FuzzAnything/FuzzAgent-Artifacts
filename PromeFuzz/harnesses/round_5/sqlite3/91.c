// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *dummy_sql = "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT);";
    const char *param_sql = "INSERT INTO test VALUES(?, ?);";
    const char *select_sql = "SELECT id, name FROM test LIMIT 1;";
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table
    rc = sqlite3_exec(db, dummy_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare first statement
    rc = sqlite3_prepare_v2(db, param_sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK || !stmt1) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement (same SQL)
    rc = sqlite3_prepare_v2(db, param_sql, -1, &stmt2, NULL);
    if (rc != SQLITE_OK || !stmt2) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Test sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt1);
    
    // Test sqlite3_bind_int64
    if (param_count > 0) {
        sqlite3_int64 int_val = (Size > 0) ? (sqlite3_int64)Data[0] : 0;
        int bind_idx = 1 + (Data[0] % param_count);
        if (bind_idx > 0 && bind_idx <= param_count) {
            sqlite3_bind_int64(stmt1, bind_idx, int_val);
        }
    }
    
    // Create a value using SQLite's own functions
    // First, insert a row to have some data
    sqlite3_stmt *insert_stmt = NULL;
    rc = sqlite3_prepare_v2(db, "INSERT INTO test VALUES(?, ?)", -1, &insert_stmt, NULL);
    if (rc == SQLITE_OK && insert_stmt) {
        if (Size > 0) {
            sqlite3_bind_int64(insert_stmt, 1, (sqlite3_int64)Data[0]);
        }
        if (Size > 1) {
            sqlite3_bind_text(insert_stmt, 2, (const char*)(Data + 1), 
                             (int)(Size - 1 > 100 ? 100 : Size - 1), SQLITE_TRANSIENT);
        }
        sqlite3_step(insert_stmt);
        sqlite3_finalize(insert_stmt);
    }
    
    // Now get a value from the table to use with sqlite3_bind_value
    sqlite3_stmt *select_stmt = NULL;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, NULL);
    if (rc == SQLITE_OK && select_stmt) {
        if (sqlite3_step(select_stmt) == SQLITE_ROW) {
            // Get a value from the result
            const sqlite3_value *col_value = sqlite3_column_value(select_stmt, 0);
            if (col_value && param_count > 0) {
                int bind_idx = 1 + (Data[0] % param_count);
                if (bind_idx > 0 && bind_idx <= param_count) {
                    sqlite3_bind_value(stmt1, bind_idx, col_value);
                }
            }
        }
        sqlite3_finalize(select_stmt);
    }
    
    // Test sqlite3_transfer_bindings
    if (stmt1 && stmt2) {
        sqlite3_transfer_bindings(stmt1, stmt2);
    }
    
    // Test sqlite3_expired
    if (stmt1) {
        int expired = sqlite3_expired(stmt1);
        (void)expired;
    }
    
    // Test sqlite3_reset
    if (stmt1) {
        sqlite3_reset(stmt1);
    }
    if (stmt2) {
        sqlite3_reset(stmt2);
    }
    
    // Cleanup
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (db) sqlite3_close(db);
    
    return 0;
}