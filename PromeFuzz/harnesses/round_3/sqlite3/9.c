// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:90982:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:90967:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
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
#include <stdio.h>

static void dummy_file_init(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        if (Size > 0) {
            fwrite(Data, 1, Size < 1024 ? Size : 1024, f);
        } else {
            const char *default_data = "test";
            fwrite(default_data, 1, 4, f);
        }
        fclose(f);
    }
}

static sqlite3* create_test_db(void) {
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        char *err_msg = NULL;
        const char *sql = 
            "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
            "INSERT INTO test (data) VALUES (x'01020304');";
        sqlite3_exec(db, sql, NULL, NULL, &err_msg);
        if (err_msg) {
            sqlite3_free(err_msg);
        }
    }
    return db;
}

static sqlite3_blob* create_test_blob(sqlite3 *db) {
    sqlite3_blob *blob = NULL;
    if (db) {
        sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    }
    return blob;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize dummy file
    dummy_file_init(Data, Size);
    
    // Create test database and blob handle
    sqlite3 *db = create_test_db();
    sqlite3_blob *blob = create_test_blob(db);
    
    // 1. sqlite3_errmsg
    const char *err_msg = NULL;
    if (db) {
        err_msg = sqlite3_errmsg(db);
        (void)err_msg; // Use result to avoid unused variable warning
    }
    
    // 2. sqlite3_blob_reopen
    if (blob) {
        sqlite3_int64 rowid = 1;
        if (Size >= sizeof(sqlite3_int64)) {
            memcpy(&rowid, Data, sizeof(sqlite3_int64));
        }
        sqlite3_blob_reopen(blob, rowid);
    }
    
    // 3. sqlite3_blob_bytes
    if (blob) {
        int blob_size = sqlite3_blob_bytes(blob);
        (void)blob_size;
    }
    
    // 4. sqlite3_realloc64
    sqlite3_uint64 realloc_size = 0;
    if (Size >= sizeof(sqlite3_uint64)) {
        memcpy(&realloc_size, Data, sizeof(sqlite3_uint64));
        realloc_size = realloc_size % 65536; // Limit size to avoid excessive memory usage
    }
    void *ptr = sqlite3_malloc(1);
    if (ptr) {
        void *new_ptr = sqlite3_realloc64(ptr, realloc_size);
        if (new_ptr) {
            ptr = new_ptr;
        }
        sqlite3_free(ptr);
    }
    
    // 5. sqlite3_randomness
    int randomness_size = 0;
    if (Size >= sizeof(int)) {
        memcpy(&randomness_size, Data, sizeof(int));
        randomness_size = randomness_size % 1024; // Limit size
        if (randomness_size < 0) randomness_size = 0;
    }
    if (randomness_size > 0) {
        void *buf = sqlite3_malloc(randomness_size);
        if (buf) {
            sqlite3_randomness(randomness_size, buf);
            sqlite3_free(buf);
        }
    } else {
        sqlite3_randomness(0, NULL); // Force reseed
    }
    
    // Cleanup
    if (blob) {
        sqlite3_blob_close(blob);
    }
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}