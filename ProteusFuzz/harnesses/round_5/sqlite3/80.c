// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Static helper function to safely open a database connection.
 * Uses a dummy file as per instructions.
 */
static sqlite3 *open_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * WAL Hook callback implementation.
 * Required for sqlite3_wal_hook.
 */
static int wal_callback(void *arg, sqlite3 *db, const char *db_name, int page_count) {
    (void)arg;
    (void)db;
    (void)db_name;
    (void)page_count;
    return SQLITE_OK;
}

/*
 * Fuzzing entry point.
 */
int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = open_db();
    if (!db) {
        return 0;
    }

    /*
     * Prepare a simple statement to test sqlite3_db_handle.
     * We use a fixed SQL string to ensure valid input for prepare,
     * but we could also fuzz the SQL if we had a larger buffer.
     */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        sqlite3_close_v2(db);
        return 0;
    }

    /*
     * 1. Test sqlite3_db_config
     * We try to configure lookaside memory.
     * We use data from the fuzz input to determine arguments, with bounds checking.
     */
    if (Size >= 2) {
        int op = SQLITE_DBCONFIG_LOOKASIDE;
        int sz = (Data[0] % 100) + 1;
        int n = (Data[1] % 10) + 1;
        /* sqlite3_db_config expects: db, op, void*, int, int */
        void *buf = NULL; 
        sqlite3_db_config(db, op, buf, sz, n);
        
        /* Try another op code if available in data */
        if (Size > 2) {
            int op2 = Data[2];
            /* Only pass valid op codes or let SQLite handle invalid ones gracefully */
            sqlite3_db_config(db, op2, NULL, 0, 0);
        }
    }

    /*
     * 2. Test sqlite3_wal_hook
     * Register the callback with user data derived from input.
     */
    void *user_data = (void *)(uintptr_t)(Data[0]);
    void *prev_data = sqlite3_wal_hook(db, wal_callback, user_data);
    (void)prev_data;

    /* Try disabling the hook */
    sqlite3_wal_hook(db, NULL, NULL);

    /*
     * 3. Test sqlite3_busy_timeout
     * Use fuzz data to set timeout.
     */
    if (Size > 3) {
        int timeout_ms = (int)Data[3];
        /* Clamp to reasonable range to avoid hanging the fuzzer indefinitely */
        if (timeout_ms > 1000) timeout_ms = 1000;
        if (timeout_ms < 0) timeout_ms = 0;
        sqlite3_busy_timeout(db, timeout_ms);
    }

    /*
     * 4. Test sqlite3_errcode
     * Trigger an error first to get a meaningful code, or just check current state.
     * We prepare an invalid SQL to force an error.
     */
    sqlite3_stmt *bad_stmt = NULL;
    if (sqlite3_prepare_v2(db, "INVALID SQL SYNTAX", -1, &bad_stmt, NULL) != SQLITE_OK) {
        int err = sqlite3_errcode(db);
        (void)err;
    }
    
    /* Check error code on the valid statement too */
    int err_valid = sqlite3_errcode(db);
    (void)err_valid;

    /*
     * 5. Test sqlite3_db_handle
     * Retrieve the db handle from the prepared statement.
     */
    sqlite3 *retrieved_db = sqlite3_db_handle(stmt);
    if (retrieved_db != db) {
        /* This should not happen, but we check for logic */
        sqlite3_finalize(stmt);
        sqlite3_close_v2(db);
        return 0;
    }

    /*
     * Cleanup
     * Finalize statement
     */
    sqlite3_finalize(stmt);
    if (bad_stmt) {
        sqlite3_finalize(bad_stmt);
    }

    /* Close database */
    sqlite3_close_v2(db);

    return 0;
}