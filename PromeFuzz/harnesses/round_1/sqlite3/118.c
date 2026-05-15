// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Use input data to generate parameters for the API functions
    // We'll use various bytes from the input to create different parameter values
    
    // For 64-bit integer parameters, we'll construct from the input data
    sqlite3_int64 limit_val = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_val, Data, sizeof(sqlite3_int64));
    } else {
        // Use available bytes
        memcpy(&limit_val, Data, Size);
    }
    
    // Test sqlite3_soft_heap_limit64 with various values
    // Use different values based on input bytes
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(limit_val);
    
    // Test with -1 to query current limit
    sqlite3_int64 current_soft_limit = sqlite3_soft_heap_limit64(-1);
    
    // Test with 0 to disable
    sqlite3_int64 disabled_soft_limit = sqlite3_soft_heap_limit64(0);
    
    // Restore previous limit
    sqlite3_soft_heap_limit64(prev_soft_limit);
    
    // Test sqlite3_hard_heap_limit64
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(limit_val);
    
    // Test with -1 to query
    sqlite3_int64 current_hard_limit = sqlite3_hard_heap_limit64(-1);
    
    // Test with 0 to disable
    sqlite3_int64 disabled_hard_limit = sqlite3_hard_heap_limit64(0);
    
    // Restore previous hard limit
    sqlite3_hard_heap_limit64(prev_hard_limit);
    
    // Test sqlite3_status64
    // Use input data to determine operation code, reset flag
    int op = 0;
    int resetFlag = 0;
    
    if (Size >= 1) {
        op = Data[0] % 10;  // Use modulo to get valid-ish operation codes
    }
    if (Size >= 2) {
        resetFlag = Data[1] & 1;  // Use LSB for reset flag (0 or 1)
    }
    
    sqlite3_int64 current = 0;
    sqlite3_int64 highwater = 0;
    
    // Test with various operation codes
    int status_result = sqlite3_status64(op, &current, &highwater, resetFlag);
    
    // Test sqlite3_memory_used
    sqlite3_int64 memory_used = sqlite3_memory_used();
    
    // Test sqlite3_status (32-bit version)
    int current32 = 0;
    int highwater32 = 0;
    
    // Use different op codes for 32-bit version
    int op32 = 0;
    if (Size >= 3) {
        op32 = Data[2] % 10;
    }
    
    int status32_result = sqlite3_status(op32, &current32, &highwater32, resetFlag);
    
    // Test sqlite3_memory_highwater
    // Use different reset flags based on input
    int resetFlag2 = 0;
    if (Size >= 4) {
        resetFlag2 = Data[3] & 1;
    }
    
    sqlite3_int64 memory_highwater = sqlite3_memory_highwater(resetFlag2);
    
    // Test with the other reset flag value
    int alternate_reset = resetFlag2 ^ 1;  // Flip the bit
    sqlite3_int64 memory_highwater_alt = sqlite3_memory_highwater(alternate_reset);
    
    // Test edge cases for heap limits
    // Test with various special values
    sqlite3_int64 test_values[] = {0, -1, 1, 1000, 1000000, -1000};
    for (int i = 0; i < 6 && i * 8 < (int)Size; i++) {
        sqlite3_int64 test_val = test_values[i];
        if (i * 8 + 8 <= (int)Size) {
            memcpy(&test_val, Data + i * 8, 8);
        }
        
        sqlite3_int64 prev = sqlite3_soft_heap_limit64(test_val);
        sqlite3_soft_heap_limit64(prev);
        
        prev = sqlite3_hard_heap_limit64(test_val);
        sqlite3_hard_heap_limit64(prev);
    }
    
    // Test status with various operation codes and reset flags
    for (int test_op = 0; test_op < 10; test_op++) {
        for (int test_reset = 0; test_reset < 2; test_reset++) {
            sqlite3_int64 cur = 0, high = 0;
            sqlite3_status64(test_op, &cur, &high, test_reset);
            
            int cur32 = 0, high32 = 0;
            sqlite3_status(test_op, &cur32, &high32, test_reset);
        }
    }
    
    return 0;
}