// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:92807:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
 * Fuzz driver for sqlite3 context result functions.
 * Target APIs:
 * - sqlite3_result_null
 * - sqlite3_user_data
 * - sqlite3_result_error_toobig
 * - sqlite3_result_value
 * - sqlite3_result_error_nomem
 * - sqlite3_result_subtype
 */

static void dummy_user_data_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* Fuzzing logic inside the callback */
    if (ctx == NULL) {
        return;
    }

    /* 1. Test sqlite3_result_null */
    sqlite3_result_null(ctx);

    /* 2. Test sqlite3_user_data */
    void *userData = sqlite3_user_data(ctx);
    (void)userData; /* Suppress unused warning */

    /* 3. Test sqlite3_result_error_toobig */
    sqlite3_result_error_toobig(ctx);

    /* 4. Test sqlite3_result_value */
    if (argc > 0 && argv[0] != NULL) {
        sqlite3_result_value(ctx, argv[0]);
    }

    /* 5. Test sqlite3_result_error_nomem */
    sqlite3_result_error_nomem(ctx);

    /* 6. Test sqlite3_result_subtype */
    /* Use a fixed subtype for now, or derive from input if needed */
    sqlite3_result_subtype(ctx, 0x42);
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table to execute queries against */
    const char *create_table_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data TEXT);";
    rc = sqlite3_exec(db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register a custom function that exercises the target APIs */
    /* The 5th parameter is user data, which sqlite3_user_data will return */
    void *user_data_ptr = (void *)(size_t)Size; /* Use input size as dummy user data */
    rc = sqlite3_create_function(db, "fuzz_test_func", 1, SQLITE_UTF8, user_data_ptr, dummy_user_data_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement that uses the custom function */
    const char *select_sql = "SELECT fuzz_test_func(?) FROM test_table;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind input data as a blob or text to the statement */
    if (Size > 0) {
        /* Bind the fuzz input as a blob */
        rc = sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    } else {
        /* Bind NULL if input is empty */
        rc = sqlite3_bind_null(stmt, 1);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return 0;
        }
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /* The custom function may set errors, so rc might not be SQLITE_ROW or SQLITE_DONE */
    /* We ignore the result as the goal is to exercise the function internals */

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}