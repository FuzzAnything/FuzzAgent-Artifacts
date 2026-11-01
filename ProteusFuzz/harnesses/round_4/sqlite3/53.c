// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_aggregate_context, sqlite3_value_bytes, and sqlite3_value_text.
 * 
 * Since these functions are designed to be called within the context of an SQL aggregate
 * function callback (xStep/xFinal), we must create a custom aggregate function and
 * invoke it via SQL execution to generate the necessary sqlite3_context and sqlite3_value
 * objects.
 */

/* Global variable to store the fuzz input for the custom function */
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

/*
 * Custom aggregate function step callback.
 * This is where we will invoke the target API functions.
 */
static void fuzz_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* 
     * Target 1: sqlite3_aggregate_context
     * Allocate context memory. We use a size derived from the fuzz input if available,
     * otherwise a small fixed size. We ensure the size is non-negative.
     */
    int nBytes = 0;
    if (g_fuzz_data && g_fuzz_size > 0) {
        /* Use first byte as size, capped to avoid huge allocations */
        nBytes = g_fuzz_data[0] % 1024;
    } else {
        nBytes = 64;
    }

    void *pCtx = sqlite3_aggregate_context(ctx, nBytes);
    if (pCtx) {
        /* Initialize the context memory with fuzz data if possible */
        if (g_fuzz_data && g_fuzz_size > 0) {
            size_t copy_len = (size_t)nBytes;
            if (copy_len > g_fuzz_size) copy_len = g_fuzz_size;
            memcpy(pCtx, g_fuzz_data, copy_len);
        }
    }

    /* 
     * Target 2 & 3: sqlite3_value_bytes and sqlite3_value_text
     * We need an sqlite3_value to test these. The 'argv' array contains the arguments
     * passed to the aggregate function. We check if there are arguments.
     */
    if (argc > 0 && argv[0]) {
        /* Target 2: sqlite3_value_bytes */
        int bytes = sqlite3_value_bytes(argv[0]);
        (void)bytes; /* Use the result to prevent unused variable warnings */

        /* Target 3: sqlite3_value_text */
        const unsigned char *text = sqlite3_value_text(argv[0]);
        if (text) {
            /* Do something with text to ensure it's used, e.g., check length */
            size_t len = strlen((const char *)text);
            (void)len;
        }
    }
}

/*
 * Custom aggregate function final callback.
 */
static void fuzz_agg_final(sqlite3_context *ctx) {
    if (!ctx) return;

    /* 
     * Call sqlite3_aggregate_context again with 0 bytes as per documentation
     * for xFinal to avoid re-allocation.
     */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    (void)pCtx; /* Result is not strictly needed here, but calling the API is the goal */
}

/*
 * Main fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    /* Store input for the callback */
    g_fuzz_data = Data;
    g_fuzz_size = Size;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom aggregate function */
    /* 
     * sqlite3_create_function_v2 signature:
     * int sqlite3_create_function_v2(
     *   sqlite3 *db,
     *   const char *zFunctionName,
     *   int nArg,
     *   int eTextRep,
     *   void *pApp,
     *   void (*xFunc)(...),      <-- NULL for aggregate
     *   void (*xStep)(...),      <-- fuzz_agg_step
     *   void (*xFinal)(...),     <-- fuzz_agg_final
     *   void(*xDestroy)(void*)   <-- NULL
     * );
     */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_agg",
        -1, /* Any number of arguments */
        SQLITE_UTF8,
        NULL, /* No user data */
        NULL, /* xFunc is NULL for aggregates */
        fuzz_agg_step,
        fuzz_agg_final,
        NULL  /* xDestroy */
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Construct an SQL query to invoke the aggregate function.
     * We use the fuzz data as a literal string if possible, or a simple value.
     * To ensure we hit the code paths, we construct a query that calls fuzz_agg.
     */
    char sql[256];
    
    /* Create a simple table to aggregate over */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test_table (val TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert some data to ensure xStep is called */
    /* We use a safe string insertion to avoid SQL injection issues in the test setup */
    sqlite3_stmt *stmt = NULL;
    const char *insert_sql = "INSERT INTO test_table VALUES ('fuzz_data');";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* 
     * Execute the aggregate function.
     * We pass a string argument to ensure argv[0] is valid for sqlite3_value_bytes/text.
     */
    snprintf(sql, sizeof(sql), "SELECT fuzz_agg('test_value') FROM test_table;");
    
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    (void)rc; /* We don't care about the result, just the execution path */

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}