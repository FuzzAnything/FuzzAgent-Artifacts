// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_cleanup(void *ptr) {
    // Do nothing - used for bind cleanup callbacks
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Create in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a simple table for testing
    const char *create_table = "CREATE TABLE test (id INTEGER, data BLOB, text TEXT, value INTEGER);";
    sqlite3_exec(db, create_table, NULL, NULL, NULL);
    
    // Prepare a statement with parameters
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    const char *sql = "INSERT INTO test VALUES (?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt1, NULL) != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a second statement for transfer_bindings
    if (sqlite3_prepare_v2(db, sql, -1, &stmt2, NULL) != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt1);
    
    // Use fuzzer input to generate bind values
    if (Size > 0 && param_count >= 4) {
        // Extract various values from fuzzer input
        sqlite3_int64 int_val = 0;
        sqlite3_uint64 uint_val = 0;
        const void *blob_data = Data;
        const char *text_data = (const char *)Data;
        
        // Use first 8 bytes for int64 if available
        if (Size >= sizeof(sqlite3_int64)) {
            memcpy(&int_val, Data, sizeof(sqlite3_int64));
        } else {
            int_val = (sqlite3_int64)Size;
        }
        
        // Use next 8 bytes for uint64 if available
        if (Size >= 2 * sizeof(sqlite3_uint64)) {
            memcpy(&uint_val, Data + sizeof(sqlite3_uint64), sizeof(sqlite3_uint64));
        } else {
            uint_val = (sqlite3_uint64)Size;
        }
        
        // Limit blob/text sizes to prevent excessive memory usage
        size_t bind_size = Size;
        if (bind_size > 1024) {
            bind_size = 1024;
        }
        
        // Test different bind functions
        sqlite3_bind_int64(stmt1, 1, int_val);
        sqlite3_bind_blob64(stmt1, 2, blob_data, bind_size, dummy_cleanup);
        sqlite3_bind_text64(stmt1, 3, text_data, bind_size, dummy_cleanup, SQLITE_UTF8);
        sqlite3_bind_zeroblob64(stmt1, 4, uint_val % 1024); // Limit size
        
        // Execute the statement
        sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
        
        // Test transfer_bindings
        sqlite3_transfer_bindings(stmt1, stmt2);
        
        // Execute second statement
        sqlite3_step(stmt2);
        sqlite3_reset(stmt2);
        
        // Test binding with different parameter indices
        if (param_count > 1) {
            // Try binding with random indices within valid range
            for (int i = 1; i <= param_count && i <= 10; i++) {
                sqlite3_bind_int64(stmt1, i, int_val + i);
                sqlite3_bind_zeroblob64(stmt2, i, (uint_val + i) % 512);
            }
            
            // Execute with different bindings
            sqlite3_step(stmt1);
            sqlite3_reset(stmt1);
            sqlite3_step(stmt2);
            sqlite3_reset(stmt2);
        }
        
        // Test edge cases for blob64 and text64
        if (Size > 0) {
            // Test with zero size
            sqlite3_bind_blob64(stmt1, 1, NULL, 0, NULL);
            sqlite3_bind_text64(stmt1, 2, NULL, 0, NULL, SQLITE_UTF8);
            
            // Test with maximum size (using modulo to keep it reasonable)
            sqlite3_uint64 large_size = uint_val % 1048576; // Up to 1MB
            sqlite3_bind_zeroblob64(stmt1, 3, large_size);
            
            sqlite3_step(stmt1);
            sqlite3_reset(stmt1);
        }
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}