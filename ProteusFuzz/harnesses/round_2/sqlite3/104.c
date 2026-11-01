// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_error_code at sqlite3.c:92939:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to create a dummy database file if needed */
static void create_dummy_db(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fclose(f);
    }
}

/*
 * Aggregate step function: Tests sqlite3_aggregate_context and sqlite3_result_error16
 */
static void agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* Test sqlite3_aggregate_context with varying sizes */
    void *pCtx = sqlite3_aggregate_context(ctx, 100);
    if (pCtx) {
        /* Simulate some state update */
        memset(pCtx, 0x41, 100);
    }

    /* Test sqlite3_result_error16 with a static UTF-16 string */
    static const uint16_t utf16_err[] = {0x0041, 0x0042, 0x0000};
    sqlite3_result_error16(ctx, utf16_err, 6);
}

/*
 * Aggregate final function: Tests sqlite3_result_error_code and sqlite3_result_text
 */
static void agg_final(sqlite3_context *ctx) {
    /* Test sqlite3_aggregate_context with 0 bytes in finalizer */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    (void)pCtx;

    /* Test sqlite3_result_error_code */
    sqlite3_result_error_code(ctx, SQLITE_CORRUPT);

    /* Test sqlite3_result_text with static string */
    const char *static_text = "Error message";
    sqlite3_result_text(ctx, static_text, -1, SQLITE_STATIC);
}

/*
 * Scalar function: Tests sqlite3_result_blob and sqlite3_result_zeroblob
 */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* Test sqlite3_result_blob with static data */
    static const char blob_data[] = "BLOB_DATA";
    sqlite3_result_blob(ctx, blob_data, sizeof(blob_data) - 1, SQLITE_STATIC);

    /* Test sqlite3_result_zeroblob with varying sizes */
    int n = 10;
    if (argc > 0) {
        n = sqlite3_value_int(argv[0]);
        if (n < 0) n = 0;
        if (n > 1000000) n = 1000000; /* Cap to avoid OOM */
    }
    sqlite3_result_zeroblob(ctx, n);
}

/*
 * Another scalar function to test error paths and text
 */
static void error_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* Test sqlite3_result_error16 with dynamic allocation */
    uint16_t *dyn_utf16 = (uint16_t *)malloc(50);
    if (dyn_utf16) {
        memset(dyn_utf16, 0x42, 50);
        sqlite3_result_error16(ctx, dyn_utf16, 50);
        free(dyn_utf16);
    }

    /* Test sqlite3_result_text with transient data */
    char *dyn_text = (char *)malloc(20);
    if (dyn_text) {
        strcpy(dyn_text, "Dynamic Text");
        sqlite3_result_text(ctx, dyn_text, -1, free);
    }
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    (void)Data;
    (void)Size;

    /* Create dummy file if needed */
    create_dummy_db();

    /* Open database */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register aggregate function: xFunc is NULL, xStep and xFinal are provided */
    rc = sqlite3_create_function(db, "fuzz_agg", 0, SQLITE_UTF8, NULL, NULL, agg_step, agg_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register scalar function */
    rc = sqlite3_create_function(db, "fuzz_blob", 1, SQLITE_UTF8, NULL, scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register error function */
    rc = sqlite3_create_function(db, "fuzz_error", 0, SQLITE_UTF8, NULL, error_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to query */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, val TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a query using the aggregate function */
    sqlite3_stmt *stmt = NULL;
    const char *sql_agg = "SELECT fuzz_agg() FROM test LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql_agg, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Prepare a query using the scalar function with fuzzed input if possible */
    const char *sql_scalar = "SELECT fuzz_blob(1) FROM test LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql_scalar, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Prepare a query using the error function */
    const char *sql_error = "SELECT fuzz_error() FROM test LIMIT 1;";
    rc = sqlite3_prepare_v2(db, sql_error, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}