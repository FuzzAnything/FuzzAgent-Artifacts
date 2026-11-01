// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_user_data at sqlite3.c:93222:18 in sqlite3.h
// sqlite3_result_double at sqlite3.c:92742:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:92742:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:92742:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_result_error_toobig at sqlite3.c:92954:17 in sqlite3.h
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
 * Fuzz driver for sqlite3 result functions and user_data.
 *
 * Since sqlite3_context and sqlite3_value are opaque and managed internally by SQLite,
 * we cannot directly instantiate them with valid internal state without a full database
 * connection and a prepared statement.
 *
 * Strategy:
 * 1. Create a database connection.
 * 2. Define a custom SQL function that accepts fuzzed data.
 * 3. Inside the custom function, use the fuzzed input to decide which result API to call.
 * 4. Execute a query that invokes this function.
 * 5. Handle errors and cleanup.
 */

/*
 * Global variable to store user data for the custom function.
 * This is passed to the function via sqlite3_create_function.
 */
static void *g_user_data_ptr = NULL;

/*
 * Custom SQL function implementation.
 * This function is called by SQLite when the query executes.
 * It uses the fuzzed input (via argc and argv) to trigger different code paths.
 */
static void fuzz_function_impl(sqlite3_context *pCtx, int argc, sqlite3_value **argv) {
    if (pCtx == NULL) {
        /* API armor check: if context is NULL, do nothing safely. */
        return;
    }

    /*
     * We need a way to pass the fuzzed data into this function.
     * Since we can't easily pass the raw fuzz buffer directly to the callback
     * without a wrapper, we will use the arguments of the SQL function.
     * However, the fuzz driver needs to construct the SQL call.
     *
     * Alternative approach for this specific task:
     * The fuzz input is available in LLVMFuzzerTestOneInput.
     * We can store the fuzz input in a static buffer or use the arguments.
     * Let's use a static buffer to store the current fuzz input for the callback to read.
     */
    
    /* 
     * To make this work, we need to pass the fuzz data into the SQL function.
     * Since we can't pass the whole buffer easily as a single argument without
     * constructing a BLOB, let's use the first argument if available, or
     * rely on a global state set by the fuzzer before execution.
     * 
     * Better approach: The fuzzer will set a global variable with the fuzz data
     * and the function will read from it. But we need to ensure thread safety
     * (though fuzzing is usually single-threaded).
     * 
     * Let's use the arguments passed to the function. We will construct the SQL
     * call in the fuzzer to pass the data as a BLOB or TEXT.
     * However, the target APIs don't take the data directly, they take the context.
     * The decision of which API to call can be driven by the fuzz data.
     *
     * Let's assume the first argument contains a "command" byte.
     */
    
    if (argc > 0) {
        const unsigned char *arg_data = sqlite3_value_blob(argv[0]);
        int arg_len = sqlite3_value_bytes(argv[0]);
        
        if (arg_data != NULL && arg_len > 0) {
            unsigned char cmd = arg_data[0];
            
            switch (cmd) {
                case 0:
                    /* Test sqlite3_result_error_nomem */
                    sqlite3_result_error_nomem(pCtx);
                    break;
                case 1:
                    /* Test sqlite3_result_null */
                    sqlite3_result_null(pCtx);
                    break;
                case 2:
                    /* Test sqlite3_result_value */
                    /* We need a valid sqlite3_value to copy. Use the first argument. */
                    sqlite3_result_value(pCtx, argv[0]);
                    break;
                case 3:
                    /* Test sqlite3_result_error_toobig */
                    sqlite3_result_error_toobig(pCtx);
                    break;
                case 4:
                    /* Test sqlite3_user_data */
                    {
                        void *user_ptr = sqlite3_user_data(pCtx);
                        /* Just use the pointer to ensure the call happens. 
                         * We don't need to do anything with it, just ensure it doesn't crash. */
                        (void)user_ptr;
                        /* Return the length of the fuzz data as a double to keep the function returning something */
                        sqlite3_result_double(pCtx, (double)arg_len);
                    }
                    break;
                case 5:
                    /* Test sqlite3_result_double */
                    {
                        double val = 0.0;
                        if (arg_len >= 8) {
                            memcpy(&val, arg_data, 8);
                        } else if (arg_len > 0) {
                            val = (double)arg_data[0];
                        }
                        sqlite3_result_double(pCtx, val);
                    }
                    break;
                default:
                    /* Fallback: return the length as a double */
                    sqlite3_result_double(pCtx, (double)arg_len);
                    break;
            }
        } else {
            /* No data or NULL argument, just return null */
            sqlite3_result_null(pCtx);
        }
    } else {
        /* No arguments, return null */
        sqlite3_result_null(pCtx);
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql;
    
    /* 
     * Create a dummy file for the database if needed, but ":memory:" is safer for fuzzing
     * to avoid disk I/O issues and cleanup.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Register the custom function.
     * We pass a unique pointer as user_data to test sqlite3_user_data.
     * We use the fuzz data pointer as user_data to ensure it's unique per run.
     */
    g_user_data_ptr = (void*)(uintptr_t)Data;
    rc = sqlite3_create_function(db, "fuzz_func", -1, SQLITE_UTF8, g_user_data_ptr, fuzz_function_impl, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Prepare the SQL statement.
     * We call our function with a BLOB argument containing the fuzz data.
     * We limit the size to avoid huge allocations in the SQL string.
     * If Size is too large, we truncate or just pass a small chunk.
     */
    size_t max_blob_size = 1024; /* Limit blob size to prevent huge SQL strings */
    if (Size > max_blob_size) {
        Size = max_blob_size;
    }

    /* 
     * Construct SQL: SELECT fuzz_func(X'...')
     * We need to hex-encode the data for the SQL BLOB literal.
     * This is expensive but necessary for correctness.
     * Alternatively, we can use a parameterized query.
     */
    
    /* Use parameterized query for efficiency and safety */
    sql = "SELECT fuzz_func(?1)";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind the fuzz data as a BLOB */
    rc = sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /* We don't care about the result, just that it doesn't crash */
    (void)rc;

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}