// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_zeroblob64 at sqlite3.c:80145:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_transfer_bindings at sqlite3.c:80227:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_blob64 at sqlite3.c:79997:16 in sqlite3.h
// sqlite3_bind_text64 at sqlite3.c:80070:16 in sqlite3.h
// sqlite3_bind_parameter_count at sqlite3.c:80167:16 in sqlite3.h
// sqlite3_bind_text16 at sqlite3.c:80086:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_destructor(void *ptr) {
    // Do nothing - just a placeholder destructor
}

int LLVMFuzzerTestOneInput_109(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt1 = NULL;
    sqlite3_stmt *stmt2 = NULL;
    int rc;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Create a table with various column types
    const char *create_sql = "CREATE TABLE test (id INTEGER, data BLOB, text TEXT, text16 TEXT);";
    rc = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare a statement with parameters
    const char *insert_sql = "INSERT INTO test VALUES (?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt1, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Prepare another statement for transfer_bindings
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt1);
        sqlite3_close(db);
        return 0;
    }
    
    // Get parameter count
    int param_count = sqlite3_bind_parameter_count(stmt1);
    
    // Use fuzz data to determine which function to test
    // We'll use the first byte to select function, second for parameter index
    if (Size >= 2 && param_count > 0) {
        uint8_t func_selector = Data[0] % 6;  // 6 functions to choose from
        int param_idx = (Data[1] % param_count) + 1;  // Parameters are 1-indexed
        
        // Calculate remaining data for bind operations
        const uint8_t *bind_data = Data + 2;
        size_t bind_size = (Size > 2) ? Size - 2 : 0;
        
        switch (func_selector) {
            case 0:  // sqlite3_bind_blob64
                if (bind_size > 0) {
                    sqlite3_uint64 blob_size = (bind_size < 8) ? bind_size : 
                                              (*(sqlite3_uint64*)bind_data) % (1ULL << 30);  // Limit to 1GB
                    sqlite3_bind_blob64(stmt1, param_idx, bind_data, blob_size, dummy_destructor);
                }
                break;
                
            case 1:  // sqlite3_bind_text64
                if (bind_size > 0) {
                    sqlite3_uint64 text_size = (bind_size < 8) ? bind_size : 
                                              (*(sqlite3_uint64*)bind_data) % (1ULL << 30);  // Limit to 1GB
                    unsigned char encoding = (bind_size > 8) ? bind_data[8] % 3 : SQLITE_UTF8;
                    sqlite3_bind_text64(stmt1, param_idx, (const char*)bind_data, 
                                       text_size, dummy_destructor, encoding);
                }
                break;
                
            case 2:  // sqlite3_bind_parameter_count (just call it)
                // Already called above, but call again for fuzzing
                sqlite3_bind_parameter_count(stmt1);
                break;
                
            case 3:  // sqlite3_bind_text16
                if (bind_size > 0) {
                    int text16_size = (bind_size > 100) ? 100 : (int)bind_size;  // Limit size
                    // Ensure even number of bytes for UTF-16
                    text16_size = text16_size & ~1;
                    if (text16_size > 0) {
                        sqlite3_bind_text16(stmt1, param_idx, bind_data, text16_size, dummy_destructor);
                    }
                }
                break;
                
            case 4:  // sqlite3_bind_zeroblob64
                if (bind_size >= 8) {
                    sqlite3_uint64 zero_size = (*(sqlite3_uint64*)bind_data) % (1ULL << 30);  // Limit to 1GB
                    sqlite3_bind_zeroblob64(stmt1, param_idx, zero_size);
                }
                break;
                
            case 5:  // sqlite3_transfer_bindings
                // First bind some data to stmt1
                if (bind_size > 0) {
                    sqlite3_bind_blob(stmt1, 1, bind_data, 
                                     (bind_size > 100) ? 100 : (int)bind_size, 
                                     SQLITE_TRANSIENT);
                }
                // Then transfer bindings
                sqlite3_transfer_bindings(stmt1, stmt2);
                break;
        }
        
        // Try to step the statement to execute it
        sqlite3_step(stmt1);
        sqlite3_reset(stmt1);
        
        // Also try stepping the second statement
        sqlite3_step(stmt2);
        sqlite3_reset(stmt2);
    }
    
    // Cleanup
    sqlite3_finalize(stmt1);
    sqlite3_finalize(stmt2);
    sqlite3_close(db);
    
    return 0;
}