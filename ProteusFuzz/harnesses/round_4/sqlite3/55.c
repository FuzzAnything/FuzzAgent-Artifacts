// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_errstr at sqlite3.c:188469:24 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_extended_errcode at sqlite3.c:188451:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare at sqlite3.c:147204:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:147234:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_extended_errcode
 * - sqlite3_set_errmsg
 * - sqlite3_prepare
 * - sqlite3_prepare_v3
 * - sqlite3_errcode
 * - sqlite3_errstr
 */

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *tail = NULL;
    int rc;
    int errcode;
    const char *errmsg;
    const char *errstr;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Create a dummy file for database operations */
    const char *dbPath = "./dummy_file";
    FILE *f = fopen(dbPath, "w");
    if (f) {
        fclose(f);
    }

    /* Open database in memory or file mode */
    rc = sqlite3_open(dbPath, &db);
    if (rc != SQLITE_OK || db == NULL) {
        /* Try in-memory database as fallback */
        rc = sqlite3_open(":memory:", &db);
        if (rc != SQLITE_OK || db == NULL) {
            return 0;
        }
    }

    /* Ensure we have a valid database handle */
    if (db == NULL) {
        return 0;
    }

    /*
     * Use input data as SQL string. Ensure it is null-terminated.
     * We'll use a portion of the input as SQL, and the rest as error message.
     */
    size_t sqlLen = Size;
    if (sqlLen > 0 && Data[sqlLen - 1] == 0) {
        /* Already null-terminated */
        sqlLen--;
    }

    /* Ensure null termination by using a copy */
    char *sqlCopy = malloc(sqlLen + 1);
    if (sqlCopy == NULL) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sqlCopy, Data, sqlLen);
    sqlCopy[sqlLen] = '\0';
    const char *zSql = sqlCopy;
    int nByte = (int)sqlLen;

    /*
     * Test sqlite3_prepare
     */
    rc = sqlite3_prepare(db, zSql, nByte, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /*
     * Test sqlite3_prepare_v3 with various flags
     */
    rc = sqlite3_prepare_v3(db, zSql, nByte, 0, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /* Try with SQLITE_PREPARE_PERSISTENT if available */
    #ifdef SQLITE_PREPARE_PERSISTENT
    rc = sqlite3_prepare_v3(db, zSql, nByte, SQLITE_PREPARE_PERSISTENT, &stmt, &tail);
    #endif
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    free(sqlCopy);

    /*
     * Test error code retrieval functions
     */
    errcode = sqlite3_errcode(db);
    (void)errcode; /* Suppress unused variable warning */

    int extErrcode = sqlite3_extended_errcode(db);
    (void)extErrcode;

    errmsg = sqlite3_errmsg(db);
    (void)errmsg;

    /*
     * Test sqlite3_errstr with various error codes
     */
    errstr = sqlite3_errstr(SQLITE_OK);
    (void)errstr;
    errstr = sqlite3_errstr(SQLITE_ERROR);
    (void)errstr;
    errstr = sqlite3_errstr(SQLITE_MISUSE);
    (void)errstr;
    errstr = sqlite3_errstr(SQLITE_NOMEM);
    (void)errstr;

    /*
     * Test sqlite3_set_errmsg with various inputs
     */
    rc = sqlite3_set_errmsg(db, SQLITE_OK, NULL);
    if (rc == SQLITE_OK) {
        /* Check that error code and message were set */
        errcode = sqlite3_errcode(db);
        errmsg = sqlite3_errmsg(db);
        (void)errcode;
        (void)errmsg;
    }

    /* Set a custom error message */
    const char *customMsg = "Custom error message";
    rc = sqlite3_set_errmsg(db, SQLITE_ERROR, customMsg);
    if (rc == SQLITE_OK) {
        errcode = sqlite3_errcode(db);
        errmsg = sqlite3_errmsg(db);
        (void)errcode;
        (void)errmsg;
    }

    /* Set error with NULL message (should use default) */
    rc = sqlite3_set_errmsg(db, SQLITE_MISUSE, NULL);
    if (rc == SQLITE_OK) {
        errcode = sqlite3_errcode(db);
        errmsg = sqlite3_errmsg(db);
        (void)errcode;
        (void)errmsg;
    }

    /*
     * Test with invalid database handle (NULL)
     */
    errcode = sqlite3_errcode(NULL);
    (void)errcode;

    extErrcode = sqlite3_extended_errcode(NULL);
    (void)extErrcode;

    rc = sqlite3_set_errmsg(NULL, SQLITE_ERROR, "Should fail");
    (void)rc;

    /*
     * Test with invalid SQL (empty or comment)
     */
    const char *emptySql = "";
    rc = sqlite3_prepare(db, emptySql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    const char *commentSql = "-- This is a comment";
    rc = sqlite3_prepare(db, commentSql, -1, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /*
     * Test with very long SQL
     */
    size_t longSqlLen = Size > 10000 ? 10000 : Size;
    char *longSql = malloc(longSqlLen + 1);
    if (longSql != NULL) {
        memset(longSql, 'A', longSqlLen);
        longSql[longSqlLen] = '\0';
        rc = sqlite3_prepare(db, longSql, (int)longSqlLen, &stmt, &tail);
        if (rc == SQLITE_OK && stmt != NULL) {
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
        free(longSql);
    }

    /*
     * Test with binary data as SQL (should fail gracefully)
     */
    rc = sqlite3_prepare(db, (const char *)Data, (int)Size, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /*
     * Test sqlite3_prepare_v3 with invalid flags
     */
    rc = sqlite3_prepare_v3(db, (const char *)Data, (int)Size, 0xFFFFFFFF, &stmt, &tail);
    if (rc == SQLITE_OK && stmt != NULL) {
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    /*
     * Clean up
     */
    if (stmt != NULL) {
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);

    return 0;
}