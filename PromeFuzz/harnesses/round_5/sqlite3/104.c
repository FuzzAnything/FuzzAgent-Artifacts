// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:17178:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    // Initialize SQLite if needed
    sqlite3_initialize();
    
    // Create a database connection for testing
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    
    if (rc != SQLITE_OK || db == NULL) {
        // If we can't open a database, we can still test some functions
        // that don't require a database connection
    } else {
        // Create a simple table and insert some data to have something in the database
        char *err_msg = NULL;
        sqlite3_exec(db, "CREATE TABLE test (id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, &err_msg);
        sqlite3_exec(db, "INSERT INTO test (data) VALUES ('test data');", NULL, NULL, &err_msg);
        
        // Test sqlite3_db_release_memory
        if (db) {
            sqlite3_db_release_memory(db);
        }
        
        // Clean up error message if any
        if (err_msg) {
            sqlite3_free(err_msg);
            err_msg = NULL;
        }
    }
    
    // Test sqlite3_release_memory
    // Use first byte of input as number of bytes to try to release
    int bytes_to_release = 0;
    if (Size > 0) {
        bytes_to_release = Data[0] * 1024; // Scale up to make it more interesting
    }
    sqlite3_release_memory(bytes_to_release);
    
    // Test sqlite3_soft_heap_limit (deprecated but still testable)
    // Use second byte as limit value
    int soft_limit = 0;
    if (Size > 1) {
        soft_limit = Data[1] * 1024 * 1024; // Scale to MB range
    }
    sqlite3_soft_heap_limit(soft_limit);
    
    // Test sqlite3_memory_highwater
    // Use third byte to decide whether to reset (0 = no reset, non-zero = reset)
    int reset_flag = 0;
    if (Size > 2) {
        reset_flag = (Data[2] & 0x01); // Use only LSB for 0/1 decision
    }
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    (void)highwater; // Mark as used to avoid compiler warning
    
    // Test sqlite3_status with various operations
    // We'll test a few different status parameters
    int current = 0;
    int highwater_val = 0;
    
    // Use input data to select which status parameters to query
    if (Size > 0) {
        // Test SQLITE_STATUS_MEMORY_USED (0)
        sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater_val, 0);
        
        // Test SQLITE_STATUS_MALLOC_COUNT (9) if we have enough data
        if (Size > 3) {
            int reset = (Data[3] & 0x01);
            sqlite3_status(SQLITE_STATUS_MALLOC_COUNT, &current, &highwater_val, reset);
        }
        
        // Test SQLITE_STATUS_PAGECACHE_USED (1) if we have enough data
        if (Size > 4) {
            int reset = (Data[4] & 0x01);
            sqlite3_status(SQLITE_STATUS_PAGECACHE_USED, &current, &highwater_val, reset);
        }
        
        // Test a few more if we have data
        if (Size > 8) {
            // Use bytes 5-8 to create a parameter value, modulo the number of status parameters
            int param = 0;
            for (size_t i = 5; i < 9 && i < Size; i++) {
                param = (param << 8) | Data[i];
            }
            // SQLITE_STATUS parameters go from 0 to about 13
            param = param % 14; // Ensure it's a valid parameter range
            int reset = (Data[5] & 0x01);
            sqlite3_status(param, &current, &highwater_val, reset);
        }
    }
    
    // Test sqlite3_hard_heap_limit64
    // Use input data to create a 64-bit limit value
    sqlite3_int64 hard_limit = -1; // Default: don't change limit
    if (Size >= 8) {
        // Use first 8 bytes to form the limit
        hard_limit = 0;
        for (size_t i = 0; i < 8 && i < Size; i++) {
            hard_limit = (hard_limit << 8) | Data[i];
        }
    } else if (Size > 0) {
        // For smaller inputs, use what we have
        hard_limit = 0;
        for (size_t i = 0; i < Size; i++) {
            hard_limit = (hard_limit << 8) | Data[i];
        }
        // Scale it to be in a reasonable range
        hard_limit = hard_limit * 1024;
    }
    
    sqlite3_int64 prev_limit = sqlite3_hard_heap_limit64(hard_limit);
    (void)prev_limit; // Mark as used to avoid compiler warning
    
    // If we set a limit, try setting it back to something else
    if (hard_limit >= 0 && Size > 8) {
        // Use bytes 8-15 if available to set another limit
        sqlite3_int64 another_limit = -1;
        if (Size >= 16) {
            another_limit = 0;
            for (size_t i = 8; i < 16 && i < Size; i++) {
                another_limit = (another_limit << 8) | Data[i];
            }
        }
        sqlite3_hard_heap_limit64(another_limit);
    }
    
    // Clean up database if we opened one
    if (db) {
        sqlite3_close(db);
    }
    
    // Shutdown SQLite
    sqlite3_shutdown();
    
    return 0;
}