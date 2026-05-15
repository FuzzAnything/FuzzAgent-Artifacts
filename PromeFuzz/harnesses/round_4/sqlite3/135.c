// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:17130:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_memory_alarm_callback(void *arg, sqlite3_int64 used, int flags) {
    // Do nothing - this is a dummy callback for sqlite3_memory_alarm
}

int LLVMFuzzerTestOneInput_135(const uint8_t *Data, size_t Size) {
    // Use the fuzzer input to generate parameters for the API calls
    // We'll use the first few bytes to determine various parameters
    
    if (Size < 1) {
        return 0;
    }
    
    // Use Data[0] to determine reset flags and operation codes
    uint8_t selector = Data[0];
    
    // Call sqlite3_memory_highwater with resetFlag based on input
    int resetFlag = (selector & 0x01) ? 1 : 0;
    sqlite3_memory_highwater(resetFlag);
    
    // Call sqlite3_status with various operation codes
    // SQLITE_STATUS_MEMORY_USED = 0, SQLITE_STATUS_MALLOC_COUNT = 6, etc.
    // We'll cycle through different status codes
    int status_op = selector % 15;  // There are 15 status codes (0-14)
    int current = 0;
    int highwater = 0;
    int status_resetFlag = (selector & 0x02) ? 1 : 0;
    sqlite3_status(status_op, &current, &highwater, status_resetFlag);
    
    // Call sqlite3_memory_alarm - this function currently does nothing
    // but we can still call it with dummy parameters
    sqlite3_int64 alarm_threshold = 0;
    if (Size >= 9) {
        // Use bytes 1-8 to form a 64-bit threshold
        memcpy(&alarm_threshold, Data + 1, sizeof(sqlite3_int64));
    }
    sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, alarm_threshold);
    
    // Call sqlite3_memory_used - no parameters needed
    sqlite3_memory_used();
    
    // Call sqlite3_soft_heap_limit64
    sqlite3_int64 soft_limit = -1;  // Default to query current limit
    if (Size >= 17) {
        // Use bytes 9-16 to form a 64-bit limit
        memcpy(&soft_limit, Data + 9, sizeof(sqlite3_int64));
    }
    sqlite3_soft_heap_limit64(soft_limit);
    
    // Call sqlite3_hard_heap_limit64
    sqlite3_int64 hard_limit = -1;  // Default to query current limit
    if (Size >= 25) {
        // Use bytes 17-24 to form a 64-bit limit
        memcpy(&hard_limit, Data + 17, sizeof(sqlite3_int64));
    }
    sqlite3_hard_heap_limit64(hard_limit);
    
    // Try calling sqlite3_status again with a different op code
    // to explore more states
    int status_op2 = (selector + 1) % 15;
    sqlite3_status(status_op2, &current, &highwater, 0);
    
    // Call sqlite3_memory_highwater again with opposite reset flag
    sqlite3_memory_highwater(!resetFlag);
    
    return 0;
}