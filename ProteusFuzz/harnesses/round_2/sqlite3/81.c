// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text16le at sqlite3.c:92889:17 in sqlite3.h
// sqlite3_result_pointer at sqlite3.c:92788:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 result setting functions.
 * Target APIs:
 * - sqlite3_result_blob
 * - sqlite3_aggregate_context
 * - sqlite3_result_int
 * - sqlite3_result_pointer
 * - sqlite3_result_text16le
 * - sqlite3_result_text
 */

/*
 * Custom scalar function that exercises sqlite3_result_blob,
 * sqlite3_result_int, sqlite3_result_text, sqlite3_result_text16le,
 * and sqlite3_result_pointer based on input data.
 */
static void custom_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /*
     * We cannot directly access fuzzing input here, so we rely on
     * the fuzzing harness to call this function multiple times with
     * different internal states or via different query paths.
     * To simulate diverse behavior, we use a static counter.
     */
    static int call_count = 0;
    call_count++;
    int mode = call_count % 5;

    switch (mode) {
        case 0:
            /* sqlite3_result_blob */
            {
                const char *blob_data = "BLOB_DATA";
                int len = 9;
                sqlite3_result_blob(ctx, blob_data, len, SQLITE_STATIC);
            }
            break;
        case 1:
            /* sqlite3_result_int */
            sqlite3_result_int(ctx, call_count);
            break;
        case 2:
            /* sqlite3_result_text */
            {
                const char *text = "Hello World";
                sqlite3_result_text(ctx, text, -1, SQLITE_STATIC);
            }
            break;
        case 3:
            /* sqlite3_result_text16le */
            {
                /* Simple UTF-16LE string: "Hi" -> 0x48 0x00 0x69 0x00 */
                const char utf16le[] = {0x48, 0x00, 0x69, 0x00};
                sqlite3_result_text16le(ctx, utf16le, 4, SQLITE_STATIC);
            }
            break;
        case 4:
            /* sqlite3_result_pointer */
            {
                static char dummy_ptr_val = 0x42;
                sqlite3_result_pointer(ctx, &dummy_ptr_val, "DUMMY_TYPE", NULL);
            }
            break;
    }
}

/*
 * Custom aggregate function step callback that exercises
 * sqlite3_aggregate_context and sqlite3_result_int.
 */
static void custom_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* Allocate or retrieve aggregate context */
    void *pCtx = sqlite3_aggregate_context(ctx, 4);
    if (pCtx) {
        int *pVal = (int *)pCtx;
        (*pVal)++;
    }
}

/*
 * Custom aggregate function final callback that exercises
 * sqlite3_aggregate_context with 0 bytes and sqlite3_result_int.
 */
static void custom_agg_final(sqlite3_context *ctx) {
    if (!ctx) return;

    /* Retrieve context without reallocating */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    if (pCtx) {
        int *pVal = (int *)pCtx;
        sqlite3_result_int(ctx, *pVal);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

/*
 * LLVMFuzzerTestOneInput: Entry point for fuzzing.
 * Creates a database, registers custom functions, and executes queries
 * to trigger the custom function callbacks.
 */
int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    sqlite3_stmt *stmt = NULL;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom scalar function */
    rc = sqlite3_create_function(
        db,
        "custom_scalar",
        -1,
        SQLITE_UTF8,
        NULL,
        custom_scalar_func,
        NULL,
        NULL
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom aggregate function */
    /* 
     * Signature: sqlite3_create_function(
     *   sqlite3 *db, const char *zName, int nArg, int eTextRep, void *pApp,
     *   void (*xFunc)(sqlite3_context*,int,sqlite3_value**),
     *   void (*xStep)(sqlite3_context*,int,sqlite3_value**),
     *   void (*xFinal)(sqlite3_context*)
     * );
     * For aggregates, xFunc is NULL, xStep is the step callback, xFinal is the final callback.
     */
    rc = sqlite3_create_function(
        db,
        "custom_agg",
        -1,
        SQLITE_UTF8,
        NULL,
        NULL,               /* xFunc (NULL for aggregates) */
        custom_agg_step,    /* xStep */
        custom_agg_final    /* xFinal */
    );
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * Use input data to generate diverse SQL queries.
     * We ensure queries are safe and well-formed to avoid immediate crashes
     * from syntax errors, while still exercising different code paths.
     */
    if (Size > 0) {
        /*
         * Generate a simple query that calls our custom functions.
         * We use the input data to vary the query slightly.
         */
        char query[256];
        int idx = (Size % 3);

        if (idx == 0) {
            /* Call scalar function */
            snprintf(query, sizeof(query), "SELECT custom_scalar(%d);", (int)Data[0]);
        } else if (idx == 1) {
            /* Call aggregate function */
            snprintf(query, sizeof(query), "SELECT custom_agg(%d);", (int)Data[0]);
        } else {
            /* Call both */
            snprintf(query, sizeof(query), "SELECT custom_scalar(%d), custom_agg(%d);",
                     (int)Data[0], (int)Data[Size > 1 ? 1 : 0]);
        }

        rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            /* Execute the statement */
            rc = sqlite3_step(stmt);
            /* Ignore return value; we just want to trigger the functions */
            sqlite3_finalize(stmt);
        }
    }

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}