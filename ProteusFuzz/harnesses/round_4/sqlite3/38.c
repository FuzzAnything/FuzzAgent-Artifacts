// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
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
 * Fuzzing strategy:
 * 1. Create a database connection.
 * 2. Define a custom SQL function that mimics the target API usage pattern.
 * 3. Prepare a statement that uses this function with fuzzed input.
 * 4. Execute the statement to trigger the function.
 * 5. Inside the function, invoke sqlite3_value_bytes, sqlite3_value_blob, and sqlite3_result_int.
 * 6. Clean up resources.
 */

static void target_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (ctx == NULL || argc < 1 || argv == NULL) {
        return;
    }

    sqlite3_value *val = argv[0];

    /* Step 1: Invoke sqlite3_value_bytes */
    int bytes_len = sqlite3_value_bytes(val);
    if (bytes_len < 0) {
        /* Handle potential error or unexpected state */
        return;
    }

    /* Step 2: Invoke sqlite3_value_blob */
    const void *blob = sqlite3_value_blob(val);
    /* The blob pointer is valid only until the next value access. 
       We do not dereference it to avoid potential crashes if it's NULL or invalid. */
    (void)blob;

    /* Step 3: Invoke sqlite3_result_int */
    /* Use a deterministic value derived from input or a constant to ensure coverage */
    int result_int = (bytes_len > 0) ? bytes_len : 0;
    sqlite3_result_int(ctx, result_int);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "fuzz_test_func", 1, SQLITE_UTF8, NULL, target_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a SQL statement that uses the custom function */
    const char *sql = "SELECT fuzz_test_func(?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Bind fuzzed input as BLOB to ensure we test blob conversion paths */
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_TRANSIENT);
    } else {
        /* Bind NULL if input is empty to test NULL handling */
        sqlite3_bind_null(stmt, 1);
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /* We don't strictly need to check rc here as the function handles internal errors */

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}