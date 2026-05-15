// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Parse input data to generate parameters for the API functions
    // We'll use the first few bytes to determine operation codes and flags
    
    if (Size < 4) {
        // Not enough data for meaningful fuzzing
        return 0;
    }
    
    // Extract operation codes and flags from input data
    int op1 = Data[0] % 100;  // For sqlite3_status and sqlite3_status64
    int op2 = Data[1] % 100;  // For sqlite3_db_status64
    int resetFlag1 = Data[2] & 1;
    int resetFlag2 = Data[3] & 1;
    
    // Variables to store results
    int current_int = 0;
    int highwater_int = 0;
    sqlite3_int64 current_int64 = 0;
    sqlite3_int64 highwater_int64 = 0;
    
    // 1. Test sqlite3_status
    sqlite3_status(op1, &current_int, &highwater_int, resetFlag1);
    
    // 2. Test sqlite3_status64
    sqlite3_status64(op1, &current_int64, &highwater_int64, resetFlag1);
    
    // 3. Test sqlite3_db_status64 - requires a database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db != NULL) {
        sqlite3_db_status64(db, op2, &current_int64, &highwater_int64, resetFlag2);
        
        // Clean up
        sqlite3_close(db);
    }
    
    // 4. Test sqlite3_soft_heap_limit64
    // Use some bytes from input to generate limit value
    sqlite3_int64 limit = 0;
    if (Size >= 12) {
        // Safe to read 8 bytes for limit
        memcpy(&limit, Data + 4, sizeof(sqlite3_int64));
    } else if (Size >= 5) {
        // Safe to read Data[4]
        limit = (sqlite3_int64)Data[4] * 1024;  // Simple scaling
    } else {
        // Not enough data, use default
        limit = 0;
    }
    sqlite3_soft_heap_limit64(limit);
    
    // Also test querying current limit
    sqlite3_soft_heap_limit64(-1);
    
    // 5. Test sqlite3_memory_used
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;  // Prevent unused variable warning
    
    // 6. Test sqlite3_memory_highwater
    int resetFlag3 = (Size > 0) ? (Data[0] & 1) : 0;
    sqlite3_int64 highwater = sqlite3_memory_highwater(resetFlag3);
    (void)highwater;  // Prevent unused variable warning
    
    // Test with different reset flags
    sqlite3_memory_highwater(0);
    sqlite3_memory_highwater(1);
    
    // Additional fuzzing: try different operation codes
    // SQLite defines various status parameters, let's try a few common ones
    int ops[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (size_t i = 0; i < sizeof(ops)/sizeof(ops[0]) && i < Size; i++) {
        int op = ops[i] % 11;  // There are 11 standard SQLITE_STATUS parameters
        sqlite3_status(op, &current_int, &highwater_int, Data[i] & 1);
        sqlite3_status64(op, &current_int64, &highwater_int64, Data[i] & 1);
        
        // Try with database if we have one
        if (i == 0) {
            rc = sqlite3_open(":memory:", &db);
            if (rc == SQLITE_OK && db != NULL) {
                // Try different db status operations (0-6 are valid)
                int db_op = op % 7;
                sqlite3_db_status64(db, db_op, &current_int64, &highwater_int64, Data[i] & 1);
                sqlite3_close(db);
            }
        }
    }
    
    // Test soft heap limit with various values
    for (size_t i = 0; i < 10 && i < Size; i++) {
        sqlite3_int64 test_limit = (sqlite3_int64)Data[i] * 1024 * 1024;
        sqlite3_soft_heap_limit64(test_limit);
    }
    
    // Reset to default (0 disables)
    sqlite3_soft_heap_limit64(0);
    
    return 0;
}