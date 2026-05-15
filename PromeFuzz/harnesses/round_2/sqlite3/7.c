// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static void initialize_database(sqlite3 **db, sqlite3_stmt **stmt1, sqlite3_stmt **stmt2) {
    if (sqlite3_open(":memory:", db) != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    // Create a table and insert some data
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, name TEXT, data BLOB);";
    sqlite3_exec(*db, create_sql, 0, 0, 0);
    
    const char *insert_sql = "INSERT INTO test (name, data) VALUES ('test', X'01020304');";
    sqlite3_exec(*db, insert_sql, 0, 0, 0);
    
    // Prepare statements for use
    const char *select_sql = "SELECT * FROM test;";
    sqlite3_prepare_v2(*db, select_sql, -1, stmt1, 0);
    
    const char *insert_param_sql = "INSERT INTO test (name, data) VALUES (?, ?);";
    sqlite3_prepare_v2(*db, insert_param_sql, -1, stmt2, 0);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    
    initialize_database(&db, &stmt1, &stmt2);
    if (!db || !stmt1 || !stmt2) {
        if (stmt1) sqlite3_finalize(stmt1);
        if (stmt2) sqlite3_finalize(stmt2);
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Step to get first row
    sqlite3_step(stmt1);
    
    // Use fuzz data to determine column indices (modulo number of columns)
    int col_count = sqlite3_column_count(stmt1);
    if (col_count == 0) {
        sqlite3_finalize(stmt1);
        sqlite3_finalize(stmt2);
        sqlite3_close(db);
        return 0;
    }
    
    // Use first byte for column index variations
    int idx1 = 0;
    int idx2 = 0;
    if (Size > 0) {
        idx1 = Data[0] % col_count;
        if (Size > 1) {
            idx2 = Data[1] % col_count;
        }
    }
    
    // Target function sequence
    sqlite3_column_text(stmt1, idx1);
    sqlite3_bind_value(stmt2, 1, sqlite3_column_value(stmt1, idx2));
    sqlite3_column_value(stmt1, idx1);
    sqlite3_bind_value(stmt2, 2, sqlite3_column_value(stmt1, idx2));
    sqlite3_column_value(stmt1, idx1);
    sqlite3_step(stmt1);
    sqlite3_column_int(stmt1, idx1);
    sqlite3_reset(stmt1);
    sqlite3_column_bytes16(stmt1, idx1);
    sqlite3_column_blob(stmt1, idx1);
    sqlite3_column_bytes16(stmt1, idx2);
    sqlite3_column_blob(stmt1, idx2);
    sqlite3_column_bytes(stmt1, idx1);
    sqlite3_column_blob(stmt1, idx1);
    sqlite3_column_bytes(stmt1, idx2);
    sqlite3_column_blob(stmt1, idx2);
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}