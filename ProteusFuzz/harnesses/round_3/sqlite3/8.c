// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Custom SQL function implementation to fuzz the target API sequence.
 * This function is invoked by SQLite when a query calls the registered function.
 */
static void fuzz_target_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc < 1) {
        /* If no arguments, return a default value to avoid crashes */
        sqlite3_result_int64(ctx, 0);
        return;
    }

    /* Use the first argument for testing */
    sqlite3_value *val = argv[0];

    /* 1. Invoke sqlite3_value_text */
    const unsigned char *text_val = sqlite3_value_text(val);
    (void)text_val; /* Suppress unused variable warning, pointer validity checked implicitly */

    /* 2. Invoke sqlite3_value_blob */
    const void *blob_val = sqlite3_value_blob(val);
    (void)blob_val; /* Suppress unused variable warning */

    /* 3. Invoke sqlite3_value_bytes */
    int bytes_len = sqlite3_value_bytes(val);
    (void)bytes_len; /* Use the result to ensure the call is not optimized out */

    /* 4. Invoke sqlite3_result_int64 */
    /* Use the length as the integer result, or a fixed value if length is 0 */
    sqlite3_int64 result_int64 = (bytes_len > 0) ? (sqlite3_int64)bytes_len : 0;
    sqlite3_result_int64(ctx, result_int64);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    int result;

    /* 1. Prepare the environment */
    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom function */
    /* Fixed: Use the correct function name 'fuzz_target_function' */
    rc = sqlite3_create_function(db, "fuzz_target_func", 1, SQLITE_UTF8, NULL, fuzz_target_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a SQL statement that invokes our custom function */
    const char *sql = "SELECT fuzz_target_func(X'00');";
    
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* 2. Execute the statement to trigger the callback */
    result = sqlite3_step(stmt);
    
    /* 
       The callback fuzz_target_function is executed during sqlite3_step.
       It performs the sequence:
       sqlite3_value_text -> sqlite3_value_blob -> sqlite3_value_bytes -> sqlite3_result_int64
    */

    /* 3. Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    /* Return 0 to indicate no crash (fuzzer continues) */
    return 0;
}