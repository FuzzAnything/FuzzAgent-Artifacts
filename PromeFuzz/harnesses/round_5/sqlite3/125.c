// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_memory_alarm_callback(void *arg, sqlite3_int64 used, int n) {
    // Dummy callback function for sqlite3_memory_alarm
    (void)arg;
    (void)used;
    (void)n;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    // Use the input data to generate parameters for the API functions
    // We'll use the first bytes to determine various parameters
    
    if (Size < 1) return 0;
    
    // Extract parameters from input data
    // Use modulo operations to get reasonable values
    
    // For sqlite3_soft_heap_limit64 and sqlite3_hard_heap_limit64
    sqlite3_int64 limit_param = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_param, Data, sizeof(sqlite3_int64));
        Data += sizeof(sqlite3_int64);
        Size -= sizeof(sqlite3_int64);
    } else {
        // Use available bytes
        for (size_t i = 0; i < Size && i < sizeof(sqlite3_int64); i++) {
            ((uint8_t*)&limit_param)[i] = Data[i];
        }
    }
    
    // For sqlite3_memory_highwater reset flag
    int reset_flag = 0;
    if (Size >= 1) {
        reset_flag = Data[0] & 1;
        Data++;
        Size--;
    }
    
    // For sqlite3_status parameters
    int status_op = 0;
    int status_reset = 0;
    if (Size >= 2) {
        status_op = Data[0] % 10;  // There are 10 status parameters (0-9)
        status_reset = Data[1] & 1;
        Data += 2;
        Size -= 2;
    } else if (Size >= 1) {
        status_op = Data[0] % 10;
        status_reset = 0;
    }
    
    // For sqlite3_memory_alarm threshold
    sqlite3_int64 alarm_threshold = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&alarm_threshold, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        // Use available bytes
        for (size_t i = 0; i < Size && i < sizeof(sqlite3_int64); i++) {
            ((uint8_t*)&alarm_threshold)[i] = Data[i];
        }
    }
    
    // Call the target API functions
    
    // 1. sqlite3_soft_heap_limit64
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(limit_param);
    (void)prev_soft_limit;
    
    // Try different parameter values to explore more states
    sqlite3_soft_heap_limit64(-1);  // Query current limit
    sqlite3_soft_heap_limit64(0);   // Disable limit
    
    // 2. sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    (void)highwater;
    
    // Call with different reset flags
    sqlite3_memory_highwater(0);
    sqlite3_memory_highwater(1);
    
    // 3. sqlite3_status
    int current = 0;
    int highwater_status = 0;
    int status_result = sqlite3_status(status_op, &current, &highwater_status, status_reset);
    (void)status_result;
    
    // Try with different operations
    for (int i = 0; i < 3; i++) {
        int dummy_current, dummy_highwater;
        sqlite3_status(i, &dummy_current, &dummy_highwater, i & 1);
    }
    
    // 4. sqlite3_memory_used
    sqlite3_int64 memory_used = sqlite3_memory_used();
    (void)memory_used;
    
    // 5. sqlite3_memory_alarm
    int alarm_result = sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, alarm_threshold);
    (void)alarm_result;
    
    // Try with different thresholds
    sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, 0);
    sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, 1000);
    
    // 6. sqlite3_hard_heap_limit64
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(limit_param);
    (void)prev_hard_limit;
    
    // Try different parameter values
    sqlite3_hard_heap_limit64(-1);  // Query current limit
    sqlite3_hard_heap_limit64(0);   // Disable limit
    
    // Test interaction between soft and hard limits
    if (limit_param > 0) {
        sqlite3_hard_heap_limit64(limit_param);
        sqlite3_soft_heap_limit64(limit_param / 2);
    }
    
    return 0;
}