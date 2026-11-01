// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_set_authorizer at sqlite3.c:124753:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_profile at sqlite3.c:187932:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_context_db_handle at sqlite3.c:93240:21 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Callback for sqlite3_update_hook */
static void update_callback(void *pArg, int op, const char *db, const char *tbl, sqlite3_int64 rowid) {
    (void)pArg;
    (void)op;
    (void)db;
    (void)tbl;
    (void)rowid;
}

/* Callback for sqlite3_progress_handler */
static int progress_callback(void *pArg) {
    (void)pArg;
    return 0;
}

/* Callback for sqlite3_set_authorizer */
static int authorizer_callback(void *pArg, int actionCode, const char *p1, const char *p2, const char *p3, const char *p4) {
    (void)pArg;
    (void)actionCode;
    (void)p1;
    (void)p2;
    (void)p3;
    (void)p4;
    return SQLITE_OK;
}

/* Callback for sqlite3_profile */
static void profile_callback(void *pArg, const char *zSql, sqlite3_uint64 ns) {
    (void)pArg;
    (void)zSql;
    (void)ns;
}

/* Dummy SQL function to test sqlite3_context_db_handle */
static void dummy_sql_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    /* Access the db handle inside the function context */
    sqlite3 *db = sqlite3_context_db_handle(ctx);
    if (db) {
        /* Just a sanity check, do not modify db here */
        (void)db;
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Use a dummy file or memory database. Memory is safer for fuzzing to avoid disk issues. */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to ensure we have a rowid table for update_hook testing */
    const char *create_sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    /* 2. Invoke target functions diversely using input data */
    
    /* Safely access input data with bounds checking */
    uint8_t val0 = (Size > 0) ? Data[0] : 0;
    uint8_t val1 = (Size > 1) ? Data[1] : 0;
    uint8_t val2 = (Size > 2) ? Data[2] : 0;
    uint8_t val3 = (Size > 3) ? Data[3] : 0;
    uint8_t val4 = (Size > 4) ? Data[4] : 0;

    int enable_update = (val0 % 2);
    int enable_progress = (val1 % 2);
    int enable_authorizer = (val2 % 2);
    int enable_profile = (val3 % 2);

    /* Test sqlite3_update_hook */
    if (enable_update) {
        void *prev = sqlite3_update_hook(db, update_callback, (void *)(intptr_t)val0);
        (void)prev; /* Avoid unused variable warning */
        /* Trigger the hook by modifying the table */
        sqlite3_exec(db, "INSERT INTO test_table (data) VALUES ('fuzz');", NULL, NULL, NULL);
        sqlite3_exec(db, "UPDATE test_table SET data='updated' WHERE id=1;", NULL, NULL, NULL);
        sqlite3_exec(db, "DELETE FROM test_table WHERE id=1;", NULL, NULL, NULL);
        /* Disable it */
        sqlite3_update_hook(db, NULL, NULL);
    } else {
        sqlite3_update_hook(db, NULL, NULL);
    }

    /* Test sqlite3_progress_handler */
    if (enable_progress) {
        int n_ops = (val4 % 100) + 1;
        if (n_ops < 1) n_ops = 1;
        sqlite3_progress_handler(db, n_ops, progress_callback, (void *)(intptr_t)val4);
        /* Run a query that might trigger progress handler */
        sqlite3_exec(db, "SELECT * FROM test_table;", NULL, NULL, NULL);
        /* Disable */
        sqlite3_progress_handler(db, 0, NULL, NULL);
    } else {
        sqlite3_progress_handler(db, 0, NULL, NULL);
    }

    /* Test sqlite3_set_authorizer */
    if (enable_authorizer) {
        rc = sqlite3_set_authorizer(db, authorizer_callback, (void *)(intptr_t)val2);
        if (rc != SQLITE_OK) {
            /* Handle error gracefully */
        }
        /* Try to prepare a statement to trigger authorizer */
        const char *auth_sql = "SELECT * FROM test_table;";
        sqlite3_prepare_v2(db, auth_sql, -1, &stmt, NULL);
        if (stmt) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        /* Disable authorizer */
        sqlite3_set_authorizer(db, NULL, NULL);
    } else {
        sqlite3_set_authorizer(db, NULL, NULL);
    }

    /* Test sqlite3_profile */
    if (enable_profile) {
        void *prev = sqlite3_profile(db, profile_callback, (void *)(intptr_t)val3);
        (void)prev;
        /* Run a query to trigger profile */
        sqlite3_exec(db, "SELECT count(*) FROM test_table;", NULL, NULL, NULL);
        /* Disable */
        sqlite3_profile(db, NULL, NULL);
    } else {
        sqlite3_profile(db, NULL, NULL);
    }

    /* Test sqlite3_db_handle */
    /* Prepare a statement to get a valid stmt handle */
    const char *stmt_sql = "SELECT 1;";
    rc = sqlite3_prepare_v2(db, stmt_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3 *handle = sqlite3_db_handle(stmt);
        if (handle == db) {
            /* Success */
        }
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Test sqlite3_context_db_handle */
    /* Register a custom function to trigger the context handle retrieval */
    sqlite3_create_function(db, "fuzz_test_func", 0, SQLITE_UTF8, NULL, dummy_sql_func, NULL, NULL);
    /* Call the function */
    sqlite3_exec(db, "SELECT fuzz_test_func();", NULL, NULL, NULL);
    /* Remove the function */
    sqlite3_create_function(db, "fuzz_test_func", 0, SQLITE_UTF8, NULL, NULL, NULL, NULL);

    /* 3. Cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    return 0;
}