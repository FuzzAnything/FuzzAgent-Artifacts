// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:188242:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:190608:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target APIs:
 * 1. sqlite3_db_name
 * 2. sqlite3_wal_checkpoint
 * 3. sqlite3_db_readonly
 * 4. sqlite3_wal_checkpoint_v2
 * 5. sqlite3_txn_state
 * 6. sqlite3_close_v2
 */

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Ensure we have enough data to create a valid filename and some SQL */
    if (Size < 2) {
        return 0;
    }

    /* Create a temporary database file path from input data */
    /* We use a fixed name to ensure consistency, but we could also use input data */
    const char *db_path = "./dummy_file";

    /* Open the database */
    rc = sqlite3_open(db_path, &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If we cannot open the database, we cannot test the APIs */
        return 0;
    }

    /* Initialize the database with a simple table to ensure WAL mode works */
    const char *init_sql = "PRAGMA journal_mode=WAL; CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);";
    char *err_msg = NULL;
    rc = sqlite3_exec(db, init_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close_v2(db);
        return 0;
    }

    /* Extract parameters from fuzz input */
    int idx_n = (Size > 0) ? Data[0] % 10 : 0; /* N for sqlite3_db_name, sqlite3_db_readonly */
    int idx_mode = (Size > 1) ? Data[1] % 5 : 0; /* Mode for sqlite3_wal_checkpoint_v2 */
    int idx_schema_len = (Size > 2) ? Data[2] % (Size - 2) : 0; /* Length of schema name */
    
    /* Determine schema name */
    const char *schema_name = NULL;
    if (idx_schema_len > 0 && idx_schema_len < Size - 2) {
        schema_name = (const char *)(Data + 3);
    } else {
        schema_name = "main";
    }

    /* 1. Test sqlite3_db_name */
    const char *name = sqlite3_db_name(db, idx_n);
    if (name != NULL) {
        /* Just to ensure we use the result */
        (void)strlen(name);
    }

    /* 2. Test sqlite3_wal_checkpoint */
    rc = sqlite3_wal_checkpoint(db, schema_name);
    (void)rc;

    /* 3. Test sqlite3_db_readonly */
    int is_readonly = sqlite3_db_readonly(db, schema_name);
    (void)is_readonly;

    /* 4. Test sqlite3_wal_checkpoint_v2 */
    int eMode = SQLITE_CHECKPOINT_PASSIVE;
    switch (idx_mode) {
        case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
        case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
        case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
        case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
        case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
    }

    int pnLog = 0;
    int pnCkpt = 0;
    rc = sqlite3_wal_checkpoint_v2(db, schema_name, eMode, &pnLog, &pnCkpt);
    (void)rc;
    (void)pnLog;
    (void)pnCkpt;

    /* 5. Test sqlite3_txn_state */
    int txn_state = sqlite3_txn_state(db, schema_name);
    (void)txn_state;

    /* Also test with NULL schema to check highest state */
    txn_state = sqlite3_txn_state(db, NULL);
    (void)txn_state;

    /* 6. Test sqlite3_close_v2 */
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}