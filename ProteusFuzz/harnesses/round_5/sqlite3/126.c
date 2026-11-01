// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Custom scalar function to test sqlite3_result_int, sqlite3_result_int64,
 * sqlite3_result_null.
 */
static void test_scalar_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    /* Test sqlite3_result_int */
    sqlite3_result_int(ctx, 42);
    
    /* Test sqlite3_result_int64 */
    sqlite3_result_int64(ctx, 0x123456789ABCDEF0LL);
    
    /* Test sqlite3_result_null */
    sqlite3_result_null(ctx);
}

/*
 * Custom aggregate function step to test sqlite3_aggregate_context.
 */
static void test_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    /* Request 100 bytes of context memory */
    int *state = (int *)sqlite3_aggregate_context(ctx, 100);
    if (state) {
        state[0] = 1;
    }
}

/*
 * Custom aggregate function final to test sqlite3_aggregate_context with 0 bytes.
 */
static void test_agg_final(sqlite3_context *ctx) {
    /* Request 0 bytes to avoid reallocation, just access existing state */
    int *state = (int *)sqlite3_aggregate_context(ctx, 0);
    if (state) {
        sqlite3_result_int64(ctx, (sqlite3_int64)state[0]);
    } else {
        sqlite3_result_null(ctx);
    }
}

/*
 * Dummy virtual table xColumn implementation to test sqlite3_vtab_nochange.
 * Correct signature: int xColumn(sqlite3_vtab_cursor *pVtabCursor, sqlite3_context *pCtx, int iCol);
 */
static int dummy_vtab_column(sqlite3_vtab_cursor *pVtabCursor, sqlite3_context *ctx, int col) {
    (void)pVtabCursor;
    (void)col;
    
    /* Check if this is a no-change update */
    if (sqlite3_vtab_nochange(ctx)) {
        /* Optimization: do not set result, rely on xUpdate to handle no-change */
        return SQLITE_OK;
    }
    
    /* Normal path: set a result */
    sqlite3_result_int(ctx, 123);
    return SQLITE_OK;
}

/*
 * Dummy virtual table xUpdate implementation to verify no-change logic.
 * Correct signature: int xUpdate(sqlite3_vtab *pVtab, int nArg, sqlite3_value **argv, sqlite3_int64 *pRowid);
 */
static int dummy_vtab_update(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVtab;
    (void)argc;
    (void)argv;
    (void)pRowid;
    
    /* In a real implementation, we would check sqlite3_value_nochange here.
       For fuzzing, we just return success. */
    return SQLITE_OK;
}

/*
 * Helper to test sqlite3_result_value.
 * We create a value using the API and pass it to the result function.
 */
static void test_result_value_function(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (argc > 0) {
        /* Pass the first argument directly as the result */
        sqlite3_result_value(ctx, argv[0]);
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    sqlite3 *db;
    int rc;
    
    /* 1. Prepare the environment: Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a dummy table to allow virtual table registration if needed */
    rc = sqlite3_exec(db, "CREATE TABLE dummy (id INTEGER);", NULL, NULL, NULL);
    (void)rc;

    /* Register custom scalar function */
    rc = sqlite3_create_function(db, "fuzz_scalar", 0, SQLITE_UTF8, NULL, test_scalar_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom aggregate function */
    rc = sqlite3_create_function(db, "fuzz_agg", 0, SQLITE_UTF8, NULL, NULL, test_agg_step, test_agg_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register function to test sqlite3_result_value */
    rc = sqlite3_create_function(db, "fuzz_result_value", 1, SQLITE_UTF8, NULL, test_result_value_function, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register virtual table module */
    sqlite3_module mod = {0};
    
    /* Set up the module with our dummy functions for testing */
    /* Fixed function pointer types to match sqlite3_module definition */
    mod.xColumn = dummy_vtab_column;
    mod.xUpdate = dummy_vtab_update;

    rc = sqlite3_create_module(db, "fuzz_vtab", &mod, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Execute queries to trigger the target functions */
    
    /* Test scalar function */
    rc = sqlite3_exec(db, "SELECT fuzz_scalar();", NULL, NULL, NULL);
    (void)rc;

    /* Test aggregate function */
    rc = sqlite3_exec(db, "SELECT fuzz_agg() FROM dummy;", NULL, NULL, NULL);
    (void)rc;

    /* Test virtual table column access (simulating an update scenario) */
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE fuzz_test USING fuzz_vtab;", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        /* Try to read a column (triggers xColumn) */
        rc = sqlite3_exec(db, "SELECT * FROM fuzz_test;", NULL, NULL, NULL);
        (void)rc;
        
        /* Try to update a column (triggers xColumn with nochange hint potentially) */
        rc = sqlite3_exec(db, "UPDATE fuzz_test SET col = 1 WHERE 1=0;", NULL, NULL, NULL);
        (void)rc;
    }

    /* 3. Explore more states by using fuzzed data in SQL strings if safe */
    if (Size > 0) {
        /* Escape single quotes to prevent SQL injection in our test query */
        char *escaped = malloc(Size * 2 + 1);
        if (escaped) {
            size_t j = 0;
            for (size_t i = 0; i < Size; i++) {
                if (Data[i] == '\'') {
                    escaped[j++] = '\'';
                    escaped[j++] = '\'';
                } else {
                    escaped[j++] = Data[i];
                }
            }
            escaped[j] = '\0';
            
            /* Use the fuzzed data in a query that calls our result_value function */
            char query[512];
            snprintf(query, sizeof(query), "SELECT fuzz_result_value('%s');", escaped);
            rc = sqlite3_exec(db, query, NULL, NULL, NULL);
            (void)rc;
            
            free(escaped);
        }
    }

    /* 4. Cleanup */
    sqlite3_close(db);
    
    return 0;
}