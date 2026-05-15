// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:90982:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:90967:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_test_db(sqlite3 **db, sqlite3_blob **blob) {
    int rc;
    sqlite3_stmt *stmt;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return rc;
    
    /* Create a table with a BLOB column */
    rc = sqlite3_exec(*db, 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test (data) VALUES (x'0102030405');"
        "INSERT INTO test (data) VALUES (x'060708090A');", 
        NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    /* Open a BLOB handle to the first row */
    rc = sqlite3_blob_open(*db, "main", "test", "data", 1, 0, blob);
    
    /* Prepare and finalize a statement to ensure error state */
    sqlite3_prepare_v2(*db, "SELECT * FROM test", -1, &stmt, NULL);
    sqlite3_finalize(stmt);
    
    return rc;
}

static void cleanup_test_db(sqlite3 *db, sqlite3_blob *blob) {
    if (blob) sqlite3_blob_close(blob);
    if (db) sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_blob *blob = NULL;
    const char *err_msg = NULL;
    int rc;
    void *realloc_ptr = NULL;
    sqlite3_int64 rowid;
    int blob_size;
    
    /* Use fuzzer input to determine parameters */
    if (Size < sizeof(sqlite3_int64)) {
        /* Not enough data, use defaults */
        rowid = 2;
    } else {
        /* Use first 8 bytes as rowid */
        memcpy(&rowid, Data, sizeof(sqlite3_int64));
        Data += sizeof(sqlite3_int64);
        Size -= sizeof(sqlite3_int64);
    }
    
    /* Create test database and blob handle */
    rc = create_test_db(&db, &blob);
    
    /* 1. Call sqlite3_errmsg */
    err_msg = sqlite3_errmsg(db);
    (void)err_msg; /* Use result to avoid unused variable warning */
    
    /* 2. Call sqlite3_blob_reopen if we have a valid blob handle */
    if (blob && rc == SQLITE_OK) {
        sqlite3_blob_reopen(blob, rowid);
    }
    
    /* 3. Call sqlite3_blob_bytes */
    if (blob) {
        blob_size = sqlite3_blob_bytes(blob);
        (void)blob_size; /* Use result */
    }
    
    /* 4. Call sqlite3_realloc64 */
    if (Size > 0) {
        /* Use next 8 bytes as size for realloc */
        sqlite3_uint64 realloc_size = 0;
        if (Size >= sizeof(sqlite3_uint64)) {
            memcpy(&realloc_size, Data, sizeof(sqlite3_uint64));
            Data += sizeof(sqlite3_uint64);
            Size -= sizeof(sqlite3_uint64);
        }
        
        /* Allocate initial memory if needed */
        if (realloc_size > 0) {
            void *initial_ptr = sqlite3_malloc64(16);
            if (initial_ptr) {
                realloc_ptr = sqlite3_realloc64(initial_ptr, realloc_size);
                if (realloc_ptr) {
                    sqlite3_free(realloc_ptr);
                } else {
                    sqlite3_free(initial_ptr);
                }
            }
        }
    }
    
    /* 5. Call sqlite3_randomness */
    if (Size > 0) {
        /* Use remaining data for randomness buffer */
        void *random_buf = sqlite3_malloc(Size);
        if (random_buf) {
            sqlite3_randomness((int)Size, random_buf);
            sqlite3_free(random_buf);
        }
    } else {
        /* Call with NULL to force reseed */
        sqlite3_randomness(0, NULL);
    }
    
    /* Cleanup */
    cleanup_test_db(db, blob);
    
    return 0;
}