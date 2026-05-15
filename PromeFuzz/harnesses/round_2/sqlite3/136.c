// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>

static void dummy_memory_alarm_callback(void *pArg, sqlite3_int64 used, int N) {
    // Do nothing - this is a dummy callback for sqlite3_memory_alarm
}

int LLVMFuzzerTestOneInput_136(const uint8_t *Data, size_t Size) {
    // Use the input data to generate parameters for the API functions
    // We'll use different parts of the input for different parameters
    
    // For 64-bit integer parameters, use up to 8 bytes from input
    sqlite3_int64 limit_param = 0;
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_param, Data, sizeof(sqlite3_int64));
    } else if (Size > 0) {
        // Use available bytes
        memcpy(&limit_param, Data, Size);
    }
    
    // For integer parameters (resetFlag, status op), use first byte
    int int_param = 0;
    if (Size > 0) {
        int_param = Data[0];
    }
    
    // Call sqlite3_soft_heap_limit64 with various limit values
    // Use different parts of input to generate diverse calls
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(limit_param);
    
    // Try querying current limit
    sqlite3_int64 current_soft_limit = sqlite3_soft_heap_limit64(-1);
    
    // Try disabling
    sqlite3_soft_heap_limit64(0);
    
    // Restore previous limit
    sqlite3_soft_heap_limit64(prev_soft_limit);
    
    // Call sqlite3_hard_heap_limit64
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(limit_param);
    
    // Query current hard limit
    sqlite3_int64 current_hard_limit = sqlite3_hard_heap_limit64(-1);
    
    // Try disabling hard limit
    sqlite3_hard_heap_limit64(0);
    
    // Restore previous hard limit
    sqlite3_hard_heap_limit64(prev_hard_limit);
    
    // Call sqlite3_memory_highwater with different reset flags
    // Use int_param to determine reset flag (0 or 1)
    int reset_flag = int_param & 1;
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    
    // Try with opposite reset flag
    sqlite3_int64 highwater2 = sqlite3_memory_highwater(!reset_flag);
    
    // Call sqlite3_memory_used
    sqlite3_int64 current_memory = sqlite3_memory_used();
    
    // Call sqlite3_status with various operation codes
    // Use int_param to select from common status codes
    int status_op = int_param % 10;  // 0-9 for common status codes
    int current_value = 0;
    int highwater_value = 0;
    
    // Try with reset flag on
    int status_result = sqlite3_status(status_op, &current_value, &highwater_value, 1);
    
    // Try with reset flag off
    status_result = sqlite3_status(status_op, &current_value, &highwater_value, 0);
    
    // Try with some other common status codes
    status_result = sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current_value, &highwater_value, reset_flag);
    status_result = sqlite3_status(SQLITE_STATUS_PAGECACHE_USED, &current_value, &highwater_value, reset_flag);
    status_result = sqlite3_status(SQLITE_STATUS_MALLOC_COUNT, &current_value, &highwater_value, reset_flag);
    
    // Call sqlite3_memory_alarm (currently does nothing but we call it anyway)
    // Use limit_param as the threshold
    int alarm_result = sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, limit_param);
    
    // Try with different callback (NULL)
    alarm_result = sqlite3_memory_alarm(NULL, NULL, limit_param / 2);
    
    // Try with zero threshold
    alarm_result = sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, 0);
    
    // Try with negative threshold
    alarm_result = sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, -1);
    
    return 0;
}