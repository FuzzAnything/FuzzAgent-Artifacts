// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:79664:33 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_bind_value at sqlite3.c:80096:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
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

static int create_test_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    char *err_msg = NULL;
    
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, 
        "CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT, data BLOB);"
        "INSERT INTO test(name, data) VALUES('test1', x'010203');"
        "INSERT INTO test(name, data) VALUES('test2', x'04050607');"
        "INSERT INTO test(name, data) VALUES('test3', NULL);", 
        NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        return rc;
    }
    
    rc = sqlite3_prepare_v2(*db, "SELECT * FROM test WHERE id = ?", -1, stmt, NULL);
    return rc;
}

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    if (Size < 8) return 0;
    
    rc = create_test_database(&db, &stmt);
    if (rc != SQLITE_OK) goto cleanup;
    
    int param_count = sqlite3_bind_parameter_count(stmt);
    if (param_count > 0) {
        int id = (int)(Data[0] % 3) + 1;
        sqlite3_bind_int(stmt, 1, id);
    }
    
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int col_count = sqlite3_column_count(stmt);
        if (col_count > 0) {
            int col1 = (int)(Data[1] % col_count);
            int col2 = (int)(Data[2] % col_count);
            int col3 = (int)(Data[3] % col_count);
            
            sqlite3_column_text(stmt, col1);
            
            sqlite3_stmt *stmt2 = NULL;
            rc = sqlite3_prepare_v2(db, "INSERT INTO test(name, data) VALUES(?, ?)", -1, &stmt2, NULL);
            if (rc == SQLITE_OK && stmt2) {
                sqlite3_value *val1 = sqlite3_column_value(stmt, col1);
                sqlite3_bind_value(stmt2, 1, val1);
                
                sqlite3_value *val2 = sqlite3_column_value(stmt, col2);
                sqlite3_bind_value(stmt2, 2, val2);
                
                sqlite3_step(stmt2);
                sqlite3_reset(stmt2);
                
                sqlite3_finalize(stmt2);
            }
            
            sqlite3_column_value(stmt, col3);
            sqlite3_step(stmt);
            sqlite3_column_int(stmt, col1);
            sqlite3_reset(stmt);
            
            sqlite3_column_bytes16(stmt, col1);
            sqlite3_column_blob(stmt, col1);
            sqlite3_column_bytes16(stmt, col2);
            sqlite3_column_blob(stmt, col2);
            sqlite3_column_bytes(stmt, col3);
            sqlite3_column_blob(stmt, col3);
            sqlite3_column_bytes(stmt, col1);
            sqlite3_column_blob(stmt, col1);
        }
    }
    
cleanup:
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    return 0;
}