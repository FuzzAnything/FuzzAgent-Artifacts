// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
// sqlite3_str_append at sqlite3.c:33120:17 in sqlite3.h
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *strObj = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Create a safe buffer for the SQL string to ensure null-termination
    char *safe_sql = (char *)malloc(Size + 1);
    if (!safe_sql) {
        return 0;
    }
    memcpy(safe_sql, Data, Size);
    safe_sql[Size] = '\0';

    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        free(safe_sql);
        return 0;
    }

    // Attempt to prepare a statement using the fuzz input
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
    
    // If preparation failed, try a valid dummy statement to ensure the API chain executes
    if (rc != SQLITE_OK) {
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        const char *dummy_sql = "SELECT 1, 2, 3;";
        rc = sqlite3_prepare_v2(db, dummy_sql, -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
             sqlite3_close(db);
             free(safe_sql);
             return 0;
        }
    }

    // 1. sqlite3_db_handle
    sqlite3 *db_handle = sqlite3_db_handle(stmt);
    (void)db_handle; 

    // 2. sqlite3_column_count
    int col_count = sqlite3_column_count(stmt);
    (void)col_count;

    // 3. sqlite3_bind_parameter_count
    int param_count = sqlite3_bind_parameter_count(stmt);
    (void)param_count;

    // 4. sqlite3_sql
    const char *sql_text = sqlite3_sql(stmt);
    if (sql_text) {
        size_t len = strlen(sql_text);
        (void)len;
    }

    // 5. sqlite3_str_new
    sqlite3 *target_db = (db_handle ? db_handle : db);
    strObj = sqlite3_str_new(target_db);
    
    if (strObj) {
        // Append fuzz data to the string object
        sqlite3_str_append(strObj, (const char *)Data, Size);
        
        // Finish the string object to free resources
        char *result = sqlite3_str_finish(strObj);
        if (result) {
            sqlite3_free(result);
        }
    }

    // Cleanup
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    if (db) {
        sqlite3_close(db);
    }
    free(safe_sql);

    return 0;
}