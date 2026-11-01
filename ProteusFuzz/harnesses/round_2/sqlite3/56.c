// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Profile callback function */
static void dummy_profile_callback(void *pCtx, const char *zSql, sqlite3_uint64 ns) {
    (void)pCtx;
    (void)zSql;
    (void)ns;
}

/* Custom SQL function to test sqlite3_context_db_handle */
static void dummy_sql_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Access the database handle from the context */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    if (db) {
        /* Just read a value to ensure the handle is valid and used */
        sqlite3_get_autocommit(db);
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *next_stmt = NULL;
    int rc;
    int autocommit;
    const void *errmsg16;
    void *prev_profile_ctx;
    sqlite3 *stmt_db;
    sqlite3 *ctx_db;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file for the database if needed, though we use :memory: mostly */
    const char *db_name = (Size > 0 && Data[0] == 0) ? ":memory:" : "./dummy_file";

    /* Initialize database connection */
    rc = sqlite3_open(db_name, &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 1. Test sqlite3_get_autocommit on fresh connection */
    autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;

    /* 2. Prepare a statement to test sqlite3_db_handle and sqlite3_next_stmt */
    /* Use input data as SQL, but sanitize to avoid immediate crashes on invalid syntax */
    /* We construct a simple valid SQL using the first few bytes if possible, or a default */
    const char *sql;
    if (Size > 0) {
        /* Try to use input as SQL, but wrap in a safe block or use a default if it looks bad */
        /* For fuzzing, we just pass the data. sqlite3_prepare_v2 handles syntax errors gracefully. */
        sql = (const char *)Data;
    } else {
        sql = "SELECT 1";
    }

    /* Ensure null termination for safety if the fuzzer provides raw bytes without it */
    char *safe_sql = malloc(Size + 1);
    if (safe_sql) {
        memcpy(safe_sql, Data, Size);
        safe_sql[Size] = '\0';
        rc = sqlite3_prepare_v2(db, safe_sql, Size, &stmt, NULL);
        free(safe_sql);
    } else {
        rc = SQLITE_NOMEM;
    }

    if (rc == SQLITE_OK && stmt != NULL) {
        /* 2. Test sqlite3_db_handle */
        stmt_db = sqlite3_db_handle(stmt);
        if (stmt_db != db) {
            /* Should not happen, but check logic */
        }

        /* 3. Test sqlite3_next_stmt (start from NULL to get first) */
        next_stmt = sqlite3_next_stmt(db, NULL);
        while (next_stmt != NULL) {
            /* Iterate through statements */
            next_stmt = sqlite3_next_stmt(db, next_stmt);
        }

        /* 4. Test sqlite3_next_stmt starting from current stmt */
        next_stmt = sqlite3_next_stmt(db, stmt);
        (void)next_stmt;

        /* Finalize statement */
        sqlite3_finalize(stmt);
    }

    /* 5. Test sqlite3_errmsg16 */
    /* Trigger an error to ensure we get a non-NULL message sometimes */
    rc = sqlite3_exec(db, "SELECT invalid_syntax_here;", NULL, NULL, NULL);
    errmsg16 = sqlite3_errmsg16(db);
    (void)errmsg16;

    /* 6. Test sqlite3_profile */
    prev_profile_ctx = sqlite3_profile(db, dummy_profile_callback, (void *)Data);
    (void)prev_profile_ctx;

    /* Disable profiling */
    prev_profile_ctx = sqlite3_profile(db, NULL, NULL);
    (void)prev_profile_ctx;

    /* 7. Test sqlite3_context_db_handle via a custom function */
    /* Register a function */
    rc = sqlite3_create_function(db, "fuzz_test_func", 0, SQLITE_UTF8, NULL, dummy_sql_function, NULL, NULL);
    if (rc == SQLITE_OK) {
        /* Execute the function to trigger the callback where we call sqlite3_context_db_handle */
        rc = sqlite3_exec(db, "SELECT fuzz_test_func();", NULL, NULL, NULL);
        (void)rc;
    }

    /* 8. Test autocommit state changes */
    /* Begin transaction */
    rc = sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        autocommit = sqlite3_get_autocommit(db);
        /* Should be 0 */
        (void)autocommit;

        /* Commit transaction */
        rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        if (rc == SQLITE_OK) {
            autocommit = sqlite3_get_autocommit(db);
            /* Should be 1 */
            (void)autocommit;
        }
    }

    /* 9. Test with ROLLBACK */
    rc = sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        autocommit = sqlite3_get_autocommit(db);
        (void)autocommit;
        rc = sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);
        if (rc == SQLITE_OK) {
            autocommit = sqlite3_get_autocommit(db);
            (void)autocommit;
        }
    }

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}