// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:188172:16 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_wal_checkpoint_v2
 * - sqlite3_get_autocommit
 * - sqlite3_extended_errcode
 * - sqlite3_error_offset
 * - sqlite3_errcode
 * - sqlite3_errmsg16
 *
 * Note: Passing NULL to sqlite3 API functions often causes internal dereferences
 * leading to crashes (e.g., accessing db->mutex). To fuzz these functions safely
 * and explore error handling without crashing, we must ensure a valid handle is
 * passed, or avoid calling functions that strictly require a valid handle if
 * they are known to crash on NULL.
 *
 * Based on the ASAN error, sqlite3_wal_checkpoint_v2 crashes on NULL db.
 * We will remove the NULL test for this specific function and others that
 * might behave similarly, or ensure we only call them with valid handles.
 * However, for functions like sqlite3_errcode, passing NULL is documented to
 * return SQLITE_MISUSE without crashing in some builds, but behavior varies.
 * To be safe and compliant with the crash report, we will only test with valid handles
 * or use a dummy handle if necessary, but the safest approach for fuzzing
 * robustness is to test valid paths and rely on the library's internal checks
 * if they exist. If the library crashes on NULL, we skip the NULL test for that API.
 */

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int pnLog = 0;
    int pnCkpt = 0;
    const char *zDb;
    int eMode;
    int autocommit;
    int errCode;
    int extErrCode;
    int errorOffset;
    const void *errMsg16;

    /*
     * Step 1: Prepare the environment.
     * Create a valid database connection.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* If we cannot open a database, we cannot test the APIs safely.
         * We return immediately to avoid crashes. */
        return 0;
    }

    /*
     * Parse input data to drive fuzzing logic.
     */
    if (Size > 0) {
        eMode = Data[0] % 5;
        if (Size > 1 && Data[1] != 0) {
            zDb = "main";
        } else {
            zDb = NULL;
        }
    } else {
        eMode = 0;
        zDb = NULL;
    }

    /*
     * Step 2: Invoke target functions with valid handles.
     *
     * 2.1: Enable WAL mode to make checkpointing meaningful.
     */
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);

    /*
     * 2.2: sqlite3_wal_checkpoint_v2
     * Only call with valid 'db'. The ASAN report showed a crash on NULL.
     */
    rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
    (void)rc;
    (void)pnLog;
    (void)pnCkpt;

    /*
     * 2.3: sqlite3_get_autocommit
     */
    autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;

    /*
     * 2.4: Trigger an error to test error reporting functions.
     */
    sqlite3_exec(db, "SELECT INVALID_SYNTAX;", NULL, NULL, NULL);

    /*
     * 2.5: sqlite3_errcode
     */
    errCode = sqlite3_errcode(db);
    (void)errCode;

    /*
     * 2.6: sqlite3_extended_errcode
     */
    extErrCode = sqlite3_extended_errcode(db);
    (void)extErrCode;

    /*
     * 2.7: sqlite3_error_offset
     */
    errorOffset = sqlite3_error_offset(db);
    (void)errorOffset;

    /*
     * 2.8: sqlite3_errmsg16
     */
    errMsg16 = sqlite3_errmsg16(db);
    (void)errMsg16;

    /*
     * Additional state exploration:
     * Start a transaction to change autocommit state.
     */
    sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;

    /*
     * Checkpoint inside a transaction.
     */
    rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
    (void)rc;

    /*
     * Commit and check autocommit again.
     */
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    autocommit = sqlite3_get_autocommit(db);
    (void)autocommit;

    /*
     * Step 3: Cleanup.
     */
    sqlite3_close(db);
    db = NULL;

    /*
     * Note: We do NOT test with NULL handles for functions that are known to
     * crash (like sqlite3_wal_checkpoint_v2) as per the ASAN report.
     * Testing with NULL for other functions (like sqlite3_errcode) is risky
     * and might also crash depending on the build. To ensure stability,
     * we skip NULL tests for all these functions in this driver.
     * The primary goal is to fuzz the logic paths within valid usage.
     */

    return 0;
}