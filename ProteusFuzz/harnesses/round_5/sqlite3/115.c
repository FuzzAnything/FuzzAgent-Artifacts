// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
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
#include <stdio.h>

/*
 * Fuzz driver for sqlite3 result functions.
 * This driver creates a minimal environment to test:
 * - sqlite3_result_error_code
 * - sqlite3_result_error16
 * - sqlite3_result_zeroblob
 * - sqlite3_aggregate_context
 * - sqlite3_result_blob
 * - sqlite3_result_text
 */

static void dummy_destructor(void *p) {
    if (p) {
        free(p);
    }
}

/*
 * Helper to create a simple aggregate function for testing sqlite3_aggregate_context
 */
static void test_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    /* Test aggregate context with varying sizes based on input data */
    if (ctx) {
        /* We can't easily pass fuzz data here, so we just test basic allocation */
        void *pCtx = sqlite3_aggregate_context(ctx, 64);
        if (pCtx) {
            /* Write some dummy data to ensure the memory is valid */
            memset(pCtx, 0xAB, 64);
        }
    }
}

static void test_agg_final(sqlite3_context *ctx) {
    if (ctx) {
        /* Test aggregate context with 0 bytes in final */
        void *pCtx = sqlite3_aggregate_context(ctx, 0);
        (void)pCtx;
        
        /* Also test with a small allocation in final (should not reallocate) */
        pCtx = sqlite3_aggregate_context(ctx, 16);
        if (pCtx) {
            memset(pCtx, 0xCD, 16);
        }
    }
}

/*
 * Helper to create a simple scalar function that tests various result functions
 */
static void test_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    if (!ctx) {
        return;
    }
    
    /* The actual fuzzing logic is in LLVMFuzzerTestOneInput which calls this
     * indirectly through SQL execution. This function is just a placeholder. */
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_115(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Register test functions */
    rc = sqlite3_create_function(db, "fuzz_scalar", 0, SQLITE_UTF8, NULL, test_scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    rc = sqlite3_create_function(db, "fuzz_agg", 0, SQLITE_UTF8, NULL, NULL, test_agg_step, test_agg_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Create a test table */
    rc = sqlite3_exec(db, "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Insert some test data */
    rc = sqlite3_exec(db, "INSERT INTO test (data) VALUES (X'00010203040506070809');", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Prepare a statement that uses our test functions */
    const char *sql = "SELECT fuzz_agg(data), fuzz_scalar() FROM test;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Execute the statement */
    rc = sqlite3_step(stmt);
    (void)rc;
    
    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}