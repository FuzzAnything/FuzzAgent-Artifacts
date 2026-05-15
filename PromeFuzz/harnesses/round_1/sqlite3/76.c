// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:80321:26 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
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
#include <stdio.h>

static void fuzz_prepare_and_test(sqlite3 *db, const uint8_t *sql_data, size_t sql_size) {
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    char *sql_str = NULL;
    
    if (sql_size == 0) {
        return;
    }
    
    sql_str = (char*)malloc(sql_size + 1);
    if (!sql_str) {
        return;
    }
    
    memcpy(sql_str, sql_data, sql_size);
    sql_str[sql_size] = '\0';
    
    int rc = sqlite3_prepare_v2(db, sql_str, -1, &stmt, &tail);
    
    if (rc == SQLITE_OK && stmt) {
        sqlite3 *db_handle = sqlite3_db_handle(stmt);
        
        const char *sql_text = sqlite3_sql(stmt);
        int col_count = sqlite3_column_count(stmt);
        
        int reset_rc = sqlite3_reset(stmt);
        
        sqlite3_stmt *next_stmt = sqlite3_next_stmt(db, stmt);
        
        sqlite3_finalize(stmt);
    }
    
    free(sql_str);
}

static void fuzz_iterate_statements(sqlite3 *db) {
    sqlite3_stmt *stmt = NULL;
    
    while ((stmt = sqlite3_next_stmt(db, stmt)) != NULL) {
        sqlite3 *db_handle = sqlite3_db_handle(stmt);
        const char *sql_text = sqlite3_sql(stmt);
        int col_count = sqlite3_column_count(stmt);
        int reset_rc = sqlite3_reset(stmt);
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    const char *create_table_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER, name TEXT);";
    sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    
    const char *insert_sql = "INSERT INTO test VALUES (1, 'test');";
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    
    const char *select_sql = "SELECT * FROM test;";
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_prepare_v2(db, select_sql, -1, &stmt1, NULL);
    
    const char *update_sql = "UPDATE test SET name = 'updated' WHERE id = 1;";
    sqlite3_stmt *stmt2 = NULL;
    sqlite3_prepare_v2(db, update_sql, -1, &stmt2, NULL);
    
    fuzz_prepare_and_test(db, Data, Size);
    
    fuzz_iterate_statements(db);
    
    if (stmt1) {
        sqlite3_finalize(stmt1);
    }
    
    if (stmt2) {
        sqlite3_finalize(stmt2);
    }
    
    sqlite3_close(db);
    
    return 0;
}