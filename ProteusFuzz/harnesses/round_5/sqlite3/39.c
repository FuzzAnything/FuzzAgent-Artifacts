// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to implement a dummy aggregate step function.
 * This function mimics the behavior expected by sqlite3_aggregate_context.
 * It invokes the target APIs in the required order:
 * 1. sqlite3_aggregate_context
 * 2. sqlite3_value_bytes
 * 3. sqlite3_value_text
 */
static void dummy_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    int i;
    void *pCtx;
    int bytes;
    const unsigned char *text;

    /* 
     * Step 1: Invoke sqlite3_aggregate_context.
     * We attempt to allocate a small buffer. The size is derived from the input data
     * if possible, otherwise a default small size is used to ensure valid behavior.
     */
    pCtx = sqlite3_aggregate_context(ctx, 16);

    /* 
     * Step 2: Invoke sqlite3_value_bytes on the first argument if available.
     * This function returns the byte length of the value.
     */
    if (argc > 0 && argv[0] != NULL) {
        bytes = sqlite3_value_bytes(argv[0]);
        /* 
         * We don't use the return value 'bytes' for logic flow here to avoid 
         * complex branching that might reduce coverage, but the call itself 
         * exercises the code path.
         */
        (void)bytes;
    }

    /* 
     * Step 3: Invoke sqlite3_value_text on the first argument if available.
     * This retrieves the text representation.
     */
    if (argc > 0 && argv[0] != NULL) {
        text = sqlite3_value_text(argv[0]);
        /* 
         * The returned pointer is valid only within this callback.
         * We do not copy it, just ensure the call is made.
         */
        (void)text;
    }

    /* 
     * Iterate through all arguments to maximize state exploration.
     * This ensures we test the APIs with different argument types if provided.
     */
    for (i = 0; i < argc; i++) {
        if (argv[i] != NULL) {
            (void)sqlite3_value_bytes(argv[i]);
            (void)sqlite3_value_text(argv[i]);
        }
    }

    /* 
     * Set a dummy result to prevent the function from returning NULL 
     * which might cause issues depending on the SQL query structure.
     */
    sqlite3_result_int(ctx, 0);
}

/*
 * Static helper function to implement a dummy aggregate final function.
 * Required by sqlite3_create_function_v2.
 */
static void dummy_aggregate_final(sqlite3_context *ctx) {
    /* 
     * In xFinal, it is customary to call sqlite3_aggregate_context with 0 
     * to avoid re-allocation, though not strictly required if we just want to 
     * trigger the API call once. We call it to ensure coverage of the nBytes=0 path.
     */
    (void)sqlite3_aggregate_context(ctx, 0);
    
    /* Return a dummy integer result */
    sqlite3_result_int(ctx, 0);
}

/*
 * Fuzzing entry point.
 * Data: The fuzzing input buffer.
 * Size: The size of the input buffer.
 */
int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    const char *sql = "SELECT dummy_agg(*) FROM test_table;";
    const char *create_sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data TEXT);";
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (?)";
    
    /* 
     * 1. Prepare the environment.
     * Create an in-memory database to avoid file I/O issues.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Create the test table.
     */
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Insert some dummy data to ensure the aggregate function has rows to process.
     * We use the fuzzing input as the data to insert, ensuring diverse states.
     */
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* 
     * Register the custom aggregate function.
     * We use the dummy_aggregate_step and dummy_aggregate_final functions defined above.
     * Arguments for sqlite3_create_function_v2:
     * 1. db
     * 2. zFunctionName
     * 3. nArg
     * 4. eTextRep
     * 5. pApp (user data)
     * 6. xFunc (scalar function, NULL for aggregate)
     * 7. xStep
     * 8. xFinal
     * 9. xDestroy
     */
    rc = sqlite3_create_function_v2(
        db, 
        "dummy_agg", 
        -1, /* Any number of arguments */
        SQLITE_UTF8, 
        NULL, /* pUserData */
        NULL, /* xFunc - NULL for aggregate */
        dummy_aggregate_step, /* xStep */
        dummy_aggregate_final, /* xFinal */
        NULL  /* xDestroy */
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Execute the query that triggers the aggregate function.
     * This will call xStep (dummy_aggregate_step) for each row, 
     * which in turn calls the target APIs.
     */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* 
             * The aggregate logic runs inside the step.
             * We just iterate to ensure completion.
             */
        }
        sqlite3_finalize(stmt);
    }

    /* 
     * 2. Cleanup.
     * Close the database connection.
     */
    sqlite3_close(db);

    return 0;
}