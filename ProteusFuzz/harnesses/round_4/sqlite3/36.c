// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_result_error_nomem at sqlite3.c:92965:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
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
 * Fuzz driver for sqlite3 API functions:
 * sqlite3_value_text -> sqlite3_malloc64 -> sqlite3_result_text -> sqlite3_free
 *
 * Note: These functions are designed to be called within the context of a user-defined
 * SQL function callback. Since we cannot easily invoke a real callback without a full
 * SQL execution context, we will simulate the necessary structures and states.
 * However, direct invocation of sqlite3_value_text and sqlite3_result_text without
 * a valid context (created by sqlite3_create_function and sqlite3_exec) is undefined
 * behavior and may crash.
 *
 * To make this fuzzer safe and effective, we will:
 * 1. Initialize a database connection.
 * 2. Create a dummy function that captures the context.
 * 3. Execute a simple SQL statement to trigger the function.
 * 4. Inside the function, perform the target API calls using the fuzzed data.
 * 5. Clean up resources.
 *
 * Since the fuzzer input is just bytes, we will interpret parts of it to construct
 * the necessary arguments for our custom function.
 */

static sqlite3 *g_db = NULL;

/*
 * Custom SQL function that will be invoked during SQL execution.
 * This function will perform the target API calls.
 */
static void fuzz_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /*
     * Step 1: sqlite3_value_text
     * We need an sqlite3_value to pass. We can use one of the arguments if argc > 0.
     * If argc == 0, we cannot call this safely, so we skip or return.
     */
    if (argc > 0 && argv[0] != NULL) {
        const unsigned char *text = sqlite3_value_text(argv[0]);
        /* The pointer 'text' is valid only until this function returns.
         * We can use it to determine length or copy it, but we must not store it.
         * For fuzzing, we just call it to exercise the code path.
         */
        (void)text;
    }

    /*
     * Step 2: sqlite3_malloc64
     * We need a size. We can derive a size from the fuzz input if available,
     * but here we don't have direct access to the raw input in this callback.
     * Instead, we can use a small fixed size or derive from argc/argv if possible.
     * To make it more interesting, let's try to get a size from the first argument if it's an integer.
     */
    sqlite3_uint64 size = 100; /* Default size */
    if (argc > 0 && sqlite3_value_type(argv[0]) == SQLITE_INTEGER) {
        sqlite3_int64 val = sqlite3_value_int64(argv[0]);
        if (val > 0 && val < 1000000) { /* Reasonable limit to avoid OOM */
            size = (sqlite3_uint64)val;
        }
    }

    void *ptr = sqlite3_malloc64(size);
    if (ptr == NULL) {
        /* Allocation failed, handle gracefully */
        sqlite3_result_error_nomem(ctx);
        return;
    }

    /*
     * Step 3: sqlite3_result_text
     * We need a C-string. We can use the 'text' from step 1 if available, or a dummy string.
     * If we have 'text', we can use it. Otherwise, we create a dummy string.
     * Note: 'text' from step 1 is valid only until return, so we must use it immediately.
     */
    if (argc > 0 && argv[0] != NULL) {
        const unsigned char *text = sqlite3_value_text(argv[0]);
        if (text != NULL) {
            int len = sqlite3_value_bytes(argv[0]);
            /* Use the text from the argument as the result */
            sqlite3_result_text(ctx, (const char *)text, len, SQLITE_TRANSIENT);
        } else {
            /* If text is NULL, return a dummy string */
            sqlite3_result_text(ctx, "dummy", 5, SQLITE_TRANSIENT);
        }
    } else {
        /* No arguments, return a dummy string */
        sqlite3_result_text(ctx, "no_args", 7, SQLITE_TRANSIENT);
    }

    /*
     * Step 4: sqlite3_free
     * Free the memory allocated in step 2.
     */
    sqlite3_free(ptr);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    /*
     * Prepare the environment:
     * 1. Initialize the database connection if not already done.
     * 2. Create the custom function if not already created.
     */
    if (g_db == NULL) {
        int rc = sqlite3_open(":memory:", &g_db);
        if (rc != SQLITE_OK) {
            /* Failed to open database, stop fuzzing */
            return 0;
        }
        /* Register the custom function */
        rc = sqlite3_create_function(g_db, "fuzz_func", -1, SQLITE_UTF8, NULL, fuzz_func, NULL, NULL);
        if (rc != SQLITE_OK) {
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
    }

    /*
     * Construct a SQL statement that calls our custom function.
     * We will use the fuzzed data to construct the arguments.
     * Since the data is arbitrary bytes, we need to be careful to create valid SQL.
     * We'll create a simple SELECT statement that calls fuzz_func with the data as a blob or text.
     * However, arbitrary bytes may contain quotes or other special characters.
     * To avoid SQL injection issues in the fuzzer, we'll use a parameterized query.
     */
    const char *sql = "SELECT fuzz_func(?1, ?2)";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(g_db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        /* Prepare failed, cleanup and return */
        sqlite3_close(g_db);
        g_db = NULL;
        return 0;
    }

    /*
     * Bind the fuzzed data as arguments.
     * We'll split the data into two parts if Size is large enough, otherwise use the whole data for the first argument.
     */
    if (Size > 0) {
        /* Bind first part as a blob */
        rc = sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
        /* Bind second part: if Size is large, use the second half, else use a small dummy */
        if (Size > 100) {
            rc = sqlite3_bind_blob(stmt, 2, Data + Size/2, Size/2, SQLITE_STATIC);
        } else {
            rc = sqlite3_bind_int(stmt, 2, 100); /* Default integer */
        }
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
    } else {
        /* No data, bind dummy values */
        rc = sqlite3_bind_text(stmt, 1, "empty", -1, SQLITE_STATIC);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
        rc = sqlite3_bind_int(stmt, 2, 100);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
    }

    /*
     * Execute the statement.
     * This will trigger our custom function 'fuzz_func', which will call the target APIs.
     */
    rc = sqlite3_step(stmt);
    /* We don't care about the result of the step, as long as it doesn't crash.
     * The function might return an error, which is fine for fuzzing.
     */

    /*
     * Cleanup:
     * Finalize the statement.
     */
    sqlite3_finalize(stmt);

    /*
     * We do not close the database connection here to allow multiple iterations
     * to reuse the same connection and function registration.
     * However, if we encounter a critical error, we might want to reset.
     * For now, we assume the connection remains valid.
     */

    return 0;
}