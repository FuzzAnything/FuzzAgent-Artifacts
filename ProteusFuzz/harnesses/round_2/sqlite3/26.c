// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_stmt_isexplain at sqlite3.c:94274:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:92399:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target sequence:
 * sqlite3_column_bytes -> sqlite3_column_blob -> sqlite3_column_bytes -> sqlite3_column_blob
 * -> sqlite3_stmt_isexplain -> sqlite3_reset -> sqlite3_step -> sqlite3_errmsg -> sqlite3_errmsg -> sqlite3_finalize
 */

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = NULL;
    char *sql_buf = NULL;
    int rc;

    /*
     * Prepare environment:
     * 1. Create an in-memory database.
     * 2. Construct a SQL statement from the fuzz input if possible, otherwise use a safe default.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Ensure the input is treated as a C string for SQL preparation.
     * We create a buffer and copy the input, ensuring null termination.
     * If Size is 0, use a safe default query.
     */
    if (Size == 0) {
        sql = "SELECT 1;";
    } else {
        /*
         * Use a temporary buffer to hold the SQL string.
         * We limit the size to prevent excessive memory usage, though fuzzer usually controls this.
         * We copy the data and ensure it's null-terminated.
         */
        sql_buf = (char *)malloc(Size + 1);
        if (!sql_buf) {
            sqlite3_close(db);
            return 0;
        }
        memcpy(sql_buf, Data, Size);
        sql_buf[Size] = '\0';
        sql = sql_buf;
    }

    /*
     * Prepare the statement.
     * We use sqlite3_prepare_v2 to get better error reporting.
     */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    /*
     * If preparation fails, stmt is NULL.
     * We still need to invoke the functions to test NULL handling if the API allows it,
     * or to trigger crashes if it doesn't (which is a bug).
     */

    if (stmt != NULL) {
        /*
         * Try to execute the statement to get a row.
         * This is required before calling sqlite3_column_* functions.
         */
        rc = sqlite3_step(stmt);

        /*
         * Now invoke the target sequence.
         * We use column index 0. If the query has no columns, these functions might return NULL or 0.
         */

        /* 1. sqlite3_column_bytes */
        int bytes1 = sqlite3_column_bytes(stmt, 0);

        /* 2. sqlite3_column_blob */
        const void *blob1 = sqlite3_column_blob(stmt, 0);

        /* 3. sqlite3_column_bytes (again) */
        int bytes2 = sqlite3_column_bytes(stmt, 0);

        /* 4. sqlite3_column_blob (again) */
        const void *blob2 = sqlite3_column_blob(stmt, 0);

        /* 5. sqlite3_stmt_isexplain */
        int is_explain = sqlite3_stmt_isexplain(stmt);

        /* 6. sqlite3_reset */
        rc = sqlite3_reset(stmt);

        /* 7. sqlite3_step */
        rc = sqlite3_step(stmt);

        /* 8. sqlite3_errmsg */
        const char *err1 = sqlite3_errmsg(db);

        /* 9. sqlite3_errmsg (again) */
        const char *err2 = sqlite3_errmsg(db);

        /* 10. sqlite3_finalize */
        rc = sqlite3_finalize(stmt);
    } else {
        /*
         * If statement preparation failed, stmt is NULL.
         * We invoke the functions to test NULL handling.
         */

        /* 5. sqlite3_stmt_isexplain (handles NULL safely according to docs) */
        int is_explain = sqlite3_stmt_isexplain(NULL);

        /* 6. sqlite3_reset (handles NULL safely according to docs) */
        rc = sqlite3_reset(NULL);

        /* 7. sqlite3_step (undefined on NULL, likely crash - let it happen for fuzzing) */
        rc = sqlite3_step(NULL);

        /* 8 & 9. sqlite3_errmsg */
        const char *err1 = sqlite3_errmsg(db);
        const char *err2 = sqlite3_errmsg(db);

        /* 10. sqlite3_finalize (handles NULL safely) */
        rc = sqlite3_finalize(NULL);
    }

    /*
     * Cleanup
     * Close the database connection.
     */
    sqlite3_close(db);

    /*
     * Free the SQL buffer if we allocated one.
     */
    if (sql_buf != NULL) {
        free(sql_buf);
    }

    return 0;
}