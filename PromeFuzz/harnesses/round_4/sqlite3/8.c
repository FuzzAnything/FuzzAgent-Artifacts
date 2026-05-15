// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:90982:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:90967:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
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

static void dummy_xDestroy(void *p) {
    free(p);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Create a dummy database for testing
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with a BLOB column for blob operations
    char *err_msg = NULL;
    const char *create_table_sql = 
        "CREATE TABLE IF NOT EXISTS test_table (id INTEGER PRIMARY KEY, data BLOB);";
    if (sqlite3_exec(db, create_table_sql, NULL, NULL, &err_msg) != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Insert a dummy row with BLOB data
    const char *insert_sql = 
        "INSERT OR REPLACE INTO test_table (id, data) VALUES (1, ?);";
    sqlite3_stmt *stmt = NULL;
    if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) == SQLITE_OK) {
        // Use fuzzer data for the BLOB
        sqlite3_bind_blob(stmt, 1, Data, Size > 1024 ? 1024 : Size, SQLITE_STATIC);
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }
    
    // 1. Call sqlite3_errmsg
    const char *err_str = sqlite3_errmsg(db);
    (void)err_str; // Use result to avoid unused variable warning
    
    // 2. Call sqlite3_blob_reopen
    sqlite3_blob *blob = NULL;
    int blob_open_result = sqlite3_blob_open(db, "main", "test_table", "data", 1, 0, &blob);
    
    if (blob_open_result == SQLITE_OK && blob) {
        // Try to reopen to a different row (rowid 2, which may not exist)
        // Use fuzzer data to determine rowid
        sqlite3_int64 rowid = 2;
        if (Size >= sizeof(sqlite3_int64)) {
            memcpy(&rowid, Data, sizeof(sqlite3_int64));
        }
        int reopen_result = sqlite3_blob_reopen(blob, rowid);
        (void)reopen_result; // Use result
        
        // 3. Call sqlite3_blob_bytes
        int blob_size = sqlite3_blob_bytes(blob);
        (void)blob_size; // Use result
        
        sqlite3_blob_close(blob);
    }
    
    // 4. Call sqlite3_realloc64
    // Use fuzzer data for size
    sqlite3_uint64 realloc_size = 1024;
    if (Size >= sizeof(sqlite3_uint64)) {
        memcpy(&realloc_size, Data, sizeof(sqlite3_uint64));
        // Limit size to avoid excessive memory allocation
        if (realloc_size > 1024 * 1024) {
            realloc_size = 1024 * 1024;
        }
    }
    
    // Allocate initial memory
    void *ptr = sqlite3_malloc(64);
    if (ptr) {
        void *new_ptr = sqlite3_realloc64(ptr, realloc_size);
        if (new_ptr) {
            ptr = new_ptr;
        }
        sqlite3_free(ptr);
    }
    
    // 5. Call sqlite3_randomness
    // Use fuzzer data to determine buffer size
    int randomness_size = 64;
    if (Size > 0) {
        randomness_size = Data[0] % 256;
        if (randomness_size == 0) randomness_size = 1;
    }
    
    // Allocate buffer for randomness
    void *random_buffer = sqlite3_malloc(randomness_size);
    if (random_buffer) {
        // First call with buffer
        sqlite3_randomness(randomness_size, random_buffer);
        
        // Second call with NULL to force reseed
        sqlite3_randomness(0, NULL);
        
        sqlite3_free(random_buffer);
    }
    
    // Cleanup
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}