// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:34952:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:104846:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:104607:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:104989:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:104974:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static int setup_db_and_blob(sqlite3 **db_out, sqlite3_blob **blob_out, const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    int rc;
    const char *zSql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, data BLOB); INSERT INTO test_table (data) VALUES (X'DEADBEEF');";

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    char *zErrMsg = NULL;
    rc = sqlite3_exec(db, zSql, NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return 0;
    }

    // Open a blob handle on the inserted row (rowid 1)
    // Signature: sqlite3_blob_open(sqlite3*, const char *zDb, const char *zTable, const char *zColumn, sqlite3_int64 iRow, int flags, sqlite3_blob **ppBlob)
    rc = sqlite3_blob_open(db, "main", "test_table", "data", 1, 0, &blob);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    *db_out = db;
    *blob_out = blob;
    return 1;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    int rc;

    // 1. Prepare environment
    if (Size < 1) {
        return 0;
    }

    if (!setup_db_and_blob(&db, &blob, Data, Size)) {
        return 0;
    }

    // 2. Invoke target functions in the specified order

    // Step 1: sqlite3_errmsg
    // Retrieve error message (safe to call even if no error occurred)
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg;

    // Step 2: sqlite3_blob_reopen
    // Try to reopen the blob to a different rowid based on fuzz input
    sqlite3_int64 new_rowid = 0;
    if (Size >= 8) {
        memcpy(&new_rowid, Data, 8);
    } else if (Size > 0) {
        new_rowid = (sqlite3_int64)Data[0];
    }
    
    // Reopen to new_rowid. If row doesn't exist, blob becomes aborted.
    rc = sqlite3_blob_reopen(blob, new_rowid);

    // Step 3: sqlite3_blob_bytes
    // Returns size of blob, or 0 if aborted
    int blob_size = sqlite3_blob_bytes(blob);
    (void)blob_size;

    // Step 4: sqlite3_realloc64
    void *ptr = sqlite3_malloc(100);
    if (ptr) {
        sqlite3_uint64 new_size = 0;
        if (Size >= 8) {
            memcpy(&new_size, Data + 8, 8);
        } else {
            new_size = (Size > 0) ? (sqlite3_uint64)Data[0] : 100;
        }
        
        // Cap size to prevent excessive memory usage in fuzzer
        if (new_size > 1024 * 1024) new_size = 1024 * 1024;
        
        void *new_ptr = sqlite3_realloc64(ptr, new_size);
        if (new_ptr) {
            sqlite3_free(new_ptr);
        } else {
            sqlite3_free(ptr);
        }
    }

    // Step 5: sqlite3_randomness
    int n_rand = (Size > 1024) ? 1024 : (int)Size;
    if (n_rand < 0) n_rand = 0;
    
    if (n_rand > 0) {
        uint8_t *rand_buf = (uint8_t *)sqlite3_malloc(n_rand);
        if (rand_buf) {
            sqlite3_randomness(n_rand, rand_buf);
            sqlite3_free(rand_buf);
        }
    } else {
        sqlite3_randomness(0, NULL);
    }

    // 3. Cleanup
    if (blob) {
        sqlite3_blob_close(blob);
    }
    if (db) {
        sqlite3_close(db);
    }

    return 0;
}