// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

/*
 * Fuzz driver for sqlite3_value_text -> sqlite3_value_blob -> sqlite3_value_bytes -> sqlite3_result_int64
 *
 * Strategy:
 * 1. Create an in-memory database connection.
 * 2. Define a custom scalar function that receives fuzzed input as arguments.
 * 3. Inside the custom function, invoke the target API sequence.
 * 4. Execute a SQL statement calling the custom function with fuzzed data.
 * 5. Clean up resources.
 */

/* Custom function implementation to exercise the target APIs */
static void fuzz_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1 || ctx == NULL) {
        return;
    }

    sqlite3_value *val = argv[0];
    const unsigned char *text = NULL;
    const void *blob = NULL;
    int bytes = 0;
    sqlite3_int64 result_int64 = 0;

    /* 1. Invoke sqlite3_value_text */
    /* This attempts to retrieve the value as a UTF-8 text string. */
    text = sqlite3_value_text(val);

    /* 2. Invoke sqlite3_value_blob */
    /* This attempts to retrieve the value as a BLOB. 
     * Note: Calling this might invalidate the pointer returned by sqlite3_value_text. */
    blob = sqlite3_value_blob(val);

    /* 3. Invoke sqlite3_value_bytes */
    /* This returns the byte length. 
     * Note: This might invalidate pointers from previous text/blob calls. */
    bytes = sqlite3_value_bytes(val);

    /* 4. Invoke sqlite3_result_int64 */
    /* Set the result of the SQL function to an integer derived from the fuzz input.
     * We use 'bytes' or a hash of the input data to generate a 64-bit integer. */
    if (bytes > 0 && val != NULL) {
        /* Use the byte count as a seed for the result */
        result_int64 = (sqlite3_int64)bytes;
    } else {
        /* Fallback if bytes is 0 or val is NULL */
        result_int64 = 0;
    }
    
    sqlite3_result_int64(ctx, result_int64);
}

/* Entry point for the fuzzer */
int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, fuzz_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a SQL statement that uses the custom function */
    /* We use a placeholder ?1 to pass the fuzzed data as a BLOB or TEXT */
    const char *sql = "SELECT fuzz_func(?)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzzed input to the statement */
    /* We treat the input as a BLOB to ensure it can be converted to text/blob internally */
    sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /* We don't strictly need to check the result here as the fuzzing logic is inside the function */
    /* However, we step to trigger the function execution */

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}