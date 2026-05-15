// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:80032:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_user_data at sqlite3.c:79215:18 in sqlite3.h
// sqlite3_get_auxdata at sqlite3.c:79417:18 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_value_blob at sqlite3.c:78446:24 in sqlite3.h
// sqlite3_set_auxdata at sqlite3.c:79448:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_result_double at sqlite3.c:78735:17 in sqlite3.h
// sqlite3_result_text at sqlite3.c:78822:17 in sqlite3.h
// sqlite3_value_bytes at sqlite3.c:78459:16 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:79396:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"

/* Dummy user data structure for testing */
static struct dummy_user_data {
    int id;
    char *name;
} dummy_data = {42, "test"};

/* Dummy destructor for auxiliary data */
static void dummy_destructor(void *ptr) {
    /* Do nothing - just for testing */
}

/* Dummy SQL function implementation that exercises the target APIs */
static void dummy_sql_function(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* Exercise sqlite3_user_data */
    void *user_data = sqlite3_user_data(context);
    
    /* Exercise sqlite3_get_auxdata for argument 0 */
    void *aux_data = sqlite3_get_auxdata(context, 0);
    
    /* Set some auxiliary data if we have an argument */
    if (argc > 0) {
        int size = sqlite3_value_bytes(argv[0]) % 256;
        if (size > 0) {
            char *data_copy = malloc(size);
            if (data_copy) {
                memcpy(data_copy, sqlite3_value_blob(argv[0]), size);
                sqlite3_set_auxdata(context, 0, data_copy, dummy_destructor);
            }
        }
    }
    
    /* Choose result type based on input */
    if (argc == 0) {
        sqlite3_result_null(context);
    } else {
        int size = sqlite3_value_bytes(argv[0]) % 256;
        if (size % 4 == 0) {
            sqlite3_result_null(context);
        } else if (size % 4 == 1) {
            const char *text = "test result";
            sqlite3_result_text(context, text, -1, SQLITE_TRANSIENT);
        } else if (size % 4 == 2) {
            double value = 3.14159;
            sqlite3_result_double(context, value);
        } else {
            const char *text = "alternate result";
            sqlite3_result_text(context, text, -1, SQLITE_TRANSIENT);
        }
    }
}

/* Dummy aggregate step function */
static void dummy_aggregate_step(sqlite3_context *context, int argc, sqlite3_value **argv) {
    /* Exercise sqlite3_aggregate_context - only allocate on first call */
    static int call_count = 0;
    call_count++;
    
    int size = 0;
    if (argc > 0) {
        size = sqlite3_value_bytes(argv[0]) % 128;
    }
    
    void *agg_ctx = sqlite3_aggregate_context(context, size);
    
    /* Update aggregate state if we have context */
    if (agg_ctx && size > 0) {
        int *count = (int *)agg_ctx;
        *count += 1;
    }
}

/* Dummy aggregate final function */
static void dummy_aggregate_final(sqlite3_context *context) {
    /* Get aggregate context without allocating */
    void *agg_ctx = sqlite3_aggregate_context(context, 0);
    
    if (agg_ctx) {
        int *count = (int *)agg_ctx;
        sqlite3_result_int(context, *count);
    } else {
        sqlite3_result_null(context);
    }
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *err_msg = NULL;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    /* Register scalar function */
    rc = sqlite3_create_function(
        db,
        "dummy_func",
        1,  /* Number of arguments */
        SQLITE_UTF8,
        &dummy_data,
        dummy_sql_function,
        NULL,
        NULL
    );
    
    /* Register aggregate function */
    rc = sqlite3_create_function(
        db,
        "dummy_agg",
        1,  /* Number of arguments */
        SQLITE_UTF8,
        &dummy_data,
        NULL,
        dummy_aggregate_step,
        dummy_aggregate_final
    );
    
    if (rc == SQLITE_OK) {
        /* Create a dummy table with the fuzzed data */
        const char *create_table = 
            "CREATE TABLE IF NOT EXISTS test_data (content BLOB);";
        rc = sqlite3_exec(db, create_table, NULL, NULL, &err_msg);
        
        if (rc == SQLITE_OK) {
            /* Insert fuzzed data into table */
            sqlite3_stmt *stmt;
            const char *insert = "INSERT INTO test_data (content) VALUES (?);";
            rc = sqlite3_prepare_v2(db, insert, -1, &stmt, NULL);
            
            if (rc == SQLITE_OK) {
                int data_size = (int)(Size > 1024 ? 1024 : Size);
                if (data_size > 0) {
                    sqlite3_bind_blob(stmt, 1, Data, data_size, SQLITE_STATIC);
                } else {
                    sqlite3_bind_null(stmt, 1);
                }
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
                
                /* Call the scalar function with fuzzed data */
                const char *select_scalar = "SELECT dummy_func(content) FROM test_data;";
                rc = sqlite3_exec(db, select_scalar, NULL, NULL, &err_msg);
                
                /* Call the aggregate function with fuzzed data */
                const char *select_agg = "SELECT dummy_agg(content) FROM test_data;";
                rc = sqlite3_exec(db, select_agg, NULL, NULL, &err_msg);
                
                /* Also test with empty input */
                const char *select_null = "SELECT dummy_func(NULL);";
                rc = sqlite3_exec(db, select_null, NULL, NULL, &err_msg);
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