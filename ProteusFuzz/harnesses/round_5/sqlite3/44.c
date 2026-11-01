// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:188242:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:190608:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *zDbName;
    int txnState;
    int logSize, ckptSize;
    int readOnly;
    int dbIndex;
    const char *schemaName;

    if (Size < 2) {
        return 0;
    }

    // Use the first byte to determine the database type (memory vs file)
    // Use the second byte to select the operation type
    int opType = Data[1] % 7;

    // Create a temporary file if needed
    const char *dbPath = "./dummy_file";
    FILE *f = fopen(dbPath, "w");
    if (f) {
        if (Size > 2) {
            fwrite(Data + 2, 1, Size - 2, f);
        }
        fclose(f);
    }

    // Open database
    if (Data[0] & 0x01) {
        // Memory database
        rc = sqlite3_open(":memory:", &db);
    } else {
        // File database
        rc = sqlite3_open(dbPath, &db);
    }

    if (rc != SQLITE_OK || db == NULL) {
        if (f) fclose(f);
        return 0;
    }

    // Ensure WAL mode for checkpoint tests
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    // Create a simple table to allow transactions
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, val TEXT);", NULL, NULL, NULL);

    // Generate a schema name from input if valid
    if (Size > 10) {
        size_t len = Size - 10;
        if (len > 100) len = 100;
        char buf[101];
        memcpy(buf, Data + 10, len);
        buf[len] = '\0';
        schemaName = buf;
    } else {
        schemaName = "main";
    }

    switch (opType) {
        case 0:
            // Test sqlite3_txn_state
            txnState = sqlite3_txn_state(db, schemaName);
            (void)txnState;
            break;

        case 1:
            // Test sqlite3_wal_checkpoint
            rc = sqlite3_wal_checkpoint(db, schemaName);
            (void)rc;
            break;

        case 2:
            // Test sqlite3_db_name
            // Use a byte from input to select index
            dbIndex = (Size > 5) ? (Data[5] % 10) : 0;
            zDbName = sqlite3_db_name(db, dbIndex);
            if (zDbName) {
                // Consume the string to avoid unused warning
                (void)strlen(zDbName);
            }
            break;

        case 3:
            // Test sqlite3_db_readonly
            readOnly = sqlite3_db_readonly(db, schemaName);
            (void)readOnly;
            break;

        case 4:
            // Test sqlite3_wal_checkpoint_v2 with various modes
            int mode = (Size > 6) ? (Data[6] % 5) : SQLITE_CHECKPOINT_PASSIVE;
            logSize = -1;
            ckptSize = -1;
            rc = sqlite3_wal_checkpoint_v2(db, schemaName, mode, &logSize, &ckptSize);
            (void)rc;
            break;

        case 5:
            // Test sqlite3_wal_checkpoint_v2 with NULL schema (all databases)
            logSize = -1;
            ckptSize = -1;
            rc = sqlite3_wal_checkpoint_v2(db, NULL, SQLITE_CHECKPOINT_PASSIVE, &logSize, &ckptSize);
            (void)rc;
            break;

        case 6:
            // Test sqlite3_txn_state with NULL schema
            txnState = sqlite3_txn_state(db, NULL);
            (void)txnState;
            break;
    }

    // Cleanup
    sqlite3_close_v2(db);
    
    // Remove dummy file
    remove(dbPath);

    return 0;
}