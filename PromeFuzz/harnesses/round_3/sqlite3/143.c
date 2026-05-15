// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
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

static void create_dummy_db(sqlite3 **db, sqlite3_stmt **stmt1, sqlite3_stmt **stmt2) {
    int rc;
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    // Create a table and prepare statements with parameters
    sqlite3_exec(*db, "CREATE TABLE t1 (a INT, b TEXT);", NULL, NULL, NULL);
    sqlite3_exec(*db, "INSERT INTO t1 VALUES (1, 'test');", NULL, NULL, NULL);
    
    // Prepare statements with parameters
    sqlite3_prepare_v2(*db, "SELECT * FROM t1 WHERE a = ? AND b = ?;", -1, stmt1, NULL);
    sqlite3_prepare_v2(*db, "SELECT * FROM t1 WHERE a = ? OR b = ?;", -1, stmt2, NULL);
}

static void cleanup_db(sqlite3 *db, sqlite3_stmt *stmt1, sqlite3_stmt *stmt2) {
    if (stmt1) sqlite3_finalize(stmt1);
    if (stmt2) sqlite3_finalize(stmt2);
    if (db) sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL, *stmt2 = NULL;
    
    create_dummy_db(&db, &stmt1, &stmt2);
    if (!db || !stmt1 || !stmt2) {
        cleanup_db(db, stmt1, stmt2);
        return 0;
    }
    
    // Use fuzzer data to determine parameters
    size_t data_idx = 0;
    
    // 1. Test sqlite3_bind_parameter_count
    int param_count1 = sqlite3_bind_parameter_count(stmt1);
    int param_count2 = sqlite3_bind_parameter_count(stmt2);
    
    // 2. Test sqlite3_expired - initially should be 0
    int expired1 = sqlite3_expired(stmt1);
    int expired2 = sqlite3_expired(stmt2);
    
    // 3. Test sqlite3_bind_value - create a value from column of executed statement
    sqlite3_stmt *stmt3 = NULL;
    sqlite3_prepare_v2(db, "SELECT a, b FROM t1;", -1, &stmt3, NULL);
    if (stmt3) {
        if (sqlite3_step(stmt3) == SQLITE_ROW) {
            const sqlite3_value *val = sqlite3_column_value(stmt3, 0);
            if (param_count1 > 0) {
                int idx = (Size > 0) ? (Data[data_idx % Size] % param_count1) + 1 : 1;
                sqlite3_bind_value(stmt1, idx, val);
            }
        }
        sqlite3_finalize(stmt3);
    }
    
    // 4. Test sqlite3_bind_null
    if (param_count1 > 0 && Size > 0) {
        for (int i = 1; i <= param_count1 && data_idx < Size; i++) {
            if (Data[data_idx] % 2 == 0) {
                sqlite3_bind_null(stmt1, i);
            }
            data_idx = (data_idx + 1) % Size;
        }
    }
    
    // 5. Test sqlite3_transfer_bindings
    if (param_count1 == param_count2) {
        sqlite3_transfer_bindings(stmt1, stmt2);
    }
    
    // 6. Test sqlite3_reset
    sqlite3_reset(stmt1);
    sqlite3_reset(stmt2);
    
    // Try to expire a statement by schema change
    sqlite3_exec(db, "ALTER TABLE t1 ADD COLUMN c INT;", NULL, NULL, NULL);
    expired1 = sqlite3_expired(stmt1);
    expired2 = sqlite3_expired(stmt2);
    
    // Test bindings after reset
    if (param_count1 > 0 && Size > 0) {
        data_idx = 0;
        for (int i = 1; i <= param_count1 && data_idx < Size; i++) {
            sqlite3_bind_null(stmt1, i);
            data_idx = (data_idx + 1) % Size;
        }
    }
    
    // Final reset
    sqlite3_reset(stmt1);
    sqlite3_reset(stmt2);
    
    cleanup_db(db, stmt1, stmt2);
    return 0;
}