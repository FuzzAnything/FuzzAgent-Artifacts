// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_errmsg16 at sqlite3.c:188400:24 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_next_stmt at sqlite3.c:94328:26 in sqlite3.h
// sqlite3_get_autocommit at sqlite3.c:189554:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void cleanup_stmt(sqlite3_stmt *pStmt) {
    if (pStmt) {
        sqlite3_finalize(pStmt);
    }
}

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    sqlite3_stmt *pIter = NULL;
    const char *zTail = NULL;
    const void *errMsg = NULL;
    int rc;
    int autoCommit;
    sqlite3 *stmtDb;

    // Ensure we have some data to work with
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Create a dummy SQL string from the input data, ensuring it is null-terminated
    size_t sqlLen = Size;
    char *sqlBuf = (char *)malloc(sqlLen + 1);
    if (!sqlBuf) {
        cleanup_db(db);
        return 0;
    }
    memcpy(sqlBuf, Data, sqlLen);
    sqlBuf[sqlLen] = '\0';

    // 2. Invoke target functions with diverse states

    // A. sqlite3_prepare
    // Try with negative nByte (null-terminated)
    rc = sqlite3_prepare(db, sqlBuf, -1, &pStmt, &zTail);
    if (rc == SQLITE_OK && pStmt) {
        cleanup_stmt(pStmt);
        pStmt = NULL;
    }

    // Try with positive nByte (exact length)
    rc = sqlite3_prepare(db, sqlBuf, (int)sqlLen, &pStmt, &zTail);
    if (rc == SQLITE_OK && pStmt) {
        cleanup_stmt(pStmt);
        pStmt = NULL;
    }

    // Try with zero nByte
    rc = sqlite3_prepare(db, sqlBuf, 0, &pStmt, &zTail);
    if (rc == SQLITE_OK && pStmt) {
        cleanup_stmt(pStmt);
        pStmt = NULL;
    }

    // B. sqlite3_errmsg16
    errMsg = sqlite3_errmsg16(db);
    (void)errMsg;

    // C. sqlite3_next_stmt
    pIter = sqlite3_next_stmt(db, NULL);
    while (pIter) {
        pIter = sqlite3_next_stmt(db, pIter);
    }

    // D. sqlite3_get_autocommit
    autoCommit = sqlite3_get_autocommit(db);
    (void)autoCommit;

    // E. sqlite3_db_handle
    // Prepare a valid statement to test sqlite3_db_handle
    const char *validSql = "SELECT 1;";
    rc = sqlite3_prepare(db, validSql, -1, &pStmt, &zTail);
    if (rc == SQLITE_OK && pStmt) {
        stmtDb = sqlite3_db_handle(pStmt);
        (void)stmtDb;
        cleanup_stmt(pStmt);
        pStmt = NULL;
    }

    // Test with NULL statement
    stmtDb = sqlite3_db_handle(NULL);
    (void)stmtDb;

    // F. sqlite3_context_db_handle
    // This function requires a valid sqlite3_context, which is only available
    // inside a user-defined SQL function. Passing NULL triggers an assert in
    // debug builds (as seen in the error log). We must skip this test case
    // or only test it if we can construct a valid context, which is not possible
    // directly from the fuzzer driver without executing a function.
    // Therefore, we omit the call with NULL to prevent the crash.
    // sqlite3_context_db_handle(NULL); // Removed to fix crash

    // Cleanup
    free(sqlBuf);
    cleanup_db(db);

    return 0;
}