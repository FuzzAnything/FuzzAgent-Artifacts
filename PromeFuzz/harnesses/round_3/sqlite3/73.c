// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:17178:17 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:171786:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static sqlite3* create_test_db(void) {
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return NULL;
    }
    
    // Create a simple table to ensure some database structure exists
    const char* sql = "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    
    // Insert some data to potentially use memory
    sql = "INSERT INTO test (data) VALUES ('test data');";
    sqlite3_exec(db, sql, NULL, NULL, NULL);
    
    return db;
}

static void cleanup_test_db(sqlite3* db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    // Initialize variables for sqlite3_status
    int current = 0;
    int highwater = 0;
    int reset_flag = 0;
    
    // Extract parameters from fuzzer input
    if (Size >= 1) {
        // Use first byte for sqlite3_status op parameter
        // Valid op codes are SQLITE_STATUS_MEMORY_USED, SQLITE_STATUS_PAGECACHE_USED, etc.
        // We'll use modulo to get a valid-ish range
        int op = Data[0] % 20;  // There are about 20 status parameters
        
        // Use second byte for reset flag
        if (Size >= 2) {
            reset_flag = Data[1] & 1;  // Only need 0 or 1
        }
        
        // Call sqlite3_status
        sqlite3_status(op, &current, &highwater, reset_flag);
    }
    
    // Call sqlite3_soft_heap_limit with a value derived from input
    if (Size >= 4) {
        int limit_value;
        memcpy(&limit_value, Data, sizeof(int));
        sqlite3_soft_heap_limit(limit_value);
    }
    
    // Call sqlite3_release_memory
    if (Size >= 8) {
        int bytes_to_free;
        memcpy(&bytes_to_free, Data + 4, sizeof(int));
        sqlite3_release_memory(bytes_to_free);
    }
    
    // Call sqlite3_db_release_memory
    sqlite3* db = create_test_db();
    if (db) {
        sqlite3_db_release_memory(db);
        cleanup_test_db(db);
    }
    
    // Call sqlite3_memory_highwater
    if (Size >= 9) {
        int reset_highwater = Data[8] & 1;
        sqlite3_int64 highwater_mark = sqlite3_memory_highwater(reset_highwater);
        (void)highwater_mark;  // Avoid unused variable warning
    }
    
    // Call sqlite3_hard_heap_limit64
    if (Size >= 16) {
        sqlite3_int64 hard_limit;
        memcpy(&hard_limit, Data + 8, sizeof(sqlite3_int64));
        sqlite3_int64 prev_limit = sqlite3_hard_heap_limit64(hard_limit);
        (void)prev_limit;  // Avoid unused variable warning
    }
    
    // Try to explore more states by calling sqlite3_status with different parameters
    if (Size >= 17) {
        // Use different bytes for different parameter combinations
        int op2 = Data[16] % 20;
        int reset2 = (Size >= 18) ? (Data[17] & 1) : 0;
        int curr2 = 0, high2 = 0;
        sqlite3_status(op2, &curr2, &high2, reset2);
    }
    
    return 0;
}