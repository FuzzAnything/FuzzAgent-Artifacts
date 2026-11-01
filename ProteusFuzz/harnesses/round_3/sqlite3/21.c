// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
#include <stdio.h>
#include <sqlite3.h>

static int run_fuzz_logic(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database connection in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table to ensure we have something to query */
    const char *init_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, init_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement using a safe query to ensure the statement is valid */
    const char *safe_sql = "SELECT * FROM test_table WHERE id = ?;";
    rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* 
       Invoke the target API functions in the specified order:
       1. sqlite3_db_handle
       2. sqlite3_column_count
       3. sqlite3_bind_parameter_count
       4. sqlite3_sql
       5. sqlite3_str_new
    */

    /* 1. sqlite3_db_handle */
    sqlite3 *db_handle = sqlite3_db_handle(stmt);
    (void)db_handle; /* Suppress unused variable warning */

    /* 2. sqlite3_column_count */
    int col_count = sqlite3_column_count(stmt);
    (void)col_count;

    /* 3. sqlite3_bind_parameter_count */
    int param_count = sqlite3_bind_parameter_count(stmt);
    (void)param_count;

    /* 4. sqlite3_sql */
    const char *sql_text = sqlite3_sql(stmt);
    (void)sql_text;

    /* 5. sqlite3_str_new */
    sqlite3_str *str = sqlite3_str_new(db);
    if (str != NULL) {
        /* Attempt to append fuzzed data to the string object to exercise logic */
        if (Size > 0) {
            sqlite3_str_append(str, (const char *)Data, Size);
        }
        /* Always finish the string object to free resources. 
           This function returns the allocated string or NULL on error/oom.
           We must free the returned string if it is not NULL to prevent leaks. */
        char *result_str = sqlite3_str_finish(str);
        if (result_str != NULL) {
            sqlite3_free(result_str);
        }
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    return run_fuzz_logic(Data, Size);
}