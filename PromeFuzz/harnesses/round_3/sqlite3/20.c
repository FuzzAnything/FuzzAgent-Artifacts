// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:80251:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:79514:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:80374:24 in sqlite3.h
// sqlite3_str_new at sqlite3.c:19276:25 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:19191:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql_text = NULL;
    sqlite3_str *str_obj = NULL;
    int rc;
    
    // Open an in-memory database for fuzzing
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement using the fuzzer input as SQL
    // We need to ensure the input is null-terminated
    char *sql = malloc(Size + 1);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql, Data, Size);
    sql[Size] = '\0';
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    free(sql);
    
    // Call the target functions in the specified order
    // 1. sqlite3_db_handle
    sqlite3 *db_handle = sqlite3_db_handle(stmt);
    
    // 2. sqlite3_column_count
    int column_count = 0;
    if (stmt) {
        column_count = sqlite3_column_count(stmt);
    }
    
    // 3. sqlite3_bind_parameter_count
    int bind_param_count = 0;
    if (stmt) {
        bind_param_count = sqlite3_bind_parameter_count(stmt);
    }
    
    // 4. sqlite3_sql
    if (stmt) {
        sql_text = sqlite3_sql(stmt);
        // sql_text is managed by SQLite, no need to free
    }
    
    // 5. sqlite3_str_new
    str_obj = sqlite3_str_new(db_handle ? db_handle : db);
    if (str_obj) {
        // Clean up the string object as required
        char *str_result = sqlite3_str_finish(str_obj);
        if (str_result) {
            sqlite3_free(str_result);
        }
    }
    
    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif