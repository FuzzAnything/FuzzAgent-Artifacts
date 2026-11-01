// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
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
 * Custom SQL function implementation that exercises the target API sequence:
 * sqlite3_value_text -> sqlite3_value_blob -> sqlite3_value_bytes -> sqlite3_result_int64
 */
static void fuzz_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) {
        /* No arguments provided, return 0 */
        sqlite3_result_int64(ctx, 0);
        return;
    }

    /* Step 1: sqlite3_value_text */
    const unsigned char *text_val = sqlite3_value_text(argv[0]);
    
    /* Step 2: sqlite3_value_blob */
    /* Note: The documentation warns that calling value_blob after value_text 
     * might invalidate the text pointer, but we are calling them sequentially 
     * on the same value to test state transitions. */
    const void *blob_val = sqlite3_value_blob(argv[0]);
    
    /* Step 3: sqlite3_value_bytes */
    int bytes_len = sqlite3_value_bytes(argv[0]);
    
    /* Step 4: sqlite3_result_int64 */
    /* Use the byte length as the result. If negative (error), return 0. */
    if (bytes_len < 0) {
        sqlite3_result_int64(ctx, 0);
    } else {
        sqlite3_result_int64(ctx, (sqlite3_int64)bytes_len);
    }
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Register the custom function */
    rc = sqlite3_create_function(db, "fuzz_test_func", 1, SQLITE_UTF8, NULL, fuzz_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement that uses the custom function with the fuzzed input */
    /* We use a placeholder ?1 to bind the fuzzed data as a BLOB or TEXT */
    const char *sql = "SELECT fuzz_test_func(?1)";
    sqlite3_stmt *stmt = NULL;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzzed input data to the first parameter */
    /* If Size is 0, we bind NULL to test that path */
    if (Size > 0) {
        rc = sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
    } else {
        rc = sqlite3_bind_null(stmt, 1);
    }

    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    
    /* 
     * If the step returns SQLITE_ROW, the function executed successfully.
     * If it returns SQLITE_DONE, the query finished (no rows).
     * Any other return code indicates an error during execution.
     * We do not need to read the result for fuzzing purposes, 
     * as the side effects (calling the API sequence) have occurred.
     */

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}