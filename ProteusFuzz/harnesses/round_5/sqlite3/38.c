// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value_bytes -> sqlite3_value_blob -> sqlite3_result_int
 *
 * This driver simulates the execution of a user-defined SQL function.
 * It creates a database connection, a statement, binds fuzzed data,
 * and then invokes the target API sequence on the bound value.
 */

/*
 * The user-defined function callback must return void according to sqlite3_create_function.
 * It receives the context, argument count, and argument values.
 */
static void fuzz_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /*
     * We expect at least one argument to test the value accessors.
     * If argc is 0, we return early to avoid null pointer dereference.
     */
    if (argc < 1) {
        sqlite3_result_int(ctx, 0);
        return;
    }

    sqlite3_value *val = argv[0];

    /*
     * Step 1: Invoke sqlite3_value_bytes
     * Retrieves the byte length of the value.
     */
    int bytes = sqlite3_value_bytes(val);

    /*
     * Step 2: Invoke sqlite3_value_blob
     * Retrieves the BLOB data.
     * Note: The pointer returned is valid only until the next value access.
     * We do not use the pointer for complex logic to avoid lifetime issues,
     * but the call itself exercises the code path.
     */
    const void *blob = sqlite3_value_blob(val);
    (void)blob; /* Suppress unused variable warning */

    /*
     * Step 3: Invoke sqlite3_result_int
     * Sets the result of the function.
     * We use the byte count (or a derived value) as the integer result.
     * Ensure the value fits in int.
     */
    int result_val = 0;
    if (bytes > 0) {
        result_val = bytes;
    }
    sqlite3_result_int(ctx, result_val);
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /*
     * Prepare the environment:
     * 1. Open an in-memory database.
     * 2. Create a user-defined function that exercises the target APIs.
     * 3. Prepare a statement that uses this function.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Register the custom function.
     * We name it "fuzz_test" and it takes a variable number of arguments.
     * The function pointer type must match void (*)(sqlite3_context*, int, sqlite3_value**)
     */
    rc = sqlite3_create_function(db, "fuzz_test", -1, SQLITE_UTF8, NULL, fuzz_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Prepare a SQL statement that calls our function with a bound parameter.
     * The fuzzed data will be bound to this parameter.
     */
    const char *sql = "SELECT fuzz_test(?1);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Bind the fuzzed input data to the first parameter.
     * We treat the input as a BLOB to ensure sqlite3_value_blob is exercised.
     */
    if (Size > 0) {
        sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_TRANSIENT);
    } else {
        /* Bind NULL if input is empty to test NULL handling */
        sqlite3_bind_null(stmt, 1);
    }

    /*
     * Execute the statement to trigger the user-defined function.
     * The function will internally call:
     * 1. sqlite3_value_bytes
     * 2. sqlite3_value_blob
     * 3. sqlite3_result_int
     */
    rc = sqlite3_step(stmt);
    
    /*
     * We don't strictly need to fetch the result, as the side effects
     * (calling the APIs) have occurred inside the step execution.
     * However, checking rc helps catch errors during execution.
     */
    (void)rc;

    /*
     * Cleanup:
     * 1. Finalize the statement.
     * 2. Close the database connection.
     */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}