// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_txn_state at sqlite3.c:186941:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_wal_checkpoint_v2, sqlite3_is_interrupted,
 * sqlite3_extended_errcode, sqlite3_set_errmsg, sqlite3_errcode,
 * and sqlite3_txn_state.
 */

int LLVMFuzzerTestOneInput_62(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int logFrames = -1;
    int ckptFrames = -1;

    /* Create a temporary in-memory database to avoid file I/O issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Enable WAL mode to make checkpointing meaningful */
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    /* Optionally create a table and insert some data to have frames in WAL */
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO test (data) VALUES (randomblob(100));", NULL, NULL, NULL);

    /*
     * Fuzz sqlite3_wal_checkpoint_v2
     * Modes: 0=PASSIVE, 1=FULL, 2=RESTART, 3=TRUNCATE, 4=NOOP
     * We try different modes and database names.
     */
    if (Size > 0) {
        int mode = Data[0] % 5; /* 0 to 4 */
        const char *zDb = NULL;
        
        if (Size > 1) {
            /* Use a substring as the database name if available, or NULL */
            size_t len = Size - 1;
            if (len > 64) len = 64;
            char db_name[65];
            memcpy(db_name, &Data[1], len);
            db_name[len] = '\0';
            
            /* Only use valid schema names (main, temp, or attached) to avoid SQLITE_ERROR */
            /* For fuzzing, we try "main" and "test" (if attached) but mostly rely on NULL */
            if (len == 4 && memcmp(db_name, "main", 4) == 0) {
                zDb = "main";
            } else if (len == 4 && memcmp(db_name, "temp", 4) == 0) {
                zDb = "temp";
            } else {
                zDb = NULL;
            }
        }

        rc = sqlite3_wal_checkpoint_v2(db, zDb, mode, &logFrames, &ckptFrames);
        (void)rc; /* Suppress unused warning */
    }

    /*
     * Fuzz sqlite3_is_interrupted
     * This is safe to call anytime.
     */
    int interrupted = sqlite3_is_interrupted(db);
    (void)interrupted;

    /*
     * Fuzz sqlite3_errcode and sqlite3_extended_errcode
     * These are safe to call even if no error occurred.
     */
    int errCode = sqlite3_errcode(db);
    int extErrCode = sqlite3_extended_errcode(db);
    (void)errCode;
    (void)extErrCode;

    /*
     * Fuzz sqlite3_set_errmsg
     * We try to set various error codes and messages from the input data.
     */
    if (Size > 2) {
        int customErrCode = (int)(Data[2]);
        /* Ensure we don't pass garbage as a string if we want to be safe, 
           but for fuzzing, we can try to pass a pointer into the buffer if it's null-terminated. */
        const char *msg = NULL;
        if (Size > 10) {
            /* Find a null terminator or use a fixed length substring */
            size_t mLen = 0;
            for (size_t i = 3; i < Size && i < 100; i++) {
                if (Data[i] == 0) break;
                mLen++;
            }
            if (mLen > 0) {
                char *safeMsg = malloc(mLen + 1);
                if (safeMsg) {
                    memcpy(safeMsg, &Data[3], mLen);
                    safeMsg[mLen] = '\0';
                    msg = safeMsg;
                    rc = sqlite3_set_errmsg(db, customErrCode, msg);
                    free(safeMsg);
                }
            }
        }
        if (!msg) {
            rc = sqlite3_set_errmsg(db, customErrCode, NULL);
        }
        (void)rc;
    }

    /*
     * Fuzz sqlite3_txn_state
     * Try with NULL and with "main" schema.
     */
    int txnState = sqlite3_txn_state(db, NULL);
    (void)txnState;

    if (Size > 5) {
        /* Try to use a schema name from input if it looks like "main" or "temp" */
        size_t sLen = 0;
        for (size_t i = 5; i < Size && i < 10; i++) {
            if (Data[i] == 0) break;
            sLen++;
        }
        if (sLen > 0) {
            char schema[11];
            memcpy(schema, &Data[5], sLen);
            schema[sLen] = '\0';
            txnState = sqlite3_txn_state(db, schema);
            (void)txnState;
        }
    }

    /* Cleanup */
    sqlite3_close(db);

    return 0;
}