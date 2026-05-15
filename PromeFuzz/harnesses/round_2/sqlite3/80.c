// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_function_v2 at sqlite3.c:173011:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:79233:21 in sqlite3.h
// sqlite3_value_numeric_type at sqlite3.c:81591:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:78760:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:173243:18 in sqlite3.h
// sqlite3_create_function at sqlite3.c:172998:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Callback for sqlite3_update_hook */
static void update_hook_callback(void *arg, int op, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
    /* Use the arguments to avoid compiler warnings about unused parameters */
    (void)arg;
    (void)op;
    (void)db_name;
    (void)table_name;
    (void)rowid;
    /* Do nothing - just a placeholder */
}

/* Scalar function implementation */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Get database handle from context */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    (void)db; /* Use to avoid warning */
    
    /* Check numeric type of each argument */
    for (int i = 0; i < argc; i++) {
        int type = sqlite3_value_numeric_type(argv[i]);
        (void)type; /* Use to avoid warning */
    }
    
    /* Return a simple integer result */
    sqlite3_result_int(ctx, 42);
}

/* Aggregate step function */
static void aggregate_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    /* Similar to scalar function but for aggregate */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    (void)db;
    
    for (int i = 0; i < argc; i++) {
        int type = sqlite3_value_numeric_type(argv[i]);
        (void)type;
    }
}

/* Aggregate final function */
static void aggregate_final(sqlite3_context *ctx) {
    sqlite3_result_int(ctx, 100);
}

/* Destructor for sqlite3_create_function_v2 */
static void destroy_func(void *arg) {
    (void)arg;
    /* Cleanup if needed */
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use the fuzz data to create function names and parameters */
    if (Size > 0) {
        /* Create a null-terminated function name from the data */
        char func_name[256];
        size_t name_len = Size < 255 ? Size : 255;
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';
        
        /* Extract parameters from data */
        int nArg = (Size > 0) ? (Data[0] % 10) : 1; /* 0-9 arguments */
        int eTextRep = (Size > 1) ? (Data[1] % 4) : SQLITE_UTF8;
        int use_deterministic = (Size > 2) ? (Data[2] & 1) : 0;
        int use_directonly = (Size > 2) ? (Data[2] & 2) : 0;
        
        /* Apply flags */
        if (use_deterministic) eTextRep |= SQLITE_DETERMINISTIC;
        if (use_directonly) eTextRep |= SQLITE_DIRECTONLY;
        
        /* Test sqlite3_update_hook */
        void *prev_arg = sqlite3_update_hook(db, update_hook_callback, (void*)0x1234);
        (void)prev_arg;
        
        /* Disable the hook */
        sqlite3_update_hook(db, NULL, NULL);
        
        /* Test sqlite3_create_function */
        rc = sqlite3_create_function(
            db,
            func_name,
            nArg,
            eTextRep,
            (void*)0x5678,
            scalar_func,
            NULL,
            NULL
        );
        (void)rc;
        
        /* Test sqlite3_create_function_v2 */
        rc = sqlite3_create_function_v2(
            db,
            "agg_func",
            nArg,
            eTextRep,
            (void*)0x9ABC,
            NULL,
            aggregate_step,
            aggregate_final,
            destroy_func
        );
        (void)rc;
        
        /* Test sqlite3_overload_function */
        rc = sqlite3_overload_function(db, "overloaded_func", nArg);
        (void)rc;
        
        /* Execute a simple SQL statement to trigger the update hook */
        char *err_msg = NULL;
        sqlite3_exec(db, "CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT)", NULL, NULL, &err_msg);
        sqlite3_exec(db, "INSERT INTO test(name) VALUES('test')", NULL, NULL, &err_msg);
        sqlite3_exec(db, "UPDATE test SET name='updated' WHERE id=1", NULL, NULL, &err_msg);
        sqlite3_exec(db, "DELETE FROM test WHERE id=1", NULL, NULL, &err_msg);
        
        if (err_msg) {
            sqlite3_free(err_msg);
        }
        
        /* Call the scalar function via SQL */
        sqlite3_stmt *stmt;
        if (sqlite3_prepare_v2(db, "SELECT scalar_func(?)", -1, &stmt, NULL) == SQLITE_OK) {
            /* Bind some data from the fuzz input */
            if (Size > 10) {
                sqlite3_bind_blob(stmt, 1, Data, Size > 100 ? 100 : Size, SQLITE_STATIC);
            }
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}