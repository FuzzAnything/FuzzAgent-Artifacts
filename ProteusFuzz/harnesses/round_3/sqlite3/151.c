// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <sqlite3.h>

static void dummy_memory_alarm_callback(void *pArg, sqlite3_int64 nCur, int nThreshold) {
    (void)pArg;
    (void)nCur;
    (void)nThreshold;
}

int LLVMFuzzerTestOneInput_151(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    int op;
    int resetFlag;
    sqlite3_int64 nLimit;
    int *pCurrent32 = NULL;
    int *pHighwater32 = NULL;
    sqlite3_int64 *pCurrent64 = NULL;
    sqlite3_int64 *pHighwater64 = NULL;

    // Allocate stack variables for output to avoid NULL pointer issues
    int current32_val = 0;
    int highwater32_val = 0;
    sqlite3_int64 current64_val = 0;
    sqlite3_int64 highwater64_val = 0;

    pCurrent32 = &current32_val;
    pHighwater32 = &highwater32_val;
    pCurrent64 = &current64_val;
    pHighwater64 = &highwater64_val;

    // Extract values from fuzz input
    if (Size >= sizeof(int)) {
        memcpy(&op, Data, sizeof(int));
    } else {
        op = 0;
    }

    if (Size >= sizeof(int) * 2) {
        memcpy(&resetFlag, Data + sizeof(int), sizeof(int));
    } else {
        resetFlag = 0;
    }

    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&nLimit, Data, sizeof(sqlite3_int64));
    } else {
        nLimit = 0;
    }

    // Fuzz sqlite3_status64
    // Only call with valid pointers since the library may not handle NULLs if API_ARMOR is disabled
    sqlite3_status64(op, pCurrent64, pHighwater64, resetFlag);
    
    // Fuzz sqlite3_status (32-bit version)
    sqlite3_status(op, pCurrent32, pHighwater32, resetFlag);

    // Fuzz sqlite3_memory_alarm
    // Try with valid callback and arguments
    sqlite3_memory_alarm(dummy_memory_alarm_callback, (void*)(uintptr_t)nLimit, nLimit);
    // Try with NULL callback (allowed by API)
    sqlite3_memory_alarm(NULL, (void*)(uintptr_t)nLimit, nLimit);
    // Try with NULL arg (allowed by API)
    sqlite3_memory_alarm(dummy_memory_alarm_callback, NULL, nLimit);
    // Try with NULL arg and callback
    sqlite3_memory_alarm(NULL, NULL, nLimit);

    // Fuzz sqlite3_soft_heap_limit64
    // Set limit
    sqlite3_soft_heap_limit64(nLimit);
    // Query limit (negative value)
    sqlite3_soft_heap_limit64(-1);
    // Set to zero
    sqlite3_soft_heap_limit64(0);
    // Set to max possible
    sqlite3_soft_heap_limit64(LLONG_MAX);
    // Set to min possible
    sqlite3_soft_heap_limit64(LLONG_MIN);

    // Fuzz sqlite3_memory_used
    sqlite3_memory_used();

    // Fuzz sqlite3_memory_highwater
    // Query without reset
    sqlite3_memory_highwater(0);
    // Query with reset
    sqlite3_memory_highwater(1);

    return 0;
}