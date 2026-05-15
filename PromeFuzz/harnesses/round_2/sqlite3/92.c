// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_mprintf at sqlite3.c:19348:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:172047:16 in sqlite3.h
// sqlite3_changes at sqlite3.c:172031:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:174810:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void fuzz_prepare_and_execute(sqlite3 *db, const uint8_t *sql_data, size_t sql_size) {
    if (sql_size == 0) return;
    
    sqlite3_stmt *stmt = NULL;
    const char *sql = (const char*)sql_data;
    size_t sql_len = sql_size;
    
    char *sql_copy = NULL;
    if (sql_len > 0 && sql[sql_len - 1] != '\0') {
        sql_copy = malloc(sql_len + 1);
        if (!sql_copy) return;
        memcpy(sql_copy, sql, sql_len);
        sql_copy[sql_len] = '\0';
        sql = sql_copy;
    }
    
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        free(sql_copy);
        return;
    }
    
    int step_result;
    int max_steps = 10;
    for (int i = 0; i < max_steps; i++) {
        step_result = sqlite3_step(stmt);
        if (step_result != SQLITE_ROW) {
            break;
        }
    }
    
    sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
    (void)db_from_stmt;
    
    sqlite3_finalize(stmt);
    free(sql_copy);
}

static void fuzz_db_operations(sqlite3 *db, const uint8_t *data, size_t size) {
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_table, NULL, NULL, NULL);
    
    if (size > 0) {
        char *insert_sql = sqlite3_mprintf(
            "INSERT INTO fuzz_test (data) VALUES ('%.*q');",
            (int)(size > 100 ? 100 : size), data);
        if (insert_sql) {
            sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
            sqlite3_free(insert_sql);
        }
    }
    
    sqlite3_exec(db, "DELETE FROM fuzz_test WHERE id % 2 = 0;", NULL, NULL, NULL);
    
    char *update_sql = sqlite3_mprintf(
        "UPDATE fuzz_test SET data = 'updated' WHERE id % 3 = 0;");
    if (update_sql) {
        sqlite3_exec(db, update_sql, NULL, NULL, NULL);
        sqlite3_free(update_sql);
    }
}

int LLVMFuzzerTestOneInput_92(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    sqlite3_exec(db, "PRAGMA journal_mode = MEMORY;", NULL, NULL, NULL);
    sqlite3_exec(db, "PRAGMA synchronous = OFF;", NULL, NULL, NULL);
    
    fuzz_db_operations(db, Data, Size);
    
    int total_changes = sqlite3_total_changes(db);
    (void)total_changes;
    
    int changes = sqlite3_changes(db);
    (void)changes;
    
    int autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;
    
    int current = 0, highwater = 0;
    
    int op_codes[] = {
        SQLITE_DBSTATUS_LOOKASIDE_USED,
        SQLITE_DBSTATUS_LOOKASIDE_HIT,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_SIZE,
        SQLITE_DBSTATUS_LOOKASIDE_MISS_FULL,
        SQLITE_DBSTATUS_CACHE_USED,
        SQLITE_DBSTATUS_SCHEMA_USED,
        SQLITE_DBSTATUS_STMT_USED,
        SQLITE_DBSTATUS_CACHE_HIT,
        SQLITE_DBSTATUS_CACHE_MISS,
        SQLITE_DBSTATUS_CACHE_WRITE
    };
    
    for (size_t i = 0; i < sizeof(op_codes)/sizeof(op_codes[0]); i++) {
        if (i < Size) {
            int reset_flag = Data[i] % 2;
            sqlite3_db_status(db, op_codes[i], &current, &highwater, reset_flag);
        }
    }
    
    fuzz_prepare_and_execute(db, Data, Size);
    
    if (Size > 10) {
        size_t half = Size / 2;
        fuzz_prepare_and_execute(db, Data, half);
        fuzz_prepare_and_execute(db, Data + half, Size - half);
    }
    
    sqlite3_close(db);
    
    return 0;
}