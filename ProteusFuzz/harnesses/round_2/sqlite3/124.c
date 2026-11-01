// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_rollback_hook at sqlite3.c:188012:18 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_autovacuum_pages at sqlite3.c:188064:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

/*
 * Static helper functions to avoid name conflicts and manage state.
 */

/*
 * Callback function for sqlite3_rollback_hook
 */
static void dummy_rollback_callback(void *pArg) {
    (void)pArg;
    /* Do nothing */
}

/*
 * Callback function for sqlite3_autovacuum_pages
 * Returns the number of free pages to remove.
 */
static unsigned int dummy_autovac_callback(
    void *pClientData,
    const char *zSchema,
    unsigned int nDbPage,
    unsigned int nFreePage,
    unsigned int nBytePerPage) {
    (void)pClientData;
    (void)zSchema;
    (void)nDbPage;
    (void)nBytePerPage;
    /* Return all free pages or 0 based on a simple heuristic */
    return nFreePage > 0 ? nFreePage : 0;
}

/*
 * Destructor for autovacuum client data
 */
static void dummy_autovac_destructor(void *pArg) {
    (void)pArg;
    /* Do nothing */
}

/*
 * Fuzz driver entry point
 */
int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int wal_checkpoint_val;
    int autovac_pages_val;
    void *rollback_arg;
    const char *schema_name;
    int txn_state;

    /* Guard against empty inputs */
    if (Size == 0) {
        return 0;
    }

    /*
     * Step 1: Prepare the environment.
     * Initialize SQLite and open a database connection.
     * We use a file-based database to ensure WAL and autovacuum features
     * are potentially available, but we can also use :memory: if needed.
     * Using a dummy file as requested.
     */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If we cannot open the database, we cannot test the APIs. */
        return 0;
    }

    /*
     * Ensure WAL mode is enabled to make sqlite3_wal_autocheckpoint relevant.
     * This helps explore more program states.
     */
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
    /* Enable autovacuum to make sqlite3_autovacuum_pages relevant */
    sqlite3_exec(db, "PRAGMA auto_vacuum=FULL;", NULL, NULL, NULL);

    /*
     * Step 2: Parse input data to drive the fuzzing logic.
     * We interpret the input bytes to determine which functions to call
     * and with what arguments.
     */

    /* Extract values from input data safely */
    int idx = 0;
    uint8_t op = (Size > idx) ? Data[idx++] : 0;

    /*
     * 1. Test sqlite3_rollback_hook
     * Signature: void *sqlite3_rollback_hook(sqlite3 *, void (*)(void *), void *);
     */
    if (op == 0 || op == 1) {
        void *arg = (void *)(size_t)(Size > idx ? Data[idx++] : 0);
        /* Call with a callback */
        rollback_arg = sqlite3_rollback_hook(db, dummy_rollback_callback, arg);
        (void)rollback_arg; /* Suppress unused warning */

        /* Call with NULL to disable hook */
        rollback_arg = sqlite3_rollback_hook(db, NULL, NULL);
        (void)rollback_arg;
    }

    /*
     * 2. Test sqlite3_wal_autocheckpoint
     * Signature: int sqlite3_wal_autocheckpoint(sqlite3 *db, int N);
     */
    if (op == 0 || op == 2) {
        if (Size > idx) {
            wal_checkpoint_val = (int)(Data[idx++] & 0xFF);
            /* Try positive, negative, and zero values */
            rc = sqlite3_wal_autocheckpoint(db, wal_checkpoint_val);
            rc = sqlite3_wal_autocheckpoint(db, -wal_checkpoint_val);
            rc = sqlite3_wal_autocheckpoint(db, 0);
            rc = sqlite3_wal_autocheckpoint(db, 1000); /* Default */
        }
    }

    /*
     * 3. Test sqlite3_autovacuum_pages
     * Signature: int sqlite3_autovacuum_pages(sqlite3 *, callback, void *, destructor);
     */
    if (op == 0 || op == 3) {
        void *client_data = (void *)(size_t)(Size > idx ? Data[idx++] : 0);
        void (*destructor)(void *) = (Size > idx && Data[idx] == 1) ? dummy_autovac_destructor : NULL;
        idx++; /* consume byte for destructor flag */

        /* Register callback */
        rc = sqlite3_autovacuum_pages(db, dummy_autovac_callback, client_data, destructor);
        
        /* Cancel callback */
        rc = sqlite3_autovacuum_pages(db, NULL, NULL, NULL);
    }

    /*
     * 4. Test sqlite3_db_cacheflush
     * Signature: int sqlite3_db_cacheflush(sqlite3*);
     * Requires a write transaction to be open to be effective.
     */
    if (op == 0 || op == 4) {
        /* Start a write transaction to ensure the function has something to do */
        sqlite3_exec(db, "BEGIN IMMEDIATE;", NULL, NULL, NULL);
        
        /* Create some dirty pages */
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS fuzz_test (id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, NULL);
        sqlite3_exec(db, "INSERT INTO fuzz_test (data) VALUES ('test_data');", NULL, NULL, NULL);

        rc = sqlite3_db_cacheflush(db);
        (void)rc;

        /* Commit to clean up */
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }

    /*
     * 5. Test sqlite3_txn_state
     * Signature: int sqlite3_txn_state(sqlite3 *, const char *zSchema);
     */
    if (op == 0 || op == 5) {
        /* Test with NULL schema (all schemas) */
        txn_state = sqlite3_txn_state(db, NULL);
        (void)txn_state;

        /* Test with specific schema names */
        txn_state = sqlite3_txn_state(db, "main");
        txn_state = sqlite3_txn_state(db, "temp");
        
        /* Test with invalid schema name to trigger -1 return */
        txn_state = sqlite3_txn_state(db, "invalid_schema_name_xyz");
        (void)txn_state;

        /* Test during a transaction */
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        txn_state = sqlite3_txn_state(db, "main");
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }

    /*
     * Step 3: Cleanup.
     * Close the database connection safely using sqlite3_close_v2.
     */
    if (db != NULL) {
        rc = sqlite3_close_v2(db);
        (void)rc;
    }

    return 0;
}