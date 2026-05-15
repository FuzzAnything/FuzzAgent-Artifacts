// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_type at sqlite3.c:79685:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_column_name at sqlite3.c:79798:24 in sqlite3.h
// sqlite3_column_database_name at sqlite3.c:79837:24 in sqlite3.h
// sqlite3_column_decltype at sqlite3.c:79821:24 in sqlite3.h
// sqlite3_column_origin_name at sqlite3.c:79865:24 in sqlite3.h
// sqlite3_column_table_name at sqlite3.c:79851:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void setup_database(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc;
    static const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, name TEXT, value REAL);"
        "CREATE TABLE IF NOT EXISTS other_table (id INTEGER, data BLOB);"
        "INSERT OR IGNORE INTO test_table VALUES (1, 'test1', 3.14);"
        "INSERT OR IGNORE INTO test_table VALUES (2, 'test2', 2.71);"
        "INSERT OR IGNORE INTO other_table VALUES (1, x'010203');";
    
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    rc = sqlite3_exec(*db, create_table, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        return;
    }
    
    const char *select_sql = "SELECT test_table.id, test_table.name, other_table.data FROM test_table JOIN other_table ON test_table.id = other_table.id;";
    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        *db = NULL;
        *stmt = NULL;
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    setup_database(&db, &stmt);
    if (!db || !stmt) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    if (Size > 0) {
        int col_count = sqlite3_column_count(stmt);
        if (col_count > 0) {
            uint8_t col_index = Data[0] % col_count;
            
            const char *col_name = sqlite3_column_name(stmt, col_index);
            const char *db_name = sqlite3_column_database_name(stmt, col_index);
            const char *decl_type = sqlite3_column_decltype(stmt, col_index);
            const char *origin_name = sqlite3_column_origin_name(stmt, col_index);
            const char *table_name = sqlite3_column_table_name(stmt, col_index);
            
            (void)col_name;
            (void)db_name;
            (void)decl_type;
            (void)origin_name;
            (void)table_name;
            
            if (Size > 1) {
                int step_rc = sqlite3_step(stmt);
                if (step_rc == SQLITE_ROW) {
                    int col_type = sqlite3_column_type(stmt, col_index);
                    (void)col_type;
                    
                    const char *col_name2 = sqlite3_column_name(stmt, col_index);
                    const char *db_name2 = sqlite3_column_database_name(stmt, col_index);
                    const char *decl_type2 = sqlite3_column_decltype(stmt, col_index);
                    const char *origin_name2 = sqlite3_column_origin_name(stmt, col_index);
                    const char *table_name2 = sqlite3_column_table_name(stmt, col_index);
                    
                    (void)col_name2;
                    (void)db_name2;
                    (void)decl_type2;
                    (void)origin_name2;
                    (void)table_name2;
                }
                sqlite3_reset(stmt);
            }
            
            if (Size > 2) {
                for (int i = 0; i < col_count; i++) {
                    if (Data[1] & (1 << (i % 8))) {
                        const char *name = sqlite3_column_name(stmt, i);
                        const char *db_n = sqlite3_column_database_name(stmt, i);
                        const char *decl = sqlite3_column_decltype(stmt, i);
                        const char *origin = sqlite3_column_origin_name(stmt, i);
                        const char *table = sqlite3_column_table_name(stmt, i);
                        
                        (void)name;
                        (void)db_n;
                        (void)decl;
                        (void)origin;
                        (void)table;
                    }
                }
            }
        }
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}