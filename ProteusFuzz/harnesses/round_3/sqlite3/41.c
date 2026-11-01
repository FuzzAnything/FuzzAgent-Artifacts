// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_aggregate_context, sqlite3_value_bytes, sqlite3_value_text.
 *
 * The previous crash was due to an assertion failure in sqlite3_aggregate_context
 * when the context output (pOut) was not in the expected state (MEM_Null).
 * This often happens if the aggregate function logic is inconsistent or if
 * the context is misused.
 *
 * To fix this, we ensure the aggregate function correctly initializes and updates
 * its state, and we handle the case where arguments might be NULL or invalid.
 * We also ensure that we always set a result in xStep and xFinal.
 *
 * CRITICAL FIX: The assertion `pCtx->pOut->flags==MEM_Null` fails if the previous
 * call to sqlite3_result_* did not successfully clear the output or if the
 * internal state is corrupted. In aggregate functions, if xStep returns without
 * setting a result, or if it sets a result that conflicts with the aggregate
 * state, this can happen.
 *
 * The most robust way to avoid this specific assertion in the context of fuzzing
 * where we might trigger edge cases is to ensure we don't accidentally leave
 * the context in a bad state. However, the assertion is internal to SQLite's
 * VDBE execution for aggregate functions.
 *
 * A common cause for this specific assertion failure in fuzzing is calling
 * sqlite3_aggregate_context with a size > 0 in xFinal or calling it multiple
 * times with different sizes in a way that confuses the allocator, OR
 * failing to set a result in xStep when the VM expects the aggregate to have
 * produced a value for the row.
 *
 * Actually, the assertion `pCtx->pOut->flags==MEM_Null` in sqlite3.c:103045
 * (in the `sqlite3_aggregate_context` function) checks that the output memory
 * cell is NULL before allocating new context. This implies that the previous
 * step or the initial state was not cleaned up.
 *
 * In a standard aggregate, xStep is called for each row. If xStep sets a result
 * (e.g., sqlite3_result_int), that result is stored in pOut. When xStep is
 * called again for the next row, the VDBE should clear pOut before calling xStep.
 * If the VDBE doesn't clear it, or if our function logic is weird, it might fail.
 *
 * However, the most likely cause in a fuzzing context where we are just trying
 * to exercise the API is that we are not handling the "no rows" case or the
 * transition between steps correctly in a way that SQLite's internal state
 * machine expects.
 *
 * Let's try a simpler approach:
 * 1. Ensure we always set a result in xStep.
 * 2. Ensure we call sqlite3_aggregate_context with nBytes=0 in xFinal.
 * 3. Avoid complex logic that might leave state inconsistent.
 *
 * The error trace shows the assertion failing inside `sqlite3_aggregate_context`.
 * This happens when the function is called and `pCtx->pOut->flags` is NOT `MEM_Null`.
 * This usually means the previous operation (likely the previous xStep or the
 * initialization) left a value in the output register that wasn't cleared.
 *
 * Wait, `sqlite3_aggregate_context` is supposed to be called in xStep.
 * If xStep sets a result, that result is for the *current* row's output of the
 * aggregate? No, aggregate functions don't return a value per row in the same way.
 * The "result" of an aggregate function is only returned at the end (xFinal).
 * So, in xStep, we typically do NOT call sqlite3_result_*. We only update the
 * context memory.
 *
 * If we call sqlite3_result_int in xStep, we are telling SQLite "The result of
 * this aggregate function for this row is X". But aggregates don't work that way.
 * The VDBE expects the aggregate function to accumulate state in the context,
 * and NOT produce a row-by-row result.
 *
 * If we call sqlite3_result_* in xStep, we might be corrupting the pOut state
 * that the VDBE expects to be NULL (or in a specific state) when it calls
 * sqlite3_aggregate_context again for the next row.
 *
 * CORRECTION: Do NOT call sqlite3_result_* in xStep for an aggregate function.
 * Only call it in xFinal.
 */

/* Global variable to hold the fuzz input for the callback to use */
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

/*
 * Step callback for our aggregate function.
 * DO NOT call sqlite3_result_* here. Only update context.
 */
static void fuzz_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* 1. Invoke sqlite3_aggregate_context */
    /* Use fuzz data to determine size, but clamp to reasonable limits to avoid OOM */
    int nBytes = 0;
    if (g_fuzz_size >= 2) {
        nBytes = (int)((g_fuzz_data[0] << 8) | g_fuzz_data[1]);
        /* Limit allocation size to prevent immediate OOM or excessive memory usage */
        if (nBytes > 1024 * 1024) nBytes = 1024 * 1024; 
        if (nBytes < 0) nBytes = 0;
    }
    
    void *ctxMem = sqlite3_aggregate_context(ctx, nBytes);
    (void)ctxMem; /* Use the return value to avoid unused warning, but don't rely on it being non-null if nBytes=0 */
    
    /* 2. Invoke sqlite3_value_bytes */
    /* We need an argument to pass to this. If argc > 0, use the first argument. */
    if (argc > 0 && argv[0] != NULL) {
        int bytes = sqlite3_value_bytes(argv[0]);
        (void)bytes; /* Use the return value to ensure it's compiled */
        
        /* 3. Invoke sqlite3_value_text */
        const unsigned char *text = sqlite3_value_text(argv[0]);
        (void)text; /* Use the return value */
    }
    
    /* 
       IMPORTANT: Do NOT call sqlite3_result_* in xStep for an aggregate function.
       The aggregate state is stored in the context. The result is only set in xFinal.
       Calling sqlite3_result_* here was likely the cause of the assertion failure
       because it left pOut in a non-NULL state for the next call to aggregate_context.
    */
}

/*
 * Final callback for our aggregate function.
 */
static void fuzz_aggregate_final(sqlite3_context *ctx) {
    /* Call aggregate_context with 0 as per documentation for xFinal */
    void *ctxMem = sqlite3_aggregate_context(ctx, 0);
    (void)ctxMem;
    
    /* Set the final result here */
    sqlite3_result_int(ctx, 1);
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    if (Size == 0) {
        return 0;
    }

    /* Set global fuzz data for the callback */
    g_fuzz_data = Data;
    g_fuzz_size = Size;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the aggregate function */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_agg",
        -1, /* Variable number of arguments */
        SQLITE_UTF8,
        NULL, /* appData */
        NULL, /* xFunc (NULL for aggregate) */
        fuzz_aggregate_step, /* xStep */
        fuzz_aggregate_final, /* xFinal */
        NULL  /* xDestroy */
    );

    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to test aggregation on */
    rc = sqlite3_exec(db, "CREATE TABLE test_table (id INTEGER, data TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert some dummy data to ensure the aggregate function runs */
    rc = sqlite3_exec(db, "INSERT INTO test_table VALUES (1, 'hello'), (2, 'world'), (3, 'fuzz');", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a statement that uses our aggregate function */
    const char *sql = "SELECT fuzz_agg(data) FROM test_table;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Step through the query to trigger the callbacks */
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        /* We don't need to do anything with the result, just ensure the function ran */
        (void)sqlite3_column_text(stmt, 0);
    }

    /* 2. Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    /* Reset globals */
    g_fuzz_data = NULL;
    g_fuzz_size = 0;

    return 0;
}