// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *dummy_sql = "SELECT ?, ?, ?";
    const char *dummy_sql2 = "INSERT INTO test VALUES (?, ?, ?)";
    int rc;
    
    // Open in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) return 0;
    
    // Create a dummy table
    rc = sqlite3_exec(db, "CREATE TABLE test(a, b, c)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare first statement
    rc = sqlite3_prepare_v2(db, dummy_sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK || !stmt1) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare second statement
    rc = sqlite3_prepare_v2(db, dummy_sql2, -1, &stmt2, NULL);
    if (rc != SQLITE_OK || !stmt2) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt1);
    
    // Use fuzzer data to determine operations
    size_t data_index = 0;
    
    // Perform various operations based on fuzzer input
    while (data_index < Size && param_count > 0) {
        uint8_t op = Data[data_index++] % 6;
        
        switch (op) {
            case 0: // sqlite3_bind_null
                if (data_index < Size) {
                    int idx = Data[data_index++] % param_count;
                    if (idx < 1) idx = 1; // Parameters are 1-indexed
                    if (idx <= param_count) {
                        sqlite3_bind_null(stmt1, idx);
                    }
                }
                break;
                
            case 1: // sqlite3_bind_parameter_count (just call it)
                sqlite3_bind_parameter_count(stmt1);
                break;
                
            case 2: // sqlite3_transfer_bindings
                if (sqlite3_bind_parameter_count(stmt1) == sqlite3_bind_parameter_count(stmt2)) {
                    sqlite3_transfer_bindings(stmt1, stmt2);
                }
                break;
                
            case 3: // sqlite3_clear_bindings
                sqlite3_clear_bindings(stmt1);
                break;
                
            case 4: // sqlite3_reset
                sqlite3_reset(stmt1);
                break;
                
            case 5: // sqlite3_bind_value
                // We can't create sqlite3_value directly, but we can get one from a column
                // First, insert a row with some data
                sqlite3_stmt *stmt_insert = NULL;
                const char *insert_sql = "INSERT INTO test VALUES (?, ?, ?)";
                rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert, NULL);
                if (rc == SQLITE_OK && stmt_insert) {
                    // Bind some values
                    sqlite3_bind_int(stmt_insert, 1, 42);
                    sqlite3_bind_text(stmt_insert, 2, "test", 4, SQLITE_STATIC);
                    sqlite3_bind_blob(stmt_insert, 3, Data, (Size - data_index > 16) ? 16 : Size - data_index, SQLITE_STATIC);
                    sqlite3_step(stmt_insert);
                    sqlite3_finalize(stmt_insert);
                    
                    // Now select the row to get a sqlite3_value
                    sqlite3_stmt *stmt_select = NULL;
                    const char *select_sql = "SELECT * FROM test ORDER BY rowid DESC LIMIT 1";
                    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt_select, NULL);
                    if (rc == SQLITE_OK && stmt_select) {
                        if (sqlite3_step(stmt_select) == SQLITE_ROW) {
                            // Get a value from the first column
                            const sqlite3_value *col_value = sqlite3_column_value(stmt_select, 0);
                            if (col_value && data_index < Size) {
                                int idx = Data[data_index++] % param_count;
                                if (idx < 1) idx = 1;
                                if (idx <= param_count) {
                                    sqlite3_bind_value(stmt1, idx, col_value);
                                }
                            }
                        }
                        sqlite3_finalize(stmt_select);
                    }
                }
                break;
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}