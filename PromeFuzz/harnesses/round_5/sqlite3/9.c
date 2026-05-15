// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:90982:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:90967:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Create a dummy database connection for testing
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with a BLOB column to use with blob functions
    char *err_msg = NULL;
    rc = sqlite3_exec(db, 
        "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);"
        "INSERT INTO test (data) VALUES (zeroblob(100));", 
        NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        if (err_msg) sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Open a BLOB handle for testing
    sqlite3_blob *blob = NULL;
    rc = sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // 1. Call sqlite3_errmsg
    const char *error_str = sqlite3_errmsg(db);
    (void)error_str; // Use result to avoid unused variable warning
    
    // 2. Call sqlite3_blob_reopen
    // Use fuzzer data to determine rowid if we have enough bytes
    sqlite3_int64 rowid = 1; // Default to existing row
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&rowid, Data, sizeof(sqlite3_int64));
        // Ensure rowid is positive
        if (rowid < 0) rowid = -rowid;
        if (rowid == 0) rowid = 1;
    }
    rc = sqlite3_blob_reopen(blob, rowid);
    (void)rc; // Use result to avoid unused variable warning
    
    // 3. Call sqlite3_blob_bytes
    int blob_size = sqlite3_blob_bytes(blob);
    (void)blob_size; // Use result to avoid unused variable warning
    
    // 4. Call sqlite3_realloc64
    // Use fuzzer data to determine allocation size
    sqlite3_uint64 alloc_size = 0;
    if (Size >= sizeof(sqlite3_uint64) + sizeof(sqlite3_int64)) {
        memcpy(&alloc_size, Data + sizeof(sqlite3_int64), sizeof(sqlite3_uint64));
        // Limit allocation size to avoid excessive memory usage
        if (alloc_size > 1024 * 1024) { // 1MB max
            alloc_size = 1024 * 1024;
        }
    }
    
    // Allocate initial memory block
    void *ptr = sqlite3_malloc(10);
    if (ptr) {
        // Reallocate using fuzzer-determined size
        void *new_ptr = sqlite3_realloc64(ptr, alloc_size);
        if (new_ptr) {
            // If realloc succeeded, ptr is already freed by realloc64
            // Only free the new pointer if it's different from the original
            if (new_ptr != ptr) {
                sqlite3_free(new_ptr);
            }
            // Don't free ptr here - it was already freed by realloc64
        } else {
            // If realloc failed, free the original pointer
            sqlite3_free(ptr);
        }
    }
    
    // 5. Call sqlite3_randomness
    // Determine buffer size from fuzzer input
    int random_bytes = 0;
    if (Size > 0) {
        random_bytes = Data[0] % 256; // Use first byte for size (0-255)
    }
    
    // Allocate buffer for randomness
    void *random_buf = NULL;
    if (random_bytes > 0) {
        random_buf = sqlite3_malloc(random_bytes);
        if (random_buf) {
            sqlite3_randomness(random_bytes, random_buf);
            sqlite3_free(random_buf);
        }
    } else {
        // Call with NULL to force reseed
        sqlite3_randomness(0, NULL);
    }
    
    // Cleanup
    sqlite3_blob_close(blob);
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}