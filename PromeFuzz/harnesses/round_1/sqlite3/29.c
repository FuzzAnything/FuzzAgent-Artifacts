// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:79654:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
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

static void create_dummy_db(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return;
    
    char *err_msg = NULL;
    const char *create_table = "CREATE TABLE IF NOT EXISTS test (id INTEGER, value INTEGER);";
    rc = sqlite3_exec(*db, create_table, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
    }
    
    const char *insert_data = "INSERT INTO test VALUES (1, 100), (2, 200), (3, 300);";
    rc = sqlite3_exec(*db, insert_data, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK && err_msg) {
        sqlite3_free(err_msg);
    }
}

static sqlite3_stmt* prepare_test_stmt(sqlite3 *db, const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    
    const char *default_sql = "SELECT id, value FROM test WHERE id > ?;";
    sqlite3_stmt *stmt = NULL;
    
    if (size > 0) {
        char *sql = malloc(size + 1);
        if (!sql) return NULL;
        memcpy(sql, data, size);
        sql[size] = '\0';
        
        int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        free(sql);
        
        if (rc == SQLITE_OK) return stmt;
    }
    
    if (sqlite3_prepare_v2(db, default_sql, -1, &stmt, NULL) == SQLITE_OK) {
        if (size > 0) {
            int param = (int)(data[0] % 3);
            sqlite3_bind_int(stmt, 1, param);
        }
        return stmt;
    }
    
    return NULL;
}

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    
    create_dummy_db(&db);
    if (!db) return 0;
    
    stmt1 = prepare_test_stmt(db, Data, Size);
    if (!stmt1) {
        sqlite3_close(db);
        return 0;
    }
    
    const char *sql_text1 = sqlite3_sql(stmt1);
    (void)sql_text1;
    
    int step_result = sqlite3_step(stmt1);
    if (step_result == SQLITE_ROW) {
        const char *sql_text2 = sqlite3_sql(stmt1);
        (void)sql_text2;
        
        int col1 = sqlite3_column_int(stmt1, 0);
        (void)col1;
        int col2 = sqlite3_column_int(stmt1, 1);
        (void)col2;
    }
    
    sqlite3_finalize(stmt1);
    sqlite3_db_release_memory(db);
    
    stmt2 = prepare_test_stmt(db, Data, (Size > 1) ? Size - 1 : Size);
    if (stmt2) {
        const char *sql_text3 = sqlite3_sql(stmt2);
        (void)sql_text3;
        
        sqlite3_step(stmt2);
        
        const char *sql_text4 = sqlite3_sql(stmt2);
        (void)sql_text4;
        
        sqlite3_finalize(stmt2);
    }
    
    sqlite3_close(db);
    return 0;
}