// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/*
 * Static helper to create a custom function that will be invoked by the VDBE.
 * This function implements the logic to call the target APIs in the specified order.
 */
static void fuzz_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) {
        return;
    }

    sqlite3_value *val = argv[0];

    /* 1. Invoke sqlite3_value_bytes */
    int bytes = sqlite3_value_bytes(val);
    (void)bytes; /* Use the return value to avoid unused variable warnings */

    /* 2. Invoke sqlite3_value_blob */
    const void *blob = sqlite3_value_blob(val);
    (void)blob; /* Use the return value */

    /* 3. Invoke sqlite3_result_int */
    /* We use a deterministic value derived from the byte count to ensure diverse states */
    int result_int = (bytes > 0) ? bytes : 0;
    sqlite3_result_int(ctx, result_int);
}

/*
 * The entry point for the fuzzer.
 */
int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file I/O issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a simple table to hold data */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS fuzz_data (id INTEGER PRIMARY KEY, payload BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function that contains the target API calls */
    rc = sqlite3_create_function(db, "fuzz_target_func", 1, SQLITE_UTF8, NULL, fuzz_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an INSERT statement to populate the table with fuzzed data */
    const char *insert_sql = "INSERT INTO fuzz_data (payload) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzz input as a BLOB */
    sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_STATIC);

    /* Execute the insert */
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a SELECT statement that invokes our custom function */
    const char *select_sql = "SELECT fuzz_target_func(payload) FROM fuzz_data;";
    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Execute the query to trigger the custom function and the target APIs */
    rc = sqlite3_step(stmt);
    /* We expect SQLITE_ROW or SQLITE_DONE. SQLITE_ROW means the function was called. */
    if (rc == SQLITE_ROW) {
        /* The function was called, and the target APIs were executed inside fuzz_function */
        /* We can optionally read the result, but the side effect is what we fuzz */
        (void)sqlite3_column_int(stmt, 0);
    }

    /* 2. Conduct cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}