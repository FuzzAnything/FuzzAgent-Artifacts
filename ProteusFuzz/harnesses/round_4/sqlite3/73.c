// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to create a temporary database file with WAL mode enabled.
 * This is necessary because sqlite3_wal_checkpoint_v2 requires WAL mode.
 */
static int create_wal_db(const char *filename, sqlite3 **ppDb) {
    int rc;
    sqlite3 *db;

    rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        return rc;
    }

    /* Enable WAL mode to make sqlite3_wal_checkpoint_v2 meaningful */
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return rc;
    }

    /* Create a simple table and insert some data to ensure WAL activity */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return rc;
    }

    /* Insert a row to generate WAL frames */
    rc = sqlite3_exec(db, "INSERT INTO test (data) VALUES ('fuzz_data');", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return rc;
    }

    *ppDb = db;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int pnLog = 0;
    int pnCkpt = 0;
    const char *zDb = NULL;
    int eMode = 0;
    int autocommit;
    int total_changes;
    int txn_state;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Use a fixed filename for the fuzzing database */
    const char *filename = "./dummy_file";

    /* 
     * Step 1: Prepare environment.
     * We need a valid database connection in WAL mode for the checkpoint function.
     * We open the DB once per input to ensure a clean state for each fuzz iteration.
     */
    if (create_wal_db(filename, &db) != SQLITE_OK) {
        /* If we can't create the DB, we can't test the functions properly.
         * However, we can still test functions that don't strictly require WAL 
         * if we open a standard DB, but for consistency with the target API,
         * we skip this iteration if setup fails.
         */
        return 0;
    }

    /* 
     * Step 2: Parse input data to drive diverse behavior.
     * We use the input bytes to determine parameters for the API calls.
     * IMPORTANT: Always check Size before accessing Data[i] to prevent buffer overflows.
     */

    /* Determine checkpoint mode from input (0-4) */
    if (Size > 0) {
        eMode = Data[0] % 5;
        if (eMode == 0) eMode = SQLITE_CHECKPOINT_PASSIVE;
        else if (eMode == 1) eMode = SQLITE_CHECKPOINT_FULL;
        else if (eMode == 2) eMode = SQLITE_CHECKPOINT_RESTART;
        else if (eMode == 3) eMode = SQLITE_CHECKPOINT_TRUNCATE;
        else if (eMode == 4) eMode = SQLITE_CHECKPOINT_NOOP;
    }

    /* Determine database name for checkpoint (NULL or "main") */
    if (Size > 1) {
        zDb = (Data[1] % 2 == 0) ? "main" : NULL;
    }

    /* 
     * Step 3: Invoke target functions diversely.
     */

    /* Test sqlite3_wal_checkpoint_v2 */
    /* We pass pointers to pnLog and pnCkpt to get output values */
    rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
    (void)rc; /* Suppress unused variable warning, result is just for state exploration */

    /* Test sqlite3_db_cacheflush */
    /* This function forces dirty pages to disk. 
     * It works best when a transaction is active, but calling it outside is safe (no-op or error).
     * To explore more states, we might try to start a transaction first.
     */
    if (Size > 2) {
        if (Data[2] % 3 == 0) {
            /* Start a transaction to make cacheflush more interesting */
            sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        }
    }
    rc = sqlite3_db_cacheflush(db);
    (void)rc;

    /* Test sqlite3_get_autocommit */
    autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;

    /* Test sqlite3_total_changes */
    total_changes = sqlite3_total_changes(db);
    (void)total_changes;

    /* Test sqlite3_txn_state */
    /* Check state of "main" or NULL based on input */
    const char *schema = NULL;
    if (Size > 3) {
        schema = (Data[3] % 2 == 0) ? "main" : NULL;
    }
    txn_state = sqlite3_txn_state(db, schema);
    (void)txn_state;

    /* 
     * Step 4: Cleanup.
     * Close the database connection safely using sqlite3_close_v2.
     * This handles pending statements gracefully.
     */
    rc = sqlite3_close_v2(db);
    (void)rc;

    /* Clean up the dummy file if it exists to keep environment clean for next run */
    /* Note: In a real fuzzing harness, file cleanup might be handled externally, 
       but removing it here ensures isolation. */
    remove(filename);
    /* Also remove WAL and SHM files if they exist */
    remove("./dummy_file-wal");
    remove("./dummy_file-shm");

    return 0;
}