// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_extended_result_codes at sqlite3.c:189758:16 in sqlite3.h
// sqlite3_is_interrupted at sqlite3.c:187538:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * 1. sqlite3_wal_checkpoint_v2
 * 2. sqlite3_extended_result_codes
 * 3. sqlite3_is_interrupted
 * 4. sqlite3_errcode
 * 5. sqlite3_close_v2
 * 6. sqlite3_extended_errcode
 */

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int pnLog = 0;
    int pnCkpt = 0;
    const char *zDb = NULL;
    int eMode = 0;

    /*
     * Step 1: Prepare the environment.
     * We need a valid database connection. We use an in-memory database
     * to avoid file system dependencies.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If opening fails, we cannot proceed with valid db tests. */
        return 0;
    }

    /* Enable WAL mode to make sqlite3_wal_checkpoint_v2 meaningful */
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    /*
     * Step 2 & 3: Explore program states by invoking target functions diversely.
     * We use the input data to drive parameters like mode, database name, and flags.
     */

    /* 1. sqlite3_wal_checkpoint_v2 */
    /* Determine mode from input data */
    if (Size > 0) {
        int modeVal = Data[0] % 5; /* 0 to 4 */
        switch (modeVal) {
            case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
            case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
            case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
            case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
            case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
            default: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
        }
    } else {
        eMode = SQLITE_CHECKPOINT_PASSIVE;
    }

    /* Determine zDb (database name) from input if possible */
    /* Allocate memory on heap to avoid stack-use-after-scope */
    char *dbNameHeap = NULL;
    if (Size > 1) {
        /* Create a temporary string from input for zDb, ensuring null termination */
        size_t len = (Size > 10) ? 10 : Size - 1;
        dbNameHeap = (char *)malloc(len + 1);
        if (dbNameHeap) {
            memcpy(dbNameHeap, Data + 1, len);
            dbNameHeap[len] = '\0';
            zDb = dbNameHeap;
        }
    }

    rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
    (void)rc; /* Ignore return for now, just exercising the function */

    /* Free heap allocated string */
    if (dbNameHeap) {
        free(dbNameHeap);
    }

    /* 2. sqlite3_extended_result_codes */
    int enableExtended = 0;
    if (Size > 2) {
        enableExtended = (Data[2] % 2) == 1 ? 1 : 0;
    }
    rc = sqlite3_extended_result_codes(db, enableExtended);
    (void)rc;

    /* 3. sqlite3_is_interrupted */
    int interrupted = sqlite3_is_interrupted(db);
    (void)interrupted;

    /* 4. sqlite3_errcode */
    int errCode = sqlite3_errcode(db);
    (void)errCode;

    /* 6. sqlite3_extended_errcode */
    int extErrCode = sqlite3_extended_errcode(db);
    (void)extErrCode;

    /*
     * Step 2 (Cleanup): Close the database connection safely.
     */
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}