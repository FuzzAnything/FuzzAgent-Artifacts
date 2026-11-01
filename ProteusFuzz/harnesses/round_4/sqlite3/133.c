// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
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
#include <stdio.h>

/* Profile callback function */
static void profile_callback(void *pCtx, const char *zSql, sqlite3_uint64 nTime) {
    (void)pCtx;
    (void)zSql;
    (void)nTime;
}

/* Custom SQL function to test sqlite3_context_db_handle */
static void custom_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    if (ctx) {
        sqlite3 *db = sqlite3_context_db_handle(ctx);
        if (db) {
            /* Just access the handle to ensure the path is exercised */
            (void)sqlite3_get_autocommit(db);
        }
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_stmt *next_stmt = NULL;
    int rc;

    /* Guard against empty input to avoid immediate failures in some edge cases */
    if (Size == 0) {
        return 0;
    }

    /* Initialize a dummy file if needed, though we will use :memory: for safety */
    /* Create an in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 1. Test sqlite3_profile */
    /* Register a profile callback */
    void *prev_ctx = sqlite3_profile(db, profile_callback, (void *)0x1234);
    (void)prev_ctx; /* Use the previous context if needed */

    /* Disable profiling */
    sqlite3_profile(db, NULL, NULL);

    /* 2. Test sqlite3_get_autocommit */
    /* Check initial autocommit state (should be 1) */
    int auto_commit = sqlite3_get_autocommit(db);
    (void)auto_commit;

    /* 3. Test sqlite3_errmsg16 */
    /* Get error message (should be NULL if no error) */
    const void *err_msg = sqlite3_errmsg16(db);
    (void)err_msg;

    /* 4. Prepare a statement to test sqlite3_next_stmt and sqlite3_db_handle */
    /* Use input data as SQL, but sanitize to prevent immediate crashes on invalid SQL */
    /* We create a safe SQL string by taking a subset or using a fixed safe query if input is too short */
    char *safe_sql = NULL;
    if (Size > 0) {
        /* Allocate buffer for SQL + null terminator */
        safe_sql = (char *)malloc(Size + 1);
        if (safe_sql) {
            memcpy(safe_sql, Data, Size);
            safe_sql[Size] = '\0';
            
            /* Attempt to prepare the statement */
            rc = sqlite3_prepare_v2(db, safe_sql, -1, &stmt, NULL);
            free(safe_sql);
        }
    }

    if (rc == SQLITE_OK && stmt != NULL) {
        /* Test sqlite3_db_handle */
        sqlite3 *stmt_db = sqlite3_db_handle(stmt);
        if (stmt_db) {
            /* Verify it matches our db */
            if (stmt_db != db) {
                /* This should not happen, but fuzzing checks logic */
            }
        }

        /* Test sqlite3_next_stmt */
        /* Get the first statement */
        next_stmt = sqlite3_next_stmt(db, NULL);
        
        /* Iterate through statements */
        while (next_stmt != NULL) {
            /* Verify handle for this statement */
            sqlite3 *check_db = sqlite3_db_handle(next_stmt);
            (void)check_db;

            /* Get next */
            next_stmt = sqlite3_next_stmt(db, next_stmt);
        }

        /* Finalize the statement */
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* 5. Test sqlite3_context_db_handle */
    /* Create a custom function to exercise this API */
    rc = sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, NULL, custom_func, NULL, NULL);
    if (rc == SQLITE_OK) {
        /* Execute a query using the function to trigger the context */
        sqlite3_stmt *ctx_stmt = NULL;
        rc = sqlite3_prepare_v2(db, "SELECT fuzz_func()", -1, &ctx_stmt, NULL);
        if (rc == SQLITE_OK && ctx_stmt != NULL) {
            rc = sqlite3_step(ctx_stmt);
            (void)rc;
            sqlite3_finalize(ctx_stmt);
        }
        sqlite3_create_function(db, "fuzz_func", 0, SQLITE_UTF8, NULL, NULL, NULL, NULL);
    }

    /* Cleanup */
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}