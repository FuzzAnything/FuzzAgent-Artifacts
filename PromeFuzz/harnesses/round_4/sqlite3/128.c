// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_bind_zeroblob at sqlite3.c:80130:16 in sqlite3.h
// sqlite3_bind_int64 at sqlite3.c:80021:16 in sqlite3.h
// sqlite3_clear_bindings at sqlite3.c:78413:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_destructor(void *ptr) {
    // Do nothing - just a placeholder destructor
}

int LLVMFuzzerTestOneInput_128(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table with parameters
    const char *create_table = 
        "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, "
        "data BLOB, value INTEGER);";
    rc = sqlite3_exec(db, create_table, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with parameters
    const char *insert_sql = 
        "INSERT INTO test (data, value) VALUES (?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK || stmt == NULL) {
        sqlite3_close(db);
        return 0;
    }
    
    // Use the fuzzer input to generate parameters
    if (Size > 0) {
        // Get parameter count
        int param_count = sqlite3_bind_parameter_count(stmt);
        
        // Bind parameters based on available data
        if (param_count >= 1 && Size >= 1) {
            // Use first byte as index selector
            uint8_t index_selector = Data[0];
            
            // For blob64 binding
            if (Size >= 10) {  // Need at least 10 bytes for index + size + some data
                int idx1 = (index_selector % param_count) + 1;
                sqlite3_uint64 blob_size = 0;
                
                // Read blob size from input (little-endian)
                for (int i = 0; i < 8 && i + 1 < Size; i++) {
                    blob_size |= ((sqlite3_uint64)Data[i + 1] << (i * 8));
                }
                
                // Cap the blob size to prevent excessive memory usage
                if (blob_size > 1000) blob_size = 1000;
                
                // Bind blob64 if we have enough data
                if (Size >= 9 + blob_size) {
                    const void *blob_data = Data + 9;
                    sqlite3_bind_blob64(stmt, idx1, blob_data, 
                                      blob_size < Size - 9 ? blob_size : Size - 9, 
                                      dummy_destructor);
                }
            }
            
            // For zeroblob64 binding
            if (Size >= 9) {
                int idx2 = ((index_selector + 1) % param_count) + 1;
                sqlite3_uint64 zero_blob_size = 0;
                
                // Read size from input (different offset)
                for (int i = 0; i < 8 && i + 1 < Size; i++) {
                    zero_blob_size |= ((sqlite3_uint64)Data[(i + 5) % Size] << (i * 8));
                }
                
                // Cap the size
                if (zero_blob_size > 1000) zero_blob_size = 1000;
                sqlite3_bind_zeroblob64(stmt, idx2, zero_blob_size);
            }
            
            // For zeroblob binding (32-bit size)
            if (Size >= 5) {
                int idx3 = ((index_selector + 2) % param_count) + 1;
                int zero_size = 0;
                
                // Read 32-bit size from input
                for (int i = 0; i < 4 && i + 2 < Size; i++) {
                    zero_size |= ((int)Data[(i + 3) % Size] << (i * 8));
                }
                
                // Cap the size
                if (zero_size > 1000) zero_size = 1000;
                if (zero_size < 0) zero_size = 0;
                sqlite3_bind_zeroblob(stmt, idx3, zero_size);
            }
            
            // For int64 binding
            if (Size >= 9) {
                int idx4 = ((index_selector + 3) % param_count) + 1;
                sqlite3_int64 int_value = 0;
                
                // Read int64 from input
                for (int i = 0; i < 8 && i + 2 < Size; i++) {
                    int_value |= ((sqlite3_int64)Data[(i + 7) % Size] << (i * 8));
                }
                sqlite3_bind_int64(stmt, idx4, int_value);
            }
        }
        
        // Test clear_bindings
        if (Size > 1 && (Data[0] % 2 == 0)) {
            sqlite3_clear_bindings(stmt);
        }
        
        // Reset and try again with different parameters
        sqlite3_reset(stmt);
        
        // Try a different combination if we have enough data
        if (Size > 20) {
            // Use different bytes for parameter selection
            uint8_t alt_selector = Data[Size / 2];
            
            // Bind zeroblob with different size
            if (param_count >= 1) {
                int idx = (alt_selector % param_count) + 1;
                int alt_size = Data[Size / 3] * 256 + Data[Size / 4];
                if (alt_size > 1000) alt_size = 1000;
                if (alt_size < 0) alt_size = 0;
                sqlite3_bind_zeroblob(stmt, idx, alt_size);
            }
            
            // Bind int64 with different value
            if (param_count >= 2) {
                int idx = ((alt_selector + 1) % param_count) + 1;
                sqlite3_int64 alt_int = 0;
                for (int i = 0; i < 8 && Size/5 + i < Size; i++) {
                    alt_int |= ((sqlite3_int64)Data[Size/5 + i] << (i * 8));
                }
                sqlite3_bind_int64(stmt, idx, alt_int);
            }
            
            // Test clear_bindings again
            if (alt_selector % 3 == 0) {
                sqlite3_clear_bindings(stmt);
            }
        }
    }
    
    // Clean up
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}