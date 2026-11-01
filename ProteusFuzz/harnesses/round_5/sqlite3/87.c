// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_error_offset at sqlite3.c:188385:16 in sqlite3.h
// sqlite3_system_errno at sqlite3.c:188460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Fuzz driver for sqlite3 error handling APIs.
 * Target APIs:
 * 1. sqlite3_set_errmsg
 * 2. sqlite3_error_offset
 * 3. sqlite3_errmsg
 * 4. sqlite3_system_errno
 * 5. sqlite3_extended_errcode
 * 6. sqlite3_errcode
 */

/*
 * Helper to initialize a database connection.
 * Returns NULL on failure.
 */
static sqlite3 *init_db(void) {
    sqlite3 *db = NULL;
    /* Open an in-memory database to avoid file system dependencies */
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/*
 * Helper to perform a deliberate error operation to trigger error state.
 * This ensures that error retrieval functions have something to report.
 */
static void trigger_error(sqlite3 *db) {
    const char *sql = "SELECT * FROM nonexistent_table_xyz";
    sqlite3_stmt *stmt;
    /* Attempt to prepare an invalid statement to trigger an error */
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    /* Also try executing a syntax error */
    sqlite3_exec(db, "INVALID SQL STATEMENT", NULL, NULL, NULL);
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int errcode;
    int syserrno;
    int ext_errcode;
    int offset;
    const char *errmsg;
    const char *custom_msg;

    /* Guard against empty inputs */
    if (Size == 0) {
        return 0;
    }

    /* Initialize database connection */
    db = init_db();
    if (!db) {
        return 0;
    }

    /*
     * Extract data from the fuzz input to drive the logic.
     * We use a simple modulo and casting strategy to derive values.
     */
    errcode = (int)(Data[0] & 0xFF);
    /* Ensure errcode is a plausible SQLite error code range or a random int */
    if (errcode > 200) {
        errcode = Data[0] % 100;
    }

    /*
     * Prepare a custom error message if available in the buffer.
     * We treat the input as a potential string, but ensure null-termination.
     */
    custom_msg = NULL;
    if (Size > 1) {
        /* Create a small buffer for the message to ensure safety */
        char *msg_buf = (char *)malloc(Size + 1);
        if (msg_buf) {
            memcpy(msg_buf, Data, Size);
            msg_buf[Size] = '\0';
            custom_msg = msg_buf;
        }
    }

    /*
     * Scenario 1: Trigger an error first to ensure state is set
     */
    trigger_error(db);

    /*
     * Scenario 2: Test sqlite3_set_errmsg
     * Try setting a custom error code and message
     */
    rc = sqlite3_set_errmsg(db, errcode, custom_msg);
    /* Check return value, but don't fail the fuzzer, just continue */
    (void)rc;

    /*
     * Scenario 3: Test sqlite3_errcode
     * Retrieve the error code
     */
    errcode = sqlite3_errcode(db);
    (void)errcode;

    /*
     * Scenario 4: Test sqlite3_extended_errcode
     * Retrieve extended error code
     */
    ext_errcode = sqlite3_extended_errcode(db);
    (void)ext_errcode;

    /*
     * Scenario 5: Test sqlite3_errmsg
     * Retrieve error message
     */
    errmsg = sqlite3_errmsg(db);
    (void)errmsg;

    /*
     * Scenario 6: Test sqlite3_error_offset
     * Retrieve error offset
     */
    offset = sqlite3_error_offset(db);
    (void)offset;

    /*
     * Scenario 7: Test sqlite3_system_errno
     * Retrieve system errno (might be 0 if no I/O error occurred)
     */
    syserrno = sqlite3_system_errno(db);
    (void)syserrno;

    /*
     * Scenario 8: Test with NULL database handle (should return SQLITE_MISUSE or 0)
     */
    rc = sqlite3_set_errmsg(NULL, SQLITE_ERROR, "test");
    (void)rc;
    
    errcode = sqlite3_errcode(NULL);
    (void)errcode;

    ext_errcode = sqlite3_extended_errcode(NULL);
    (void)ext_errcode;

    errmsg = sqlite3_errmsg(NULL);
    (void)errmsg;

    offset = sqlite3_error_offset(NULL);
    (void)offset;

    syserrno = sqlite3_system_errno(NULL);
    (void)syserrno;

    /*
     * Scenario 9: Trigger another error and re-test to ensure state updates
     */
    trigger_error(db);
    
    /* Re-call all functions to exercise state changes */
    rc = sqlite3_set_errmsg(db, SQLITE_BUSY, "fuzz_busy");
    (void)rc;
    
    errcode = sqlite3_errcode(db);
    (void)errcode;
    
    ext_errcode = sqlite3_extended_errcode(db);
    (void)ext_errcode;
    
    errmsg = sqlite3_errmsg(db);
    (void)errmsg;
    
    offset = sqlite3_error_offset(db);
    (void)offset;
    
    syserrno = sqlite3_system_errno(db);
    (void)syserrno;

    /*
     * Cleanup
     */
    if (custom_msg) {
        free((void *)custom_msg);
    }
    sqlite3_close(db);

    return 0;
}