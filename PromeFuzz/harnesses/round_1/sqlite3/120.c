// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
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

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    // Convert input data to parameters for the functions
    sqlite3_int64 limit_param = 0;
    int status_op = 0;
    int reset_flag = 0;
    
    // Use first 8 bytes for limit parameter if available
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_param, Data, sizeof(sqlite3_int64));
        Data += sizeof(sqlite3_int64);
        Size -= sizeof(sqlite3_int64);
    } else if (Size > 0) {
        // Use available bytes for limit parameter
        memcpy(&limit_param, Data, Size);
        limit_param = limit_param & ((1ULL << (8 * Size)) - 1);
        Size = 0;
    }
    
    // Use next byte for status operation if available
    if (Size >= 1) {
        status_op = Data[0];
        Data += 1;
        Size -= 1;
    }
    
    // Use next byte for reset flag if available
    if (Size >= 1) {
        reset_flag = Data[0] & 1;  // Only use LSB for boolean
        Data += 1;
        Size -= 1;
    }
    
    // Test sqlite3_soft_heap_limit64
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(limit_param);
    // Test with -1 to query current limit
    sqlite3_soft_heap_limit64(-1);
    // Test with 0 to disable
    sqlite3_soft_heap_limit64(0);
    // Restore previous limit
    sqlite3_soft_heap_limit64(prev_soft_limit);
    
    // Test sqlite3_hard_heap_limit64
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(limit_param);
    // Test with -1 to query current limit
    sqlite3_hard_heap_limit64(-1);
    // Test with 0 to disable
    sqlite3_hard_heap_limit64(0);
    // Restore previous limit
    sqlite3_hard_heap_limit64(prev_hard_limit);
    
    // Test sqlite3_status64
    sqlite3_int64 current64 = 0;
    sqlite3_int64 highwater64 = 0;
    sqlite3_status64(status_op, &current64, &highwater64, reset_flag);
    // Test with different reset flags
    sqlite3_status64(status_op, &current64, &highwater64, 0);
    sqlite3_status64(status_op, &current64, &highwater64, 1);
    
    // Test sqlite3_memory_used
    sqlite3_int64 memory_used = sqlite3_memory_used();
    
    // Test sqlite3_status
    int current = 0;
    int highwater = 0;
    sqlite3_status(status_op, &current, &highwater, reset_flag);
    // Test with different reset flags
    sqlite3_status(status_op, &current, &highwater, 0);
    sqlite3_status(status_op, &current, &highwater, 1);
    
    // Test sqlite3_memory_highwater
    sqlite3_int64 highwater_mem = sqlite3_memory_highwater(reset_flag);
    // Test with different reset flags
    sqlite3_memory_highwater(0);
    sqlite3_memory_highwater(1);
    
    return 0;
}