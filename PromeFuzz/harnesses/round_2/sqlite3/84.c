// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void setup_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "value REAL, "
        "data BLOB, "
        "flag INTEGER);";
    
    const char *insert_sql = 
        "INSERT INTO test_table (name, value, data, flag) "
        "VALUES ('test', 3.14, x'01020304', 1);";
    
    const char *select_sql = 
        "SELECT id, name, value, data, flag, id+1, name||'suffix' FROM test_table;";
    
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    rc = sqlite3_exec(*db, create_table_sql, 0, 0, 0);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_exec(*db, insert_sql, 0, 0, 0);
    if (rc != SQLITE_OK) goto cleanup;
    
    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, 0);
    
cleanup:
    if (rc != SQLITE_OK) {
        sqlite3_finalize(*stmt);
        *stmt = NULL;
    }
}

static void fuzz_column_functions(sqlite3_stmt *stmt, const uint8_t *data, size_t size) {
    if (!stmt) return;
    
    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW && rc != SQLITE_DONE) return;
    
    int col_count = sqlite3_column_count(stmt);
    if (col_count <= 0) return;
    
    for (int i = 0; i < col_count; i++) {
        const char *table_name = sqlite3_column_table_name(stmt, i);
        int col_type = sqlite3_column_type(stmt, i);
        const char *col_name = sqlite3_column_name(stmt, i);
        const char *origin_name = sqlite3_column_origin_name(stmt, i);
        const char *decl_type = sqlite3_column_decltype(stmt, i);
        const char *db_name = sqlite3_column_database_name(stmt, i);
        
        if (size > 0) {
            int idx = data[0] % col_count;
            sqlite3_column_table_name(stmt, idx);
            sqlite3_column_type(stmt, idx);
            sqlite3_column_name(stmt, idx);
            sqlite3_column_origin_name(stmt, idx);
            sqlite3_column_decltype(stmt, idx);
            sqlite3_column_database_name(stmt, idx);
        }
    }
    
    sqlite3_reset(stmt);
}

int LLVMFuzzerTestOneInput_84(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    setup_database(&db, &stmt);
    fuzz_column_functions(stmt, data, size);
    
    if (stmt) sqlite3_finalize(stmt);
    if (db) sqlite3_close(db);
    
    return 0;
}