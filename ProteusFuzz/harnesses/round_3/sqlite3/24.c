// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:94028:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:104989:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    int rc;
    void *pMem = NULL;
    char *zErrMsg = NULL;
    int nBytes;
    sqlite3_int64 rowid;
    int blobSize;

    /* Ensure we have enough data to perform operations */
    if (Size < 10) {
        return 0;
    }

    /* Initialize SQLite */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table to work with */
    const char *sql = "CREATE TABLE test_blob (id INTEGER PRIMARY KEY, data BLOB);";
    rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return 0;
    }

    /* Insert a row with blob data */
    sqlite3_prepare_v2(db, "INSERT INTO test_blob (id, data) VALUES (?, ?);", -1, &stmt, NULL);
    sqlite3_bind_int64(stmt, 1, 1);
    sqlite3_bind_blob(stmt, 2, (const void *)Data, Size, SQLITE_STATIC);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    /* Prepare for BLOB operations */
    rc = sqlite3_prepare_v2(db, "SELECT data FROM test_blob WHERE id = 1;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return 0;
    }

    /* Open BLOB handle */
    rc = sqlite3_blob_open(db, "main", "test_blob", "data", 1, SQLITE_OPEN_READWRITE, &blob);
    sqlite3_finalize(stmt);
    stmt = NULL;

    if (rc != SQLITE_OK || blob == NULL) {
        sqlite3_close(db);
        return 0;
    }

    /* 1. sqlite3_errmsg */
    const char *err = sqlite3_errmsg(db);
    (void)err; /* Use the return value to avoid warnings */

    /* 2. sqlite3_blob_reopen */
    /* Use fuzzed data to determine rowid, but ensure it's valid */
    if (Size >= 8) {
        rowid = *(const sqlite3_int64 *)(Data + 2);
        /* Ensure rowid is positive and within reasonable bounds */
        if (rowid > 0 && rowid < 1000) {
            rc = sqlite3_blob_reopen(blob, rowid);
            (void)rc; /* Check return value implicitly */
        }
    }

    /* 3. sqlite3_blob_bytes */
    blobSize = sqlite3_blob_bytes(blob);
    (void)blobSize; /* Use the return value */

    /* 4. sqlite3_realloc64 */
    /* Allocate some memory first */
    pMem = sqlite3_malloc64(Size + 100);
    if (pMem != NULL) {
        /* Reallocate using fuzzed size */
        sqlite3_uint64 newSize = 0;
        if (Size >= 8) {
            newSize = *(const sqlite3_uint64 *)(Data + 4);
        }
        /* Ensure newSize is not too large to avoid OOM in fuzzer */
        if (newSize > 1000000) {
            newSize = 1000000;
        }
        void *pNewMem = sqlite3_realloc64(pMem, newSize);
        if (pNewMem != NULL) {
            pMem = pNewMem;
        }
    }

    /* 5. sqlite3_randomness */
    int nRandom = 0;
    if (Size >= 2) {
        nRandom = *(const int *)(Data);
        if (nRandom > 0 && nRandom < 1024) {
            char *pRandom = sqlite3_malloc64(nRandom);
            if (pRandom != NULL) {
                sqlite3_randomness(nRandom, pRandom);
                sqlite3_free(pRandom);
            }
        }
    }

    /* Cleanup */
    if (blob != NULL) {
        sqlite3_blob_close(blob);
    }
    if (pMem != NULL) {
        sqlite3_free(pMem);
    }
    sqlite3_close(db);

    return 0;
}