// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_str_finish at sqlite3.c:33198:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:94174:16 in sqlite3.h
// sqlite3_sql at sqlite3.c:94381:24 in sqlite3.h
// sqlite3_str_new at sqlite3.c:33283:25 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * LLVMFuzzerTestOneInput
 * Entry point for the fuzzer.
 *
 * Logic:
 * 1. Initialize SQLite in-memory database.
 * 2. Construct a SQL statement from input data, ensuring it is a valid C-string.
 * 3. Prepare the statement.
 * 4. Invoke the target API functions in the specified order:
 *    - sqlite3_db_handle
 *    - sqlite3_column_count
 *    - sqlite3_bind_parameter_count
 *    - sqlite3_sql
 *    - sqlite3_str_new
 * 5. Cleanup resources (finalize statement, close DB, free string).
 */
int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = NULL;
    sqlite3_str *str = NULL;
    int rc;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Initialize database connection (in-memory) */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 
     * Create a safe SQL string from input.
     * We append a null terminator to ensure it's a valid C string.
     * We limit the size to avoid excessively large allocations if Size is huge.
     */
    size_t sql_len = Size;
    if (sql_len > 100000) {
        sql_len = 100000;
    }

    char *safe_sql = (char *)malloc(sql_len + 1);
    if (safe_sql == NULL) {
        sqlite3_close(db);
        return 0;
    }

    memcpy(safe_sql, Data, sql_len);
    safe_sql[sql_len] = '\0';

    /* 
     * Prepare the statement.
     * We use sqlite3_prepare_v2 to get a valid sqlite3_stmt for testing.
     * If preparation fails, stmt will be NULL, which is a valid state to test the APIs.
     */
    const char *tail;
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, &tail);
    free(safe_sql);

    /* 
     * Execute the target API sequence.
     * Order: sqlite3_db_handle -> sqlite3_column_count -> sqlite3_bind_parameter_count 
     *        -> sqlite3_sql -> sqlite3_str_new
     */

    /* 1. sqlite3_db_handle */
    sqlite3 *db_handle = sqlite3_db_handle(stmt);
    (void)db_handle; /* Suppress unused variable warning */

    /* 2. sqlite3_column_count */
    int col_count = sqlite3_column_count(stmt);
    (void)col_count;

    /* 3. sqlite3_bind_parameter_count */
    int bind_count = sqlite3_bind_parameter_count(stmt);
    (void)bind_count;

    /* 4. sqlite3_sql */
    const char *stmt_sql = sqlite3_sql(stmt);
    (void)stmt_sql;

    /* 5. sqlite3_str_new */
    /* We pass the original 'db' handle. If stmt was valid, db_handle is also valid. */
    str = sqlite3_str_new(db);
    if (str != NULL) {
        /* Ensure we finish the string to avoid leaks */
        sqlite3_str_finish(str);
    }

    /* Cleanup */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }
    if (db != NULL) {
        sqlite3_close(db);
    }

    return 0;
}