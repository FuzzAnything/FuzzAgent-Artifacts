// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_text16 at sqlite3.c:79679:24 in sqlite3.h
// sqlite3_column_database_name16 at sqlite3.c:79841:24 in sqlite3.h
// sqlite3_column_decltype16 at sqlite3.c:79825:24 in sqlite3.h
// sqlite3_column_table_name16 at sqlite3.c:79855:24 in sqlite3.h
// sqlite3_column_origin_name16 at sqlite3.c:79869:24 in sqlite3.h
// sqlite3_column_name16 at sqlite3.c:79802:24 in sqlite3.h
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

static void setup_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    const char *create_table_sql = 
        "CREATE TABLE test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);";
    const char *insert_sql = 
        "INSERT INTO test_table (name, value) VALUES ('test', 3.14);";
    const char *select_sql = 
        "SELECT * FROM test_table;";

    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;

    rc = sqlite3_exec(*db, create_table_sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return;
    }

    rc = sqlite3_exec(*db, insert_sql, 0, 0, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return;
    }

    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, 0);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        *stmt = NULL;
        return;
    }

    sqlite3_step(*stmt);
}

static void cleanup_database(sqlite3 *db, sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_84(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    setup_database(&db, &stmt);
    if (!db || !stmt) {
        cleanup_database(db, stmt);
        return 0;
    }

    int num_cols = sqlite3_column_count(stmt);
    if (num_cols <= 0) {
        cleanup_database(db, stmt);
        return 0;
    }

    for (int i = 0; i < num_cols; i++) {
        const void *result;
        
        result = sqlite3_column_text16(stmt, i);
        (void)result;
        
        result = sqlite3_column_database_name16(stmt, i);
        (void)result;
        
        result = sqlite3_column_decltype16(stmt, i);
        (void)result;
        
        result = sqlite3_column_table_name16(stmt, i);
        (void)result;
        
        result = sqlite3_column_origin_name16(stmt, i);
        (void)result;
        
        result = sqlite3_column_name16(stmt, i);
        (void)result;
    }

    if (Size > 0) {
        int col_index = Data[0] % num_cols;
        
        const void *result;
        result = sqlite3_column_text16(stmt, col_index);
        (void)result;
        
        result = sqlite3_column_database_name16(stmt, col_index);
        (void)result;
        
        result = sqlite3_column_decltype16(stmt, col_index);
        (void)result;
        
        result = sqlite3_column_table_name16(stmt, col_index);
        (void)result;
        
        result = sqlite3_column_origin_name16(stmt, col_index);
        (void)result;
        
        result = sqlite3_column_name16(stmt, col_index);
        (void)result;
    }

    cleanup_database(db, stmt);
    return 0;
}