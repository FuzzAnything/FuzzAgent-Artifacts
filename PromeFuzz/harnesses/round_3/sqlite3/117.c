// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:80018:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_text64 at sqlite3.c:78837:17 in sqlite3.h
// sqlite3_result_zeroblob at sqlite3.c:78910:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_aggregate_count at sqlite3.c:79505:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

/* Static functions to avoid name conflicts */
static void dummy_xFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Use fuzzer data to decide which result function to call */
    const uint8_t *data = sqlite3_value_blob(argv[0]);
    int size = sqlite3_value_bytes(argv[0]);
    
    if (size > 0) {
        uint8_t selector = data[0] % 5;
        
        switch (selector) {
            case 0:
                sqlite3_result_text64(ctx, (const char*)data, 
                                     (sqlite3_uint64)(size > 1000 ? 1000 : size),
                                     NULL, SQLITE_UTF8);
                break;
            case 1:
                sqlite3_result_zeroblob(ctx, size);
                break;
            case 2:
                sqlite3_result_null(ctx);
                break;
            case 3:
                sqlite3_result_int(ctx, size);
                break;
            case 4:
                /* Test aggregate count in scalar context */
                sqlite3_result_int(ctx, sqlite3_aggregate_count(ctx));
                break;
        }
    } else {
        sqlite3_result_null(ctx);
    }
}

static void dummy_xStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Aggregate step function - just count rows */
    /* No operation needed for counting */
}

static void dummy_xFinal(sqlite3_context *ctx) {
    /* Aggregate finalize function */
    int count = sqlite3_aggregate_count(ctx);
    sqlite3_result_int(ctx, count);
}

static void dummy_xDestroy(void *p) {
    /* Cleanup function for app data */
    free(p);
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    char *err_msg = NULL;
    int rc;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Test sqlite3_create_function_v2 */
    void *app_data = malloc(16);
    if (app_data) {
        memset(app_data, 0xAA, 16);
    }
    
    rc = sqlite3_create_function_v2(
        db,
        "fuzzfunc",
        1,  /* nArg */
        SQLITE_UTF8,
        app_data,
        dummy_xFunc,
        dummy_xStep,
        dummy_xFinal,
        dummy_xDestroy
    );
    
    if (rc == SQLITE_OK) {
        /* Create a table and insert some data to test functions */
        sqlite3_exec(db, "CREATE TABLE t1(x);", NULL, NULL, &err_msg);
        
        /* Insert fuzzer data as blob */
        if (Size > 0) {
            sqlite3_stmt *stmt;
            const char *insert_sql = "INSERT INTO t1 VALUES(?);";
            
            if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_blob(stmt, 1, Data, (int)(Size > 1000 ? 1000 : Size), SQLITE_STATIC);
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
        
        /* Call the registered scalar function */
        sqlite3_stmt *stmt;
        const char *select_sql = "SELECT fuzzfunc(x) FROM t1;";
        
        if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        /* Test aggregate version */
        const char *agg_sql = "SELECT fuzzfunc(x) FROM t1 GROUP BY x;";
        if (sqlite3_prepare_v2(db, agg_sql, -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
        
        if (err_msg) {
            sqlite3_free(err_msg);
        }
    }
    
    /* Test the result functions directly by creating a simple user-defined function */
    rc = sqlite3_create_function_v2(
        db,
        "direct_test",
        1,
        SQLITE_UTF8,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );
    
    if (rc == SQLITE_OK) {
        /* We'll test the functions through SQL execution */
        sqlite3_stmt *stmt;
        const char *test_sql = "SELECT length(hex(randomblob(?1)));";
        
        if (sqlite3_prepare_v2(db, test_sql, -1, &stmt, NULL) == SQLITE_OK) {
            if (Size > 0) {
                int param = Data[0] % 100;
                sqlite3_bind_int(stmt, 1, param);
            }
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}