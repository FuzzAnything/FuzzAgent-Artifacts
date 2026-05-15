// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:79659:25 in sqlite3.h
// sqlite3_column_double at sqlite3.c:79649:19 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:79639:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_bytes16 at sqlite3.c:79644:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:79629:24 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:78593:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "int_col INTEGER, "
        "real_col REAL, "
        "text_col TEXT, "
        "blob_col BLOB"
        ");";
    const char *insert_sql = 
        "INSERT INTO test_table (int_col, real_col, text_col, blob_col) "
        "VALUES (42, 3.14, 'test text', x'01020304');";
    const char *select_sql = "SELECT * FROM test_table;";
    
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    rc = sqlite3_exec(*db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_exec(*db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_step(*stmt);
    if (rc != SQLITE_ROW) goto cleanup;
    
    return;
    
cleanup:
    if (*stmt) sqlite3_finalize(*stmt);
    if (*db) sqlite3_close(*db);
    *stmt = NULL;
    *db = NULL;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int col_count, col_type;
    sqlite3_int64 int_val;
    double double_val;
    int encoding;
    sqlite3_value *value;
    int bytes;
    const void *blob;
    int bytes16;
    const void *blob2;
    int bytes16_2;
    const void *blob3;
    int encoding2;
    sqlite3_value *value2;
    int bytes2;
    const void *blob4;
    
    initialize_database(&db, &stmt);
    if (!db || !stmt) return 0;
    
    col_count = sqlite3_column_count(stmt);
    if (col_count > 0) {
        int col_index = 0;
        
        col_type = sqlite3_column_type(stmt, col_index);
        int_val = sqlite3_column_int64(stmt, col_index);
        double_val = sqlite3_column_double(stmt, col_index);
        
        if (col_index < col_count) {
            value = sqlite3_column_value(stmt, col_index);
            if (value) {
                encoding = sqlite3_value_encoding(value);
            }
        }
        
        bytes = sqlite3_column_bytes(stmt, col_index);
        blob = sqlite3_column_blob(stmt, col_index);
        
        bytes16 = sqlite3_column_bytes16(stmt, col_index);
        blob2 = sqlite3_column_blob(stmt, col_index);
        
        bytes16_2 = sqlite3_column_bytes16(stmt, col_index);
        blob3 = sqlite3_column_blob(stmt, col_index);
        
        if (col_index < col_count) {
            value2 = sqlite3_column_value(stmt, col_index);
            if (value2) {
                encoding2 = sqlite3_value_encoding(value2);
            }
        }
        
        bytes2 = sqlite3_column_bytes(stmt, col_index);
        blob4 = sqlite3_column_blob(stmt, col_index);
    }
    
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    return 0;
}