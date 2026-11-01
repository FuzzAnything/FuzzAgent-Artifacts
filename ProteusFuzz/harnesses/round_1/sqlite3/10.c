// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:104989:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions.
 * Target sequence: sqlite3_errmsg -> sqlite3_blob_reopen -> sqlite3_blob_bytes -> sqlite3_realloc64 -> sqlite3_randomness
 */

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    int rc;
    sqlite3_int64 rowid = 0;
    void *ptr = NULL;
    void *new_ptr = NULL;
    int random_size = 0;
    unsigned char *random_buf = NULL;
    sqlite3_uint64 new_size = 0;

    /* 1. Prepare environment: Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a table and insert a row to have a valid BLOB to work with */
    const char *create_sql = "CREATE TABLE test_blob (id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO test_blob (id, data) VALUES (1, X'0102030405');";
    char *err_msg = NULL;

    rc = sqlite3_exec(db, create_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }

    /* Open a BLOB handle for the inserted row.
     * Signature: sqlite3_blob_open(sqlite3*, const char *zDb, const char *zTable, const char *zColumn, sqlite3_int64 iRow, int flags, sqlite3_blob **ppBlob)
     */
    rc = sqlite3_blob_open(db, "main", "test_blob", "data", 1, 0, &blob);

    /* 2. Invoke target functions in the specified order */

    /* Step 1: sqlite3_errmsg */
    const char *msg = sqlite3_errmsg(db);
    (void)msg;

    /* Step 2: sqlite3_blob_reopen */
    if (Size >= 8) {
        rowid = *(const sqlite3_int64 *)(Data);
    } else {
        rowid = (Size > 0) ? (sqlite3_int64)Data[0] : 999;
        if (Size > 1) rowid += Data[1];
    }

    if (blob) {
        rc = sqlite3_blob_reopen(blob, rowid);
        (void)rc;
    }

    /* Step 3: sqlite3_blob_bytes */
    int blob_size = 0;
    if (blob) {
        blob_size = sqlite3_blob_bytes(blob);
    }
    (void)blob_size;

    /* Step 4: sqlite3_realloc64 */
    if (Size > 0) {
        size_t alloc_size = (Size < 1024) ? Size : 1024;
        ptr = sqlite3_malloc(alloc_size);
        if (ptr) {
            /* Safely read new_size from Data buffer */
            if (Size >= sizeof(sqlite3_uint64)) {
                new_size = *(const sqlite3_uint64 *)(Data);
            } else {
                new_size = alloc_size * 2;
            }
            
            /* Cap the size to prevent excessive allocation */
            if (new_size > 1024 * 1024) new_size = 1024 * 1024;
            
            new_ptr = sqlite3_realloc64(ptr, new_size);
            if (new_ptr) {
                ptr = new_ptr;
            }
        }
    } else {
        ptr = sqlite3_realloc64(NULL, 64);
    }

    /* Step 5: sqlite3_randomness */
    if (Size > 0) {
        random_size = (Size < 256) ? (int)Size : 256;
        random_buf = (unsigned char *)sqlite3_malloc(random_size);
        if (random_buf) {
            sqlite3_randomness(random_size, random_buf);
        }
    } else {
        sqlite3_randomness(0, NULL);
    }

    /* 3. Cleanup */
    if (blob) {
        sqlite3_blob_close(blob);
    }
    if (ptr) {
        sqlite3_free(ptr);
    }
    if (random_buf) {
        sqlite3_free(random_buf);
    }
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}