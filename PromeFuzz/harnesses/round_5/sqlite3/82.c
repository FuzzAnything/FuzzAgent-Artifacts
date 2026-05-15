// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_value_int at sqlite3.c:78468:16 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy callbacks for sqlite3_create_function_v2 */
static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Use the fuzzer input to decide which result function to call */
    const uint8_t *data = (const uint8_t *)sqlite3_user_data(ctx);
    if (data && argc > 0) {
        switch (data[0] % 6) {
            case 0: sqlite3_result_null(ctx); break;
            case 1: sqlite3_result_int(ctx, sqlite3_value_int(argv[0])); break;
            case 2: {
                const char *text = (const char*)data;
                size_t len = strlen((char*)data);
                if (len > 0) {
                    sqlite3_result_text64(ctx, text, len > 64 ? 64 : len,
                                          SQLITE_TRANSIENT, SQLITE_UTF8);
                } else {
                    sqlite3_result_null(ctx);
                }
                break;
            }
            case 3: sqlite3_result_zeroblob(ctx, data[1] % 256); break;
            case 4: sqlite3_result_int(ctx, sqlite3_aggregate_count(ctx)); break;
            default: sqlite3_result_null(ctx); break;
        }
    } else {
        sqlite3_result_null(ctx);
    }
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Aggregate step function - just count rows */
    (void)ctx; (void)argc; (void)argv;
}

static void dummy_xFinal(sqlite3_context *ctx) {
    /* Aggregate finalize function */
    sqlite3_result_int(ctx, sqlite3_aggregate_count(ctx));
}

static void dummy_xDestroy(void *p) {
    /* Cleanup function for app data */
    free(p);
}

/* Static function for direct testing callback */
static void direct_test_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Test all result functions with fuzzer data */
    const uint8_t *data = (const uint8_t *)sqlite3_user_data(ctx);
    size_t data_size = 0;
    if (data) {
        /* Estimate size by looking for null terminator in first 1024 bytes */
        for (data_size = 0; data_size < 1024 && data[data_size]; data_size++);
    }
    
    /* Use different result functions based on fuzzer input */
    if (data && data_size > 0) {
        int selector = data[0] % 5;
        switch (selector) {
            case 0:
                sqlite3_result_null(ctx);
                break;
            case 1:
                sqlite3_result_int(ctx, (int)data[1]);
                break;
            case 2:
                if (data_size > 1) {
                    sqlite3_result_text64(ctx, (const char*)data+1, 
                                        data_size-1 > 64 ? 64 : data_size-1,
                                        SQLITE_TRANSIENT, SQLITE_UTF8);
                } else {
                    sqlite3_result_null(ctx);
                }
                break;
            case 3:
                sqlite3_result_zeroblob(ctx, (int)data[1] % 1024);
                break;
            case 4:
                /* Test aggregate count in scalar context (returns 0) */
                sqlite3_result_int(ctx, sqlite3_aggregate_count(ctx));
                break;
        }
    } else {
        sqlite3_result_null(ctx);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Copy fuzzer data for use as app data */
    uint8_t *app_data = NULL;
    if (Size > 0) {
        app_data = malloc(Size);
        if (app_data) {
            memcpy(app_data, Data, Size);
        }
    }
    
    /* Test sqlite3_create_function_v2 with various configurations */
    rc = sqlite3_create_function_v2(
        db,
        "fuzz_func",
        -1,  /* Variable number of arguments */
        SQLITE_UTF8 | SQLITE_DETERMINISTIC,
        app_data,
        dummy_xFunc,
        dummy_xStep,
        dummy_xFinal,
        dummy_xDestroy
    );
    
    if (rc == SQLITE_OK) {
        /* Execute SQL statements that use the function */
        if (Size > 0) {
            /* Create a table for testing */
            sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(x INTEGER, y TEXT);", NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            
            /* Insert some data using fuzzer input */
            char sql[256];
            if (Size > 10) {
                snprintf(sql, sizeof(sql), "INSERT INTO t1 VALUES(%d, '%.*s');", 
                        (int)Data[0], (int)(Size > 100 ? 100 : Size-1), Data+1);
                sqlite3_exec(db, sql, NULL, NULL, &err_msg);
                if (err_msg) sqlite3_free(err_msg);
            }
            
            /* Call our function with various arguments */
            sqlite3_exec(db, "SELECT fuzz_func(1);", NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            
            sqlite3_exec(db, "SELECT fuzz_func(x, y) FROM t1;", NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
            
            sqlite3_exec(db, "SELECT fuzz_func() FROM t1;", NULL, NULL, &err_msg);
            if (err_msg) sqlite3_free(err_msg);
        }
        
        /* Test aggregate functionality */
        sqlite3_exec(db, "SELECT fuzz_func(x) FROM t1 GROUP BY x;", NULL, NULL, &err_msg);
        if (err_msg) sqlite3_free(err_msg);
    }
    
    /* Test the result functions directly with a dummy context */
    if (Size > 0) {
        /* We need a valid sqlite3_context to test these functions.
           Create a temporary function and call it to get a context. */
        sqlite3_create_function_v2(
            db,
            "direct_test",
            1,
            SQLITE_UTF8,
            (void*)Data,
            direct_test_func,
            NULL, NULL, NULL
        );
        
        /* Execute the function */
        sqlite3_exec(db, "SELECT direct_test(1);", NULL, NULL, &err_msg);
        if (err_msg) sqlite3_free(err_msg);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}