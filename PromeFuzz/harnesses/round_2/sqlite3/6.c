// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
// sqlite3_blob_close at sqlite3.c:90839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_blob_open at sqlite3.c:90600:16 in sqlite3.h
// sqlite3_blob_reopen at sqlite3.c:90982:16 in sqlite3.h
// sqlite3_blob_bytes at sqlite3.c:90967:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_randomness at sqlite3.c:20945:17 in sqlite3.h
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

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Create a dummy database connection for testing
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    // Call sqlite3_errmsg first (order 1)
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg; // Use result to avoid unused variable warning
    
    // Create a dummy BLOB handle - we need to create a table with a BLOB column first
    sqlite3_blob *blob = NULL;
    
    // Create a table with BLOB column if database is open
    if (db) {
        char *err = NULL;
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data BLOB)", NULL, NULL, &err);
        sqlite3_exec(db, "INSERT OR IGNORE INTO test (id, data) VALUES (1, zeroblob(100))", NULL, NULL, &err);
        
        // Open a BLOB handle to row 1
        sqlite3_blob_open(db, "main", "test", "data", 1, 0, &blob);
    }
    
    // Call sqlite3_blob_reopen (order 2)
    if (blob) {
        // Use fuzzer input to determine rowid - ensure it's within reasonable bounds
        sqlite3_int64 rowid = 1;
        if (Size >= sizeof(sqlite3_int64)) {
            // Use first 8 bytes of input for rowid
            memcpy(&rowid, Data, sizeof(sqlite3_int64));
            // Ensure rowid is positive and not too large
            if (rowid < 1) rowid = 1;
            if (rowid > 1000) rowid = 1000;
        }
        sqlite3_blob_reopen(blob, rowid);
    }
    
    // Call sqlite3_blob_bytes (order 3)
    if (blob) {
        int blob_size = sqlite3_blob_bytes(blob);
        (void)blob_size; // Use result
    }
    
    // Call sqlite3_realloc64 (order 4)
    void *ptr = NULL;
    sqlite3_uint64 alloc_size = 0;
    
    if (Size >= sizeof(sqlite3_uint64)) {
        // Use next 8 bytes for allocation size
        memcpy(&alloc_size, Data + (Size >= 8 ? 8 : 0), sizeof(sqlite3_uint64));
        // Limit allocation size to prevent excessive memory usage
        if (alloc_size > 1024 * 1024) { // 1MB max
            alloc_size = 1024 * 1024;
        }
        
        // Allocate initial memory
        void *initial_ptr = sqlite3_malloc64(alloc_size / 2);
        if (initial_ptr) {
            // Reallocate it
            ptr = sqlite3_realloc64(initial_ptr, alloc_size);
            if (!ptr) {
                // If realloc fails, original pointer is still valid
                sqlite3_free(initial_ptr);
            }
        }
    }
    
    // Clean up allocated memory
    if (ptr) {
        sqlite3_free(ptr);
    }
    
    // Call sqlite3_randomness (order 5)
    int randomness_size = 0;
    if (Size >= 4) {
        // Use 4 bytes from input for randomness size
        memcpy(&randomness_size, Data + (Size >= 12 ? 12 : 0), 4);
        randomness_size &= 0xFFF; // Limit to 4096 bytes max
        if (randomness_size < 0) randomness_size = 0;
    }
    
    if (randomness_size > 0) {
        void *random_buffer = sqlite3_malloc(randomness_size);
        if (random_buffer) {
            sqlite3_randomness(randomness_size, random_buffer);
            sqlite3_free(random_buffer);
        }
    } else {
        // Test with NULL pointer to force reseed
        sqlite3_randomness(0, NULL);
    }
    
    // Clean up BLOB handle
    if (blob) {
        sqlite3_blob_close(blob);
    }
    
    // Clean up database
    if (db) {
        sqlite3_close(db);
    }
    
    // Finalize SQLite
    sqlite3_shutdown();
    
    return 0;
}