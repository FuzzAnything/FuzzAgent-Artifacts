// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Static helper to ensure the database file exists and is writable.
 * Uses "./dummy_file" as specified.
 */
static int ensure_dummy_db(sqlite3 **db) {
    int rc = sqlite3_open("./dummy_file", db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    return 1;
}

/*
 * Static helper to safely finalize a statement if it is not NULL.
 */
static void safe_finalize(sqlite3_stmt *stmt) {
    if (stmt) {
        sqlite3_finalize(stmt);
    }
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *next_stmt = NULL;
    const char *tail = NULL;
    int rc;
    int autocommit;
    sqlite3 *stmt_db;
    const void *err_msg;

    /* 1. Prepare the environment */
    if (!ensure_dummy_db(&db)) {
        return 0;
    }

    /* Ensure we have at least 1 byte to avoid immediate null pointer issues */
    if (Size == 0) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Create a table to ensure the database has some schema for prepare to work on 
     * if the fuzz input is valid SQL. This helps explore more states.
     */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        /* If we can't create the table, we can still test error paths */
    }

    /* 
     * 2. Invoke sqlite3_prepare 
     * We pass the fuzz data as the SQL string. 
     */
    rc = sqlite3_prepare(db, (const char *)Data, (int)Size, &stmt, &tail);

    /* 
     * 3. Explore program states based on the result of prepare
     */
    if (rc == SQLITE_OK && stmt != NULL) {
        /* 
         * Test sqlite3_get_autocommit 
         */
        autocommit = sqlite3_get_autocommit(db);
        (void)autocommit;

        /* 
         * Test sqlite3_db_handle 
         */
        stmt_db = sqlite3_db_handle(stmt);
        if (stmt_db != db) {
            /* Logic error or unexpected state */
        }

        /* 
         * Test sqlite3_next_stmt 
         */
        next_stmt = sqlite3_next_stmt(db, NULL);
        if (next_stmt != NULL) {
            sqlite3_finalize(next_stmt);
        }
        
        next_stmt = sqlite3_next_stmt(db, stmt);
        if (next_stmt != NULL) {
            sqlite3_finalize(next_stmt);
        }

        /* 
         * Test sqlite3_errmsg16 
         */
        err_msg = sqlite3_errmsg16(db);
        (void)err_msg;
    }

    /* 
     * If prepare failed (rc != SQLITE_OK), stmt is NULL.
     * We can still test sqlite3_errmsg16 to get the error message in UTF-16.
     */
    if (rc != SQLITE_OK) {
        err_msg = sqlite3_errmsg16(db);
        (void)err_msg;
        
        /* 
         * Test sqlite3_get_autocommit on a connection that might have errors.
         */
        autocommit = sqlite3_get_autocommit(db);
        (void)autocommit;
    }

    /* 
     * Test sqlite3_next_stmt when no statements are open (if stmt was NULL or finalized).
     */
    if (stmt == NULL) {
        next_stmt = sqlite3_next_stmt(db, NULL);
        if (next_stmt != NULL) {
            sqlite3_finalize(next_stmt);
        }
    }

    /* 
     * 4. Cleanup 
     * Always finalize the statement if it was successfully created.
     */
    safe_finalize(stmt);

    /* 
     * Close the database connection.
     */
    sqlite3_close(db);

    return 0;
}