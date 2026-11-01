// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_interrupt at sqlite3.c:187522:17 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
// sqlite3_update_hook at sqlite3.c:187987:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Static helper to safely open a database connection */
static sqlite3 *safe_open_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/* Static helper to create a simple table if it doesn't exist */
static void ensure_table(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data TEXT);";
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
}

/* Static helper to prepare a statement */
static sqlite3_stmt *safe_prepare(sqlite3 *db, const char *sql) {
    sqlite3_stmt *stmt = NULL;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    return stmt;
}

/* Update hook callback implementation */
static void update_hook_callback(void *arg, int op, const char *db_name, const char *table_name, sqlite3_int64 rowid) {
    /* Intentionally empty to avoid side effects during fuzzing */
    (void)arg;
    (void)op;
    (void)db_name;
    (void)table_name;
    (void)rowid;
}

/* Fuzzer entry point */
int LLVMFuzzerTestOneInput_85(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int timeout_val = 0;
    int insert_val = 0;
    
    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Extract values from input for fuzzing parameters */
    if (Size > 0) {
        timeout_val = (int)Data[0];
        if (Size > 1) {
            insert_val = (int)Data[1];
        }
    }

    /* 1. Prepare the environment: Open a database connection */
    db = safe_open_db(":memory:");
    if (!db) {
        return 0;
    }

    /* Ensure a table exists for operations */
    ensure_table(db);

    /* 2. Execute target API functions diversely */

    /* API: sqlite3_busy_timeout */
    /* Test with positive, negative, and zero values derived from input */
    rc = sqlite3_busy_timeout(db, timeout_val);
    (void)rc; /* Suppress unused variable warning */

    /* API: sqlite3_update_hook */
    /* Register a hook, then disable it, then register again */
    void *prev_hook = sqlite3_update_hook(db, update_hook_callback, (void *)(uintptr_t)insert_val);
    (void)prev_hook;
    
    /* Disable hook */
    prev_hook = sqlite3_update_hook(db, NULL, NULL);
    (void)prev_hook;

    /* Re-enable hook */
    prev_hook = sqlite3_update_hook(db, update_hook_callback, (void *)(uintptr_t)insert_val);
    (void)prev_hook;

    /* Prepare a statement to test sqlite3_db_handle and sqlite3_interrupt */
    const char *sql_select = "SELECT * FROM test_table;";
    stmt = safe_prepare(db, sql_select);
    
    if (stmt) {
        /* API: sqlite3_db_handle */
        sqlite3 *handle = sqlite3_db_handle(stmt);
        if (handle) {
            /* Verify handle matches db */
            if (handle != db) {
                /* This is an assertion failure state, but we continue */
            }
        }
        
        /* API: sqlite3_interrupt */
        /* Call interrupt on the connection associated with the statement */
        sqlite3_interrupt(handle);
        
        /* Step the statement to see if it returns SQLITE_INTERRUPT or completes */
        rc = sqlite3_step(stmt);
        (void)rc;
        
        /* Finalize the statement */
        sqlite3_finalize(stmt);
    }

    /* Create another statement for write operations to test sqlite3_db_cacheflush */
    const char *sql_insert = "INSERT INTO test_table (data) VALUES ('fuzz_data');";
    stmt = safe_prepare(db, sql_insert);
    
    if (stmt) {
        /* Start a transaction implicitly or explicitly to have dirty pages */
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        
        /* Step the insert */
        rc = sqlite3_step(stmt);
        
        /* API: sqlite3_db_cacheflush */
        /* This attempts to flush dirty pages to disk mid-transaction */
        rc = sqlite3_db_cacheflush(db);
        (void)rc;
        
        /* Commit the transaction */
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        
        sqlite3_finalize(stmt);
    }

    /* Test sqlite3_interrupt again when no statement is running (no-op case) */
    sqlite3_interrupt(db);

    /* 3. Cleanup: Close the database connection */
    /* Use sqlite3_close_v2 to handle pending statements gracefully */
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}