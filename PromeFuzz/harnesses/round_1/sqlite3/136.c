// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
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
#include <string.h>

static void dummy_memory_alarm_callback(void *pArg, sqlite3_int64 used, int N) {
    // Dummy callback function for sqlite3_memory_alarm
    (void)pArg;
    (void)used;
    (void)N;
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    // Use the input data to generate parameters for the API calls
    sqlite3_int64 param64 = 0;
    int param32 = 0;
    
    // Extract a 64-bit integer from the input data if available
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&param64, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        // Use available bytes to form a smaller integer
        memcpy(&param64, Data, Size);
    }
    
    // Extract a 32-bit integer from the input data if available
    if (Size >= sizeof(int)) {
        memcpy(&param32, Data, sizeof(int));
    }
    
    // 1. Test sqlite3_hard_heap_limit64
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(param64);
    // Call again with negative value to query current limit
    sqlite3_hard_heap_limit64(-1);
    // Call with zero to disable
    sqlite3_hard_heap_limit64(0);
    // Restore previous limit
    sqlite3_hard_heap_limit64(prev_hard_limit);
    
    // 2. Test sqlite3_soft_heap_limit64
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(param64);
    // Query current limit
    sqlite3_soft_heap_limit64(-1);
    // Disable soft limit
    sqlite3_soft_heap_limit64(0);
    // Restore previous limit
    sqlite3_soft_heap_limit64(prev_soft_limit);
    
    // 3. Test sqlite3_memory_alarm
    // Use different combinations of parameters
    sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, param64);
    sqlite3_memory_alarm(NULL, (void*)&param64, param64);
    sqlite3_memory_alarm(dummy_memory_alarm_callback, (void*)&param32, 0);
    
    // 4. Test sqlite3_memory_used
    sqlite3_int64 memory_used = sqlite3_memory_used();
    (void)memory_used; // Use the result to avoid unused variable warning
    
    // 5. Test sqlite3_status
    int current = 0;
    int highwater = 0;
    
    // Test with different status opcodes (common ones)
    int op_codes[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
    };
    
    for (size_t i = 0; i < sizeof(op_codes)/sizeof(op_codes[0]); i++) {
        // Use param32 to decide reset flag (odd/even)
        int reset_flag = (param32 & 1);
        sqlite3_status(op_codes[i], &current, &highwater, reset_flag);
        
        // Also test with the opposite reset flag
        sqlite3_status(op_codes[i], &current, &highwater, !reset_flag);
    }
    
    // 6. Test sqlite3_memory_highwater
    sqlite3_int64 highwater_mem = sqlite3_memory_highwater(param32 & 1);
    (void)highwater_mem;
    
    // Also test with opposite reset flag
    sqlite3_memory_highwater(!(param32 & 1));
    
    return 0;
}