// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_blob at sqlite3.c:93636:24 in sqlite3.h
// sqlite3_column_bytes at sqlite3.c:93646:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_text at sqlite3.c:93671:33 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int64 at sqlite3.c:93666:25 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_int64 at sqlite3.c:92478:25 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:92453:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_result_blob at sqlite3.c:92706:17 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_result_text16 at sqlite3.c:92871:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 result and value functions.
 * This driver creates a database, registers custom functions,
 * and executes queries to exercise the target APIs.
 */

/* Custom aggregate state structure */
typedef struct {
    int sum;
    int count;
    int max_val;
} AggState;

/*
 * Scalar function implementation: returns an int64
 */
static void scalar_func_int64(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    /* Fuzzing: extract a 64-bit integer from input if possible, else use a constant */
    sqlite3_int64 val = 0;
    if (argc > 0) {
        val = sqlite3_value_int64(argv[0]);
    }
    
    /* Invoke target API: sqlite3_result_int64 */
    sqlite3_result_int64(ctx, val);
}

/*
 * Scalar function implementation: returns a blob
 */
static void scalar_func_blob(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    
    if (argc > 0) {
        const void *blob = sqlite3_value_blob(argv[0]);
        int len = sqlite3_value_bytes(argv[0]);
        
        /* Invoke target API: sqlite3_result_blob */
        /* Use SQLITE_TRANSIENT to force a copy */
        sqlite3_result_blob(ctx, blob, len, SQLITE_TRANSIENT);
    } else {
        /* Fallback for no args */
        const char *dummy = "dummy_blob";
        sqlite3_result_blob(ctx, dummy, strlen(dummy), SQLITE_TRANSIENT);
    }
}

/*
 * Scalar function implementation: returns UTF-8 text
 */
static void scalar_func_text(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    
    if (argc > 0) {
        const unsigned char *text = sqlite3_value_text(argv[0]);
        int len = sqlite3_value_bytes(argv[0]);
        
        /* Invoke target API: sqlite3_result_text */
        sqlite3_result_text(ctx, (const char *)text, len, SQLITE_TRANSIENT);
    } else {
        const char *dummy = "dummy_text";
        sqlite3_result_text(ctx, dummy, -1, SQLITE_TRANSIENT);
    }
}

/*
 * Scalar function implementation: returns UTF-16 text
 */
static void scalar_func_text16(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    
    /* Create a simple UTF-16 string: "A" = 0x0041 (little endian) or 0x4100 (big endian) */
    /* We'll use a static buffer for simplicity */
    static const unsigned char utf16_dummy[] = {0x41, 0x00, 0x00, 0x00}; /* 'A' in LE, null terminated */
    
    /* Invoke target API: sqlite3_result_text16 */
    sqlite3_result_text16(ctx, utf16_dummy, 4, SQLITE_TRANSIENT);
}

/*
 * Aggregate function: Step
 */
static void agg_func_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    /* Invoke target API: sqlite3_aggregate_context */
    AggState *state = (AggState *)sqlite3_aggregate_context(ctx, sizeof(AggState));
    
    if (state == NULL) {
        /* Allocation failed, return NULL or error */
        sqlite3_result_null(ctx);
        return;
    }
    
    /* Initialize on first call */
    if (state->count == 0) {
        state->sum = 0;
        state->max_val = -2147483648; /* INT_MIN */
    }
    
    /* Accumulate */
    if (argc > 0) {
        int val = sqlite3_value_int(argv[0]);
        state->sum += val;
        if (val > state->max_val) {
            state->max_val = val;
        }
        state->count++;
    }
}

/*
 * Aggregate function: Final
 */
static void agg_func_final(sqlite3_context *ctx) {
    /* Invoke target API: sqlite3_aggregate_context with 0 to avoid reallocation */
    AggState *state = (AggState *)sqlite3_aggregate_context(ctx, 0);
    
    if (state == NULL || state->count == 0) {
        sqlite3_result_null(ctx);
        return;
    }
    
    /* Return the sum as int64 */
    sqlite3_result_int64(ctx, (sqlite3_int64)state->sum);
}

/*
 * Helper function to create a dummy file if needed
 */
static int create_dummy_file(const char *filename, const uint8_t *data, size_t size) {
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    
    if (size > 0) {
        fwrite(data, 1, size, f);
    }
    fclose(f);
    return 0;
}

/*
 * Main fuzzing entry point
 */
int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int ret = 0;
    
    /* Create a temporary database file */
    const char *db_file = "./dummy_file";
    
    /* Initialize database */
    rc = sqlite3_open(db_file, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Register custom functions */
    /* Scalar functions */
    rc = sqlite3_create_function(db, "fuzz_int64", -1, SQLITE_UTF8, NULL, scalar_func_int64, NULL, NULL);
    if (rc != SQLITE_OK) { sqlite3_close(db); return 0; }
    
    rc = sqlite3_create_function(db, "fuzz_blob", -1, SQLITE_UTF8, NULL, scalar_func_blob, NULL, NULL);
    if (rc != SQLITE_OK) { sqlite3_close(db); return 0; }
    
    rc = sqlite3_create_function(db, "fuzz_text", -1, SQLITE_UTF8, NULL, scalar_func_text, NULL, NULL);
    if (rc != SQLITE_OK) { sqlite3_close(db); return 0; }
    
    rc = sqlite3_create_function(db, "fuzz_text16", -1, SQLITE_UTF8, NULL, scalar_func_text16, NULL, NULL);
    if (rc != SQLITE_OK) { sqlite3_close(db); return 0; }
    
    /* Aggregate function */
    rc = sqlite3_create_function(db, "fuzz_agg", -1, SQLITE_UTF8, NULL, NULL, agg_func_step, agg_func_final);
    if (rc != SQLITE_OK) { sqlite3_close(db); return 0; }
    
    /* Create a table to test with */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) { sqlite3_close(db); return 0; }
    
    /* Insert fuzz data if available */
    if (Size > 0) {
        const char *insert_sql = "INSERT INTO test_table (data) VALUES (X'00');";
        rc = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
        if (rc != SQLITE_OK) { sqlite3_close(db); return 0; }
        
        /* Try to insert the fuzz data as a blob (safely truncated if too large) */
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        sqlite3_exec(db, "DELETE FROM test_table;", NULL, NULL, NULL);
        
        /* Prepare insert statement */
        const char *insert_stmt_sql = "INSERT INTO test_table (data) VALUES (?);";
        rc = sqlite3_prepare_v2(db, insert_stmt_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            /* Bind the fuzz data as a blob */
            sqlite3_bind_blob(stmt, 1, Data, Size, SQLITE_TRANSIENT);
            rc = sqlite3_step(stmt);
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }
    
    /* Execute queries to exercise the custom functions */
    
    /* Test scalar_int64 */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_int64(123456789012345);", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is retrieved but not used, just exercising the path */
            /* FIX: Use sqlite3_column_int64 directly to avoid unprotected value issues */
            (void)sqlite3_column_int64(stmt, 0);
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    /* Test scalar_blob */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_blob(data) FROM test_table LIMIT 1;", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const void *blob = sqlite3_column_blob(stmt, 0);
            int len = sqlite3_column_bytes(stmt, 0);
            (void)blob; (void)len; /* Just to exercise the path */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    /* Test scalar_text */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_text('hello');", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char *text = (const char *)sqlite3_column_text(stmt, 0);
            (void)text; /* Just to exercise the path */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    /* Test scalar_text16 */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_text16();", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char *text16 = (const char *)sqlite3_column_text(stmt, 0);
            (void)text16; /* Just to exercise the path */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    /* Test aggregate function */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_agg(id) FROM test_table;", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* FIX: Use sqlite3_column_int64 directly to avoid unprotected value issues */
            (void)sqlite3_column_int64(stmt, 0);
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    /* Test with empty table */
    rc = sqlite3_prepare_v2(db, "SELECT fuzz_agg(id) FROM test_table WHERE id > 999999;", -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Should return NULL */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}