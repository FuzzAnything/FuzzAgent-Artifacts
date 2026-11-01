// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:92466:16 in sqlite3.h
// sqlite3_value_text at sqlite3.c:92498:33 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void step_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Step 1: Invoke sqlite3_aggregate_context to allocate persistent state */
    /* We use the input data size to determine allocation size, clamped to avoid overflow */
    int nBytes = 0;
    if (argc > 0 && argv[0]) {
        nBytes = sqlite3_value_bytes(argv[0]);
        if (nBytes < 0) nBytes = 0;
        if (nBytes > 1024) nBytes = 1024; /* Safety clamp */
    }
    void *pCtx = sqlite3_aggregate_context(ctx, nBytes);
    
    /* Step 2: Invoke sqlite3_value_bytes on the first argument */
    int byteLen = 0;
    if (argc > 0 && argv[0]) {
        byteLen = sqlite3_value_bytes(argv[0]);
    }
    
    /* Step 3: Invoke sqlite3_value_text on the first argument */
    const unsigned char *text = NULL;
    if (argc > 0 && argv[0]) {
        text = sqlite3_value_text(argv[0]);
    }
    
    /* Use the retrieved values to ensure the compiler doesn't optimize them away */
    (void)pCtx;
    (void)byteLen;
    (void)text;
    
    /* Set a dummy result to keep the function valid */
    sqlite3_result_int(ctx, 0);
}

static void final_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Invoke sqlite3_aggregate_context with 0 bytes in xFinal as per documentation */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    (void)pCtx;
    (void)argc;
    (void)argv;
    
    /* Set a dummy result */
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int ret;

    /* Create a dummy file if needed, though :memory: is safer for fuzzing */
    /* We use :memory: to avoid file system issues in the fuzzer environment */
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to aggregate over */
    const char *sql = "CREATE TABLE test_table (data TEXT);";
    rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert the fuzzing data into the table */
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    
    /* Prepare a statement to insert data */
    const char *insert_sql = "INSERT INTO test_table (data) VALUES (?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Bind the fuzzing input as text */
        sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
        
        /* Execute the insert */
        rc = sqlite3_step(stmt);
        /* We don't care if it fails, we just want to get data in there if possible */
        
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);

    /* Register the aggregate function that uses the target APIs */
    const char *func_name = "fuzz_agg";
    rc = sqlite3_create_function(db, func_name, 1, SQLITE_UTF8, NULL, step_func, final_func, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute a query that invokes the aggregate function */
    const char *query_sql = "SELECT fuzz_agg(data) FROM test_table;";
    rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK) {
        /* Step through the query to trigger the callbacks */
        while ((ret = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Process row if needed, but the side effects in callbacks are what we want */
        }
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    sqlite3_exec(db, "DROP TABLE test_table;", NULL, NULL, NULL);
    sqlite3_close(db);

    return 0;
}