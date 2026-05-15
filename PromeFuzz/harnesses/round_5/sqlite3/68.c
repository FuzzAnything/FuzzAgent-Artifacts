// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    // Initialize variables for API calls
    sqlite3_int64 soft_limit_result, highwater_result, memory_used_result;
    int status_result, status64_result, db_status64_result;
    int current_val, highwater_val;
    sqlite3_int64 current64_val, highwater64_val;
    
    // Extract parameters from fuzzer input
    // Use first 8 bytes for sqlite3_int64 parameters
    sqlite3_int64 limit_param = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_param, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        // Use available bytes
        memcpy(&limit_param, Data, Size);
    }
    
    // Use next byte for reset flags
    int reset_flag = 0;
    if (Size >= sizeof(sqlite3_int64) + 1) {
        reset_flag = Data[sizeof(sqlite3_int64)] & 1;
    }
    
    // Use next byte for operation codes
    int op_code = 0;
    if (Size >= sizeof(sqlite3_int64) + 2) {
        op_code = Data[sizeof(sqlite3_int64) + 1] % 10; // Limit to valid range
    }
    
    // Use next byte for db operation codes
    int db_op_code = 0;
    if (Size >= sizeof(sqlite3_int64) + 3) {
        db_op_code = Data[sizeof(sqlite3_int64) + 2] % 13; // Limit to valid range
    }
    
    // Use next byte for db reset flag
    int db_reset_flag = 0;
    if (Size >= sizeof(sqlite3_int64) + 4) {
        db_reset_flag = Data[sizeof(sqlite3_int64) + 3] & 1;
    }
    
    // Call sqlite3_soft_heap_limit64 with various parameters
    // Test with query parameter (-1)
    soft_limit_result = sqlite3_soft_heap_limit64(-1);
    
    // Test with disable parameter (0)
    soft_limit_result = sqlite3_soft_heap_limit64(0);
    
    // Test with fuzzer-provided parameter
    soft_limit_result = sqlite3_soft_heap_limit64(limit_param);
    
    // Test with negative values (should not change limit)
    soft_limit_result = sqlite3_soft_heap_limit64(-limit_param);
    
    // Call sqlite3_memory_highwater
    highwater_result = sqlite3_memory_highwater(0); // Don't reset
    highwater_result = sqlite3_memory_highwater(reset_flag); // Possibly reset
    
    // Call sqlite3_status with various operation codes
    status_result = sqlite3_status(op_code, &current_val, &highwater_val, reset_flag);
    
    // Test with different operation codes
    for (int i = 0; i < 10; i++) {
        int test_op = (op_code + i) % 10;
        status_result = sqlite3_status(test_op, &current_val, &highwater_val, i & 1);
    }
    
    // Call sqlite3_memory_used
    memory_used_result = sqlite3_memory_used();
    
    // Call sqlite3_status64
    status64_result = sqlite3_status64(op_code, &current64_val, &highwater64_val, reset_flag);
    
    // Test with different operation codes for status64
    for (int i = 0; i < 10; i++) {
        int test_op = (op_code + i) % 10;
        status64_result = sqlite3_status64(test_op, &current64_val, &highwater64_val, (i + reset_flag) & 1);
    }
    
    // Create a dummy database for sqlite3_db_status64
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db != NULL) {
        // Call sqlite3_db_status64 with various operation codes
        db_status64_result = sqlite3_db_status64(db, db_op_code, &current64_val, &highwater64_val, db_reset_flag);
        
        // Test with different operation codes
        for (int i = 0; i < 13; i++) {
            int test_db_op = (db_op_code + i) % 13;
            db_status64_result = sqlite3_db_status64(db, test_db_op, &current64_val, &highwater64_val, (i + db_reset_flag) & 1);
        }
        
        // Clean up
        sqlite3_close(db);
    }
    
    // Test edge cases for soft heap limit
    // Test with very large values
    soft_limit_result = sqlite3_soft_heap_limit64(INT64_MAX);
    soft_limit_result = sqlite3_soft_heap_limit64(INT64_MIN);
    
    // Test with alternating queries and sets
    sqlite3_soft_heap_limit64(1000000);
    soft_limit_result = sqlite3_soft_heap_limit64(-1);
    sqlite3_soft_heap_limit64(0);
    soft_limit_result = sqlite3_soft_heap_limit64(-1);
    
    // Test memory functions in sequence
    memory_used_result = sqlite3_memory_used();
    highwater_result = sqlite3_memory_highwater(1); // Reset highwater
    memory_used_result = sqlite3_memory_used();
    highwater_result = sqlite3_memory_highwater(0);
    
    return 0;
}