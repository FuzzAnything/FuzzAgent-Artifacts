// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_data_count at sqlite3.c:79524:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void create_dummy_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    char *err_msg = NULL;
    
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    rc = sqlite3_exec(*db, 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "CREATE TABLE another_table (col1 TEXT, col2 BLOB);"
        "CREATE VIEW test_view AS SELECT * FROM test_table;", 
        NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    
    rc = sqlite3_prepare_v2(*db, 
        "SELECT test_table.id, test_table.name, another_table.col1 FROM test_table "
        "LEFT JOIN another_table ON test_table.id = 1", 
        -1, stmt, NULL);
    if (rc != SQLITE_OK) return;
    
    sqlite3_step(*stmt);
}

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int column_index = 0;
    
    if (Size < 1) return 0;
    
    create_dummy_database(&db, &stmt);
    if (!db || !stmt) {
        if (stmt) sqlite3_finalize(stmt);
        if (db) sqlite3_close(db);
        return 0;
    }
    
    column_index = Data[0] % 10;
    
    const void *result1 = sqlite3_column_name16(stmt, column_index);
    const void *result2 = sqlite3_column_decltype16(stmt, column_index);
    const void *result3 = sqlite3_column_database_name16(stmt, column_index);
    const void *result4 = sqlite3_column_origin_name16(stmt, column_index);
    const void *result5 = sqlite3_column_table_name16(stmt, column_index);
    int data_count = sqlite3_data_count(stmt);
    
    sqlite3_reset(stmt);
    
    column_index = (Data[0] + 1) % 10;
    
    result1 = sqlite3_column_name16(stmt, column_index);
    result2 = sqlite3_column_decltype16(stmt, column_index);
    result3 = sqlite3_column_database_name16(stmt, column_index);
    result4 = sqlite3_column_origin_name16(stmt, column_index);
    result5 = sqlite3_column_table_name16(stmt, column_index);
    data_count = sqlite3_data_count(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}