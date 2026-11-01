// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static helper to create a custom function for testing sqlite3_context_db_handle */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    if (db) {
        /* Trigger some internal logic to ensure the handle is valid */
        sqlite3_get_autocommit(db);
    }
    sqlite3_result_null(ctx);
}

/* Static profile callback for testing sqlite3_profile */
static void dummy_profile(void *pArg, const char *zSql, sqlite3_uint64 ns) {
    (void)pArg;
    (void)zSql;
    (void)ns;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *next_stmt = NULL;
    int rc;

    /* Guard against empty input or extremely small sizes that might cause issues */
    if (Size < 1) {
        return 0;
    }

    /* Initialize SQLite if not already initialized (thread-safe) */
    sqlite3_initialize();

    /* Open a database connection in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a dummy table to allow statement preparation */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    /* Use input data to generate SQL statements for fuzzing */
    /* Ensure the SQL string is null-terminated */
    size_t sql_len = (Size > 256) ? 256 : Size;
    char *sql_buf = (char *)malloc(sql_len + 1);
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_buf, Data, sql_len);
    sql_buf[sql_len] = '\0';

    /* Filter out obviously invalid SQL prefixes to reduce noise, but keep variety */
    /* We try to prepare a statement. If it fails, we just move on. */
    rc = sqlite3_prepare_v2(db, sql_buf, sql_len, &stmt, NULL);

    if (rc == SQLITE_OK && stmt != NULL) {
        /* Test sqlite3_db_handle */
        sqlite3 *stmt_db = sqlite3_db_handle(stmt);
        if (stmt_db) {
            /* Verify it matches our db */
            if (stmt_db != db) {
                /* This should not happen, but good for fuzzing logic */
            }
        }

        /* Test sqlite3_next_stmt starting from NULL */
        next_stmt = sqlite3_next_stmt(db, NULL);
        while (next_stmt != NULL) {
            /* Test sqlite3_db_handle on the next statement */
            sqlite3 *ns_db = sqlite3_db_handle(next_stmt);
            (void)ns_db;

            /* Iterate to the next */
            next_stmt = sqlite3_next_stmt(db, next_stmt);
        }

        /* Test sqlite3_next_stmt starting from a specific statement */
        next_stmt = sqlite3_next_stmt(db, stmt);
        (void)next_stmt;

        /* Test sqlite3_get_autocommit */
        int auto_commit = sqlite3_get_autocommit(db);
        (void)auto_commit;

        /* Start a transaction to change autocommit state */
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        auto_commit = sqlite3_get_autocommit(db);
        (void)auto_commit;

        /* Commit to restore state */
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

        /* Test sqlite3_profile */
        void *prev_ctx = sqlite3_profile(db, dummy_profile, (void *)0x1234);
        (void)prev_ctx;

        /* Disable profiling */
        prev_ctx = sqlite3_profile(db, NULL, NULL);
        (void)prev_ctx;

        /* Test sqlite3_errmsg16 */
        const void *err_msg = sqlite3_errmsg16(db);
        (void)err_msg;

        /* Force an error to test errmsg16 */
        sqlite3_exec(db, "SELECT invalid_column FROM fuzz_test;", NULL, NULL, NULL);
        err_msg = sqlite3_errmsg16(db);
        (void)err_msg;

        /* Test sqlite3_context_db_handle by registering and calling a function */
        sqlite3_create_function(db, "fuzz_dummy", 0, SQLITE_UTF8, NULL, dummy_func, NULL, NULL);
        sqlite3_exec(db, "SELECT fuzz_dummy();", NULL, NULL, NULL);

        sqlite3_finalize(stmt);
    } else {
        /* Even if preparation fails, test the APIs with the open connection */
        /* Test sqlite3_next_stmt on a connection with no statements */
        next_stmt = sqlite3_next_stmt(db, NULL);
        (void)next_stmt;

        /* Test sqlite3_get_autocommit */
        int auto_commit = sqlite3_get_autocommit(db);
        (void)auto_commit;

        /* Test sqlite3_errmsg16 */
        const void *err_msg = sqlite3_errmsg16(db);
        (void)err_msg;
    }

    free(sql_buf);

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}