// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_memory_alarm_cb(void *pArg, sqlite3_int64 used, int N) {
    // Dummy callback function for sqlite3_memory_alarm
    (void)pArg;
    (void)used;
    (void)N;
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    // Use the input data to generate parameters for the API calls
    // We'll use the first bytes to determine various parameters
    
    if (Size == 0) {
        return 0;
    }
    
    // Extract parameters from fuzzing input
    int op = Data[0];
    int resetFlag1 = (Size > 1) ? (Data[1] & 1) : 0;
    int resetFlag2 = (Size > 2) ? (Data[2] & 1) : 0;
    
    // Extract 64-bit values from input (using up to 8 bytes each)
    sqlite3_int64 N1 = 0, N2 = 0;
    size_t offset = 3;
    
    if (Size >= offset + 8) {
        memcpy(&N1, Data + offset, 8);
        offset += 8;
    } else if (Size > offset) {
        memcpy(&N1, Data + offset, Size - offset);
    }
    
    if (Size >= offset + 8) {
        memcpy(&N2, Data + offset, 8);
    } else if (Size > offset) {
        memcpy(&N2, Data + offset, Size - offset);
    }
    
    // Call sqlite3_status with various operations
    int current = 0, highwater = 0;
    int result = 0;
    
    // Try different status operations based on input
    int status_op = op % 10; // Limit to reasonable range
    result = sqlite3_status(status_op, &current, &highwater, resetFlag1);
    (void)result; // Ignore return value for fuzzing
    
    // Call sqlite3_memory_alarm (currently does nothing but always returns SQLITE_OK)
    result = sqlite3_memory_alarm(dummy_memory_alarm_cb, NULL, N1);
    (void)result;
    
    // Call sqlite3_soft_heap_limit64
    sqlite3_int64 soft_limit_result = sqlite3_soft_heap_limit64(N1);
    (void)soft_limit_result;
    
    // Call sqlite3_memory_used
    sqlite3_int64 memory_used = sqlite3_memory_used();
    (void)memory_used;
    
    // Call sqlite3_memory_highwater
    sqlite3_int64 highwater_mem = sqlite3_memory_highwater(resetFlag2);
    (void)highwater_mem;
    
    // Call sqlite3_hard_heap_limit64
    sqlite3_int64 hard_limit_result = sqlite3_hard_heap_limit64(N2);
    (void)hard_limit_result;
    
    // Try sqlite3_status again with different parameters
    status_op = (op + 1) % 10;
    result = sqlite3_status(status_op, &current, &highwater, resetFlag2);
    (void)result;
    
    // Try sqlite3_soft_heap_limit64 with negative value (query current limit)
    sqlite3_int64 current_soft_limit = sqlite3_soft_heap_limit64(-1);
    (void)current_soft_limit;
    
    // Try sqlite3_hard_heap_limit64 with negative value (query current limit)
    sqlite3_int64 current_hard_limit = sqlite3_hard_heap_limit64(-1);
    (void)current_hard_limit;
    
    // Try sqlite3_soft_heap_limit64 with zero (disable)
    sqlite3_soft_heap_limit64(0);
    
    // Try sqlite3_hard_heap_limit64 with zero (disable)
    sqlite3_hard_heap_limit64(0);
    
    // Call memory functions again to see if limits had any effect
    memory_used = sqlite3_memory_used();
    (void)memory_used;
    
    highwater_mem = sqlite3_memory_highwater(0);
    (void)highwater_mem;
    
    return 0;
}