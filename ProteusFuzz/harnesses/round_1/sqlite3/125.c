// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to create a WAL hook callback for fuzzing.
 * It performs minimal operations to avoid infinite loops or crashes.
 */
static int fuzz_wal_callback(void *pArg, sqlite3 *db, const char *zDb, int nPage) {
    (void)pArg;
    (void)db;
    (void)zDb;
    (void)nPage;
    return SQLITE_OK;
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    void *prev_hook = NULL;
    int prev_errcode;
    int prev_db_config_result;
    int arg2 = 0;
    int *pArg2 = &arg2;

    /* Guard against empty inputs to avoid trivial loops and buffer overflows */
    if (Size < 1) {
        return 0;
    }

    /*
     * Step 1: Prepare the environment.
     * Open an in-memory database to avoid file system issues.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Step 2: Execute diverse API calls based on input data.
     * We use the input buffer to drive different code paths, ensuring bounds checks.
     */

    /* 
     * Test sqlite3_busy_timeout
     * Use the first byte to determine the timeout value.
     */
    int timeout_val = (int)Data[0];
    if (timeout_val < 0) timeout_val = -1;
    if (timeout_val > 10000) timeout_val = 10000;
    sqlite3_busy_timeout(db, timeout_val);

    /*
     * Test sqlite3_db_config
     * Check if we have enough data for op and arguments.
     */
    if (Size >= 3) {
        int op = (int)Data[1] % 10;
        int arg1 = (int)Data[2] % 2; /* 0 or 1 */

        if (op == 0) {
            /* SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE */
            prev_db_config_result = sqlite3_db_config(db, SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE, arg1, pArg2);
        } else {
            /* Try other safe operations or invalid ones to test error handling */
            prev_db_config_result = sqlite3_db_config(db, op, arg1, pArg2);
        }
    }

    /*
     * Test sqlite3_wal_hook
     * Check if we have enough data for the hook decision.
     * Fixed: Ensure Size >= 5 before accessing Data[4]
     */
    if (Size >= 5) {
        int set_hook = (int)Data[3] % 2;
        if (set_hook) {
            prev_hook = sqlite3_wal_hook(db, fuzz_wal_callback, (void *)(size_t)Data[4]);
        } else {
            prev_hook = sqlite3_wal_hook(db, NULL, NULL);
        }
    }

    /*
     * Create a statement to test sqlite3_db_handle and sqlite3_errcode.
     * We prepare a simple query.
     */
    const char *sql = "SELECT 1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt != NULL) {
        /* Test sqlite3_db_handle */
        sqlite3 *handle_from_stmt = sqlite3_db_handle(stmt);
        (void)handle_from_stmt; /* Suppress unused warning, logic is tested */

        /* Step the statement to potentially trigger errors or states */
        sqlite3_step(stmt);

        /* Finalize the statement */
        sqlite3_finalize(stmt);
    }

    /*
     * Test sqlite3_errcode
     * This retrieves the error code from the last operation.
     */
    prev_errcode = sqlite3_errcode(db);
    (void)prev_errcode;

    /*
     * Additional state exploration:
     * Try to trigger an error by executing invalid SQL to ensure errcode changes.
     */
    sqlite3_exec(db, "INVALID SQL SYNTAX FOR FUZZING", NULL, NULL, NULL);
    int err_after_invalid = sqlite3_errcode(db);
    (void)err_after_invalid;

    /*
     * Step 3: Cleanup.
     * Close the database connection using sqlite3_close_v2.
     */
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}