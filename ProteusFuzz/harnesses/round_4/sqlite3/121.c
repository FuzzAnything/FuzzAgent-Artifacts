// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:188242:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:188242:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_file_control at sqlite3.c:189771:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:190608:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:190574:24 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_open
 * - sqlite3_wal_checkpoint
 * - sqlite3_wal_checkpoint_v2
 * - sqlite3_file_control
 * - sqlite3_db_readonly
 * - sqlite3_db_name
 *
 * The driver uses the fuzzing input to drive various parameters and modes
 * to explore different execution paths and error conditions.
 */

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = NULL;
    int rc;
    int logSize = 0;
    int ckptSize = 0;
    int mode;
    int readonly;
    const char *dbName;
    int dbIndex;
    int fileControlOp;
    void *fileControlArg = NULL;
    const char *zDbName = NULL;
    const char *checkpointDbName = NULL;

    /* Create a dummy file for database operations if needed */
    const char *dbFilename = "./dummy_file";

    /* Use input data to derive various parameters */
    unsigned char *input = (unsigned char *)Data;
    size_t idx = 0;

    /* Determine mode for checkpoint operations */
    if (idx < Size) {
        mode = input[idx] % 5; /* 0-4: PASSIVE, FULL, RESTART, TRUNCATE, NOOP */
        idx++;
    } else {
        mode = 0;
    }

    /* Determine database name to use (NULL, "main", or a string from input) */
    char *tempName = NULL;
    if (idx < Size) {
        if (input[idx] % 3 == 0) {
            zDbName = NULL;
            idx++;
        } else if (input[idx] % 3 == 1) {
            zDbName = "main";
            idx++;
        } else {
            /* Use a substring from input as database name */
            size_t nameLen = (idx < Size) ? (input[idx] % 10) + 1 : 1;
            idx++;
            if (idx + nameLen <= Size) {
                tempName = malloc(nameLen + 1);
                if (tempName) {
                    memcpy(tempName, &input[idx], nameLen);
                    tempName[nameLen] = '\0';
                    zDbName = tempName;
                    idx += nameLen;
                } else {
                    zDbName = "main";
                }
            } else {
                zDbName = "main";
            }
        }
    } else {
        zDbName = "main";
    }

    checkpointDbName = (zDbName) ? zDbName : "main";

    /* Determine database index for sqlite3_db_name */
    if (idx < Size) {
        dbIndex = input[idx] % 5; /* 0-4 */
        idx++;
    } else {
        dbIndex = 0;
    }

    /* Determine file control opcode */
    if (idx < Size) {
        fileControlOp = input[idx] % 10; /* Use a range of opcodes */
        idx++;
    } else {
        fileControlOp = 0;
    }

    /* Open the database */
    rc = sqlite3_open(dbFilename, &db);
    if (rc != SQLITE_OK) {
        /* If opening fails, we can still test some functions with NULL db,
           but most will fail gracefully. We proceed to test error handling. */
        if (tempName) {
            free(tempName);
        }
        return 0;
    }

    /* Enable WAL mode to make checkpoint functions meaningful */
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    /* Test sqlite3_wal_checkpoint */
    if (zDbName) {
        sqlite3_wal_checkpoint(db, zDbName);
    } else {
        sqlite3_wal_checkpoint(db, "main");
    }

    /* Test sqlite3_wal_checkpoint_v2 with various modes */
    int pnLog = -1;
    int pnCkpt = -1;
    sqlite3_wal_checkpoint_v2(db, checkpointDbName, mode, &pnLog, &pnCkpt);

    /* Test sqlite3_file_control with various opcodes */
    sqlite3_file_control(db, zDbName ? zDbName : "main", fileControlOp, fileControlArg);

    /* Test sqlite3_db_readonly */
    readonly = sqlite3_db_readonly(db, zDbName ? zDbName : "main");
    (void)readonly; /* Suppress unused variable warning */

    /* Test sqlite3_db_name */
    dbName = sqlite3_db_name(db, dbIndex);
    (void)dbName; /* Suppress unused variable warning */

    /* Cleanup: Close the database */
    sqlite3_close(db);

    /* Free allocated memory if necessary */
    if (tempName) {
        free(tempName);
    }

    return 0;
}