// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sqlite3.h>

static void dummy_callback(void *pArg, sqlite3_int64 nBytes, int isAlarm) {
    (void)pArg;
    (void)nBytes;
    (void)isAlarm;
}

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    sqlite3_int64 pCurrent64 = 0;
    sqlite3_int64 pHighwater64 = 0;
    int pCurrent32 = 0;
    int pHighwater32 = 0;
    int resetFlag = 0;
    int op = 0;
    sqlite3_int64 limit = 0;

    // Extract values from input data to drive fuzzing safely
    if (Size >= 4) {
        op = *(int32_t *)(Data);
    }
    if (Size >= 12) {
        limit = *(sqlite3_int64 *)(Data + 4);
    }
    
    // Ensure resetFlag is 0 or 1, checking bounds first
    if (Size >= 9) {
        resetFlag = (Data[8] % 2) == 1 ? 1 : 0;
    }

    // 1. sqlite3_status64
    // Test with valid pointers
    sqlite3_status64(op, &pCurrent64, &pHighwater64, resetFlag);

    // 2. sqlite3_memory_alarm
    // Test with valid callback and arguments
    sqlite3_memory_alarm(dummy_callback, (void *)Data, limit);
    // Test with NULL callback (should be safe as it's a stub)
    sqlite3_memory_alarm(NULL, (void *)Data, limit);
    // Test with NULL context
    sqlite3_memory_alarm(dummy_callback, NULL, limit);
    // Test with extreme limit values
    sqlite3_memory_alarm(dummy_callback, (void *)Data, -1);
    sqlite3_memory_alarm(dummy_callback, (void *)Data, 0);

    // 3. sqlite3_soft_heap_limit64
    // Test with various limits derived from input
    sqlite3_int64 prevLimit = sqlite3_soft_heap_limit64(limit);
    (void)prevLimit;
    // Query current limit
    sqlite3_soft_heap_limit64(-1);
    // Set to 0 (disable)
    sqlite3_soft_heap_limit64(0);
    // Set to a large value using LLONG_MAX from limits.h
    sqlite3_soft_heap_limit64(LLONG_MAX);

    // 4. sqlite3_memory_used
    sqlite3_int64 memUsed = sqlite3_memory_used();
    (void)memUsed;

    // 5. sqlite3_status
    // Test with valid pointers
    sqlite3_status(op, &pCurrent32, &pHighwater32, resetFlag);

    // 6. sqlite3_memory_highwater
    // Query without reset
    sqlite3_int64 hw = sqlite3_memory_highwater(0);
    // Query with reset
    sqlite3_int64 hwReset = sqlite3_memory_highwater(1);
    (void)hw;
    (void)hwReset;

    return 0;
}