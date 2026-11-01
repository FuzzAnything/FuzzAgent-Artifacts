// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_error16 at sqlite3.c:92758:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:92917:17 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_result_zeroblob64 at sqlite3.c:92920:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Custom SQL function implementation to fuzz sqlite3_aggregate_context
 * and sqlite3_result_* functions.
 */
static void fuzz_aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Fuzz sqlite3_aggregate_context with various sizes derived from input */
    int nBytes = 0;
    if (argc > 0) {
        nBytes = sqlite3_value_int(argv[0]);
        /* Clamp to reasonable range to avoid immediate OOM or negative sizes */
        if (nBytes < 0) nBytes = -nBytes;
        if (nBytes > 1024 * 1024) nBytes = 1024 * 1024;
    }

    void *pCtx = sqlite3_aggregate_context(ctx, nBytes);
    if (pCtx) {
        /* Write some fuzz data into the context if available */
        if (argc > 1 && sqlite3_value_type(argv[1]) == SQLITE_BLOB) {
            const void *blob = sqlite3_value_blob(argv[1]);
            int blobLen = sqlite3_value_bytes(argv[1]);
            if (blob && blobLen > 0) {
                int copyLen = (blobLen < nBytes) ? blobLen : nBytes;
                memcpy(pCtx, blob, copyLen);
            }
        }
    }
}

static void fuzz_aggregate_final(sqlite3_context *ctx) {
    /* Call aggregate_context with 0 as per documentation for xFinal */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    
    /* Fuzz various result setting functions */
    /* 1. sqlite3_result_int */
    int valInt = 0x12345678; 
    sqlite3_result_int(ctx, valInt);

    /* 2. sqlite3_result_blob (using static data to avoid allocation issues in fuzz loop) */
    static const char blobData[] = "FUZZ_BLOB_DATA";
    sqlite3_result_blob(ctx, blobData, sizeof(blobData) - 1, SQLITE_STATIC);

    /* 3. sqlite3_result_error16 */
    /* Use uint16_t array to represent UTF-16 string manually to avoid char16_t issues */
    static const uint16_t error16[] = {0x0045, 0x0072, 0x0072, 0x006F, 0x0072, 0x0031, 0x0036, 0x0000};
    sqlite3_result_error16(ctx, error16, -1);

    /* 4. sqlite3_result_zeroblob */
    sqlite3_result_zeroblob(ctx, 100);

    /* 5. sqlite3_result_zeroblob64 */
    sqlite3_uint64 n64 = 200;
    sqlite3_result_zeroblob64(ctx, n64);
}

/*
 * Simple scalar function to test result functions directly
 */
static void fuzz_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Test sqlite3_result_int */
    int val = 0;
    if (argc > 0) val = sqlite3_value_int(argv[0]);
    sqlite3_result_int(ctx, val);

    /* Test sqlite3_result_blob */
    if (argc > 1 && sqlite3_value_type(argv[1]) == SQLITE_BLOB) {
        const void *blob = sqlite3_value_blob(argv[1]);
        int len = sqlite3_value_bytes(argv[1]);
        if (len > 0) {
            sqlite3_result_blob(ctx, blob, len, SQLITE_TRANSIENT);
        }
    }

    /* Test sqlite3_result_error16 */
    if (argc > 2) {
        const void *errData = sqlite3_value_blob(argv[2]);
        int errLen = sqlite3_value_bytes(argv[2]);
        if (errData && errLen > 0) {
            sqlite3_result_error16(ctx, errData, errLen);
        }
    }

    /* Test sqlite3_result_zeroblob */
    if (argc > 3) {
        int n = sqlite3_value_int(argv[3]);
        if (n < 0) n = 0;
        sqlite3_result_zeroblob(ctx, n);
    }

    /* Test sqlite3_result_zeroblob64 */
    if (argc > 4) {
        sqlite3_uint64 n64 = sqlite3_value_int64(argv[4]);
        sqlite3_result_zeroblob64(ctx, n64);
    }
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom functions */
    rc = sqlite3_create_function(db, "fuzz_agg", -1, SQLITE_UTF8, NULL, 
                                  NULL, fuzz_aggregate_step, fuzz_aggregate_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_function(db, "fuzz_scalar", -1, SQLITE_UTF8, NULL, 
                                  fuzz_scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to aggregate over */
    rc = sqlite3_exec(db, "CREATE TABLE test_table (id INTEGER, data BLOB);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert fuzz data into the table if available, using parameterized query to avoid buffer overflow */
    if (Size > 0) {
        sqlite3_stmt *insertStmt = NULL;
        rc = sqlite3_prepare_v2(db, "INSERT INTO test_table VALUES (?, ?);", -1, &insertStmt, NULL);
        if (rc == SQLITE_OK) {
            /* Bind first byte as integer ID */
            sqlite3_bind_int(insertStmt, 1, (int)Data[0]);
            /* Bind remaining data as BLOB */
            sqlite3_bind_blob(insertStmt, 2, Data, Size, SQLITE_STATIC);
            
            sqlite3_step(insertStmt);
            sqlite3_finalize(insertStmt);
        }
    }

    /* Execute queries to trigger the custom functions */
    
    /* 1. Trigger aggregate function */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_agg(id, data) FROM test_table;", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Consume the result to ensure cleanup */
            sqlite3_value *val = sqlite3_column_value(stmt, 0);
            if (val) {
                (void)sqlite3_value_type(val);
            }
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* 2. Trigger scalar function with various arguments */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_scalar(1, X'FF', X'EE', 50, 1000);", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *val = sqlite3_column_value(stmt, 0);
            if (val) {
                (void)sqlite3_value_type(val);
            }
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* 3. Trigger aggregate with no rows (xFinal called without xStep) */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_agg(id, data) FROM test_table WHERE id > 999999;", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value *val = sqlite3_column_value(stmt, 0);
            if (val) {
                (void)sqlite3_value_type(val);
            }
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Cleanup */
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}