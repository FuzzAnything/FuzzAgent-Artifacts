// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_subtype at sqlite3.c:78800:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:78767:17 in sqlite3.h
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

/* Dummy user data for testing */
static int dummy_user_data = 0x12345678;

/* Custom SQL function that uses all target APIs */
static void custom_function(
    sqlite3_context *context,
    int argc,
    sqlite3_value **argv
) {
    /* Test sqlite3_user_data */
    void *user_data = sqlite3_user_data(context);
    
    /* Use input data to determine behavior */
    if (argc > 0) {
        const uint8_t *data = sqlite3_value_blob(argv[0]);
        int data_size = sqlite3_value_bytes(argv[0]);
        
        if (data_size >= 16) {
            /* Extract values from input data */
            double d;
            sqlite3_int64 i64;
            unsigned int subtype;
            int agg_size;
            
            memcpy(&d, data, sizeof(double));
            memcpy(&i64, data + 8, sizeof(sqlite3_int64));
            memcpy(&subtype, data + 16, sizeof(unsigned int));
            memcpy(&agg_size, data + 20, sizeof(int));
            
            /* Test different result types based on input */
            if (data[0] & 0x01) {
                sqlite3_result_double(context, d);
            } else if (data[0] & 0x02) {
                sqlite3_result_int64(context, i64);
            } else if (data[0] & 0x04) {
                sqlite3_result_null(context);
            }
            
            /* Always test subtype if we have enough data */
            if (data_size >= 24) {
                sqlite3_result_subtype(context, subtype & 0xFF);
            }
            
            /* Test aggregate context */
            if (agg_size > 0) {
                void *agg_ctx = sqlite3_aggregate_context(context, agg_size);
                if (agg_ctx && agg_size <= 1024) {
                    /* Initialize aggregate context with data */
                    memcpy(agg_ctx, data, agg_size < data_size ? agg_size : data_size);
                }
            }
        } else {
            sqlite3_result_null(context);
        }
    } else {
        sqlite3_result_null(context);
    }
}

/* Aggregate function for testing sqlite3_aggregate_context */
static void aggregate_step(
    sqlite3_context *context,
    int argc,
    sqlite3_value **argv
) {
    /* Get aggregate context */
    void *agg_ctx = sqlite3_aggregate_context(context, 64);
    if (agg_ctx) {
        /* Store some data in aggregate context */
        static int counter = 0;
        *(int*)agg_ctx = ++counter;
    }
}

static void aggregate_final(sqlite3_context *context) {
    /* Retrieve and use aggregate context */
    void *agg_ctx = sqlite3_aggregate_context(context, 0);
    if (agg_ctx) {
        int *counter = (int*)agg_ctx;
        sqlite3_result_int64(context, *counter);
    } else {
        sqlite3_result_null(context);
    }
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *err_msg = NULL;
    
    /* Initialize database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Register custom scalar function */
    rc = sqlite3_create_function(
        db,
        "custom_func",
        1,
        SQLITE_UTF8 | SQLITE_DETERMINISTIC,
        &dummy_user_data,
        custom_function,
        NULL,
        NULL
    );
    
    /* Register custom aggregate function */
    rc = sqlite3_create_function(
        db,
        "custom_agg",
        1,
        SQLITE_UTF8,
        &dummy_user_data,
        NULL,
        aggregate_step,
        aggregate_final
    );
    
    if (rc == SQLITE_OK) {
        /* Create test table */
        rc = sqlite3_exec(db,
            "CREATE TABLE test (data BLOB);",
            NULL, NULL, &err_msg);
        
        if (rc == SQLITE_OK && Size > 0) {
            /* Insert fuzzed data into table */
            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db,
                "INSERT INTO test (data) VALUES (?);",
                -1, &stmt, NULL);
            
            if (rc == SQLITE_OK) {
                sqlite3_bind_blob(stmt, 1, Data, 
                    Size > 1024 ? 1024 : Size, SQLITE_STATIC);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                
                /* Test scalar function */
                sqlite3_stmt *stmt2;
                rc = sqlite3_prepare_v2(db,
                    "SELECT custom_func(data) FROM test;",
                    -1, &stmt2, NULL);
                
                if (rc == SQLITE_OK) {
                    while (sqlite3_step(stmt2) == SQLITE_ROW) {
                        /* Function executed, results handled internally */
                    }
                    sqlite3_finalize(stmt2);
                }
                
                /* Test aggregate function */
                sqlite3_stmt *stmt3;
                rc = sqlite3_prepare_v2(db,
                    "SELECT custom_agg(data) FROM test;",
                    -1, &stmt3, NULL);
                
                if (rc == SQLITE_OK) {
                    while (sqlite3_step(stmt3) == SQLITE_ROW) {
                        /* Function executed, results handled internally */
                    }
                    sqlite3_finalize(stmt3);
                }
            }
        }
        
        /* Clean up */
        if (err_msg) {
            sqlite3_free(err_msg);
        }
    }
    
    /* Close database */
    sqlite3_close(db);
    
    return 0;
}