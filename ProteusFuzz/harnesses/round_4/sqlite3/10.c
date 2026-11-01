// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
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
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
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
 * Fuzz driver for sqlite3 API functions:
 * sqlite3_errmsg -> sqlite3_blob_reopen -> sqlite3_blob_bytes -> sqlite3_realloc64 -> sqlite3_randomness
 */

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    int rc;
    const char *err_msg;
    void *ptr = NULL;
    sqlite3_uint64 new_size;
    uint8_t random_buf[256];

    /* Ensure minimum size to extract parameters */
    if (Size < 4) {
        return 0;
    }

    /* Extract parameters from input data */
    new_size = (sqlite3_uint64)Data[0] | ((sqlite3_uint64)Data[1] << 8) |
               ((sqlite3_uint64)Data[2] << 16) | ((sqlite3_uint64)Data[3] << 24);
    if (new_size == 0) new_size = 1; /* Avoid zero size for realloc */

    /* Step 1: Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a test table and insert a row to work with BLOBs */
    const char *create_sql = "CREATE TABLE test_blob (id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO test_blob (id, data) VALUES (1, X'DEADBEEF');";
    char *err_msg_sql = NULL;

    rc = sqlite3_exec(db, create_sql, NULL, NULL, &err_msg_sql);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg_sql);
    }
    rc = sqlite3_exec(db, insert_sql, NULL, NULL, &err_msg_sql);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg_sql);
    }

    /* Open a BLOB handle */
    rc = sqlite3_blob_open(db, "main", "test_blob", "data", 1, SQLITE_OPEN_READWRITE, &blob);
    if (rc != SQLITE_OK || blob == NULL) {
        /* If blob open fails, we can still test errmsg and other functions */
    }

    /* Step 2: Invoke sqlite3_errmsg */
    /* Even if no error occurred, this is safe to call */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg; /* Suppress unused variable warning */

    /* Step 3: Invoke sqlite3_blob_reopen */
    /* Try to reopen to a valid rowid (1) or an invalid one based on input */
    sqlite3_int64 rowid = (Size > 4) ? (sqlite3_int64)Data[4] : 1;
    if (blob != NULL) {
        rc = sqlite3_blob_reopen(blob, rowid);
        (void)rc;
    }

    /* Step 4: Invoke sqlite3_blob_bytes */
    /* This should return 0 if blob is invalid or aborted */
    if (blob != NULL) {
        int bytes = sqlite3_blob_bytes(blob);
        (void)bytes;
    }

    /* Step 5: Invoke sqlite3_realloc64 */
    /* Allocate initial memory if needed, then reallocate */
    if (ptr == NULL) {
        ptr = sqlite3_realloc64(NULL, 1024);
    }
    if (ptr != NULL) {
        ptr = sqlite3_realloc64(ptr, new_size);
    }

    /* Step 6: Invoke sqlite3_randomness */
    int random_bytes = (Size > 5) ? (int)Data[5] : 32;
    if (random_bytes > 256) random_bytes = 256;
    if (random_bytes < 0) random_bytes = 0;
    sqlite3_randomness(random_bytes, random_buf);

    /* Cleanup */
    if (blob != NULL) {
        sqlite3_blob_close(blob);
    }
    if (ptr != NULL) {
        sqlite3_free(ptr);
    }
    sqlite3_close(db);

    return 0;
}