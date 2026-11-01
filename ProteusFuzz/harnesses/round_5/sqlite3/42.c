// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_total_changes at sqlite3.c:186791:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:186554:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Targets:
 *  1. sqlite3_total_changes
 *  2. sqlite3_txn_state
 *  3. sqlite3_close_v2
 *  4. sqlite3_db_cacheflush
 *  5. sqlite3_get_autocommit
 *  6. sqlite3_wal_checkpoint_v2
 */

int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *sql = NULL;
    size_t sqlLen = 0;
    int eMode = 0;
    int pnLog = 0, pnCkpt = 0;

    /*
     * Step 1: Prepare environment.
     * Open a database connection. We use ":memory:" to avoid file I/O issues
     * and ensure a clean state for each input.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Use input data to drive SQL generation and API parameters.
     * We limit the SQL length to prevent excessive memory usage or stack overflow.
     */
    if (Size > 0) {
        /* Determine SQL length safely */
        sqlLen = (Size > 1024) ? 1024 : Size;
        sql = (char *)malloc(sqlLen + 1);
        if (sql) {
            memcpy(sql, Data, sqlLen);
            sql[sqlLen] = '\0';
        }
    }

    /*
     * Step 2: Execute diverse SQL to trigger different states (transactions, changes, etc.)
     * This helps exercise sqlite3_total_changes, sqlite3_txn_state, etc.
     */
    if (sql && sqlLen > 0) {
        /* Try to execute the input as SQL */
        rc = sqlite3_exec(db, sql, NULL, NULL, NULL);
        (void)rc; /* Ignore errors, we want to fuzz various states */
    }

    /*
     * Step 3: Explore program states by invoking target functions diversely.
     */

    /* 1. sqlite3_total_changes */
    /* Returns cumulative count of rows modified. */
    (void)sqlite3_total_changes(db);

    /* 2. sqlite3_txn_state */
    /* Check transaction state with NULL (all schemas) */
    (void)sqlite3_txn_state(db, NULL);
    /* Check with specific schema names derived from input or hardcoded */
    if (Size > 0) {
        /* Use first byte as a seed for schema name selection */
        if ((Data[0] % 3) == 0) {
            (void)sqlite3_txn_state(db, "main");
        } else if ((Data[0] % 3) == 1) {
            (void)sqlite3_txn_state(db, "temp");
        } else {
            /* Try an invalid schema name to test error handling */
            (void)sqlite3_txn_state(db, "invalid_schema_name");
        }
    }

    /* 3. sqlite3_close_v2 */
    /* We will close at the end, but we can try to close and reopen to test state transitions.
     * However, closing and reopening mid-fuzz might be too destructive for a single input.
     * Instead, we rely on the final cleanup.
     * To test the function logic specifically, we could try to close, but we need the db for other calls.
     * So we defer closing to the end.
     */

    /* 4. sqlite3_db_cacheflush */
    /* Forces dirty pages to disk. Requires a write transaction. */
    /* We try to execute an INSERT/UPDATE to ensure a transaction might be open or pages dirty. */
    if (sql && sqlLen > 0) {
        /* Re-run a simple write command to ensure state */
        sqlite3_exec(db, "BEGIN; INSERT INTO dummy VALUES(1);", NULL, NULL, NULL);
    }
    (void)sqlite3_db_cacheflush(db);

    /* 5. sqlite3_get_autocommit */
    /* Checks if in autocommit mode. */
    (void)sqlite3_get_autocommit(db);

    /* 6. sqlite3_wal_checkpoint_v2 */
    /* Checkpoint WAL. Try different modes based on input. */
    if (Size > 0) {
        int modeSeed = Data[0] % 5;
        switch (modeSeed) {
            case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
            case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
            case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
            case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
            case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
            default: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
        }
        /* Try with NULL (all databases) and "main" */
        (void)sqlite3_wal_checkpoint_v2(db, NULL, eMode, &pnLog, &pnCkpt);
        (void)sqlite3_wal_checkpoint_v2(db, "main", eMode, &pnLog, &pnCkpt);
        /* Try with invalid schema */
        (void)sqlite3_wal_checkpoint_v2(db, "non_existent", eMode, &pnLog, &pnCkpt);
    }

    /*
     * Step 4: Cleanup.
     * Close the database connection safely.
     */
    if (db) {
        rc = sqlite3_close_v2(db);
        (void)rc;
    }

    /* Free allocated memory */
    if (sql) {
        free(sql);
    }

    return 0;
}