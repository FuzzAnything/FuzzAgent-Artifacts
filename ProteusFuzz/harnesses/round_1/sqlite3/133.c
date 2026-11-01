// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_memory_callback(void *pArg, sqlite3_int64 nByte, int nAlloc) {
    (void)pArg;
    (void)nByte;
    (void)nAlloc;
}

static void dummy_memory_callback2(void *pArg, sqlite3_int64 nByte, int nAlloc) {
    (void)pArg;
    (void)nByte;
    (void)nAlloc;
}

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    sqlite3_int64 pCurrent64 = 0;
    sqlite3_int64 pHighwater64 = 0;
    int pCurrent32 = 0;
    int pHighwater32 = 0;
    int resetFlag = 0;
    int op = 0;
    sqlite3_int64 limitVal = 0;

    // Parse input to drive fuzzing logic
    unsigned int seed = Data[0];

    // 1. Test sqlite3_memory_alarm
    // Try different callback pointers and thresholds
    void (*callback1)(void*, sqlite3_int64, int) = (seed % 2) ? dummy_memory_callback : NULL;
    void (*callback2)(void*, sqlite3_int64, int) = (seed % 3) ? dummy_memory_callback2 : NULL;
    sqlite3_int64 threshold = (sqlite3_int64)(seed * 1000) - 500;
    void *arg = (void *)(size_t)(seed % 100);

    sqlite3_memory_alarm(callback1, arg, threshold);
    sqlite3_memory_alarm(callback2, NULL, 0);
    sqlite3_memory_alarm(NULL, arg, threshold);
    sqlite3_memory_alarm(callback1, NULL, -1);

    // 2. Test sqlite3_status64
    // Use different operations and reset flags
    // Only test with valid pointers to avoid crashes on NULL writes
    op = (seed % 10) + 1; // Try various status codes
    resetFlag = (seed % 2);
    
    int ret64 = sqlite3_status64(op, &pCurrent64, &pHighwater64, resetFlag);
    (void)ret64;

    // 3. Test sqlite3_status
    int ret32 = sqlite3_status(op, &pCurrent32, &pHighwater32, resetFlag);
    (void)ret32;

    // 4. Test sqlite3_memory_used
    sqlite3_int64 used = sqlite3_memory_used();
    (void)used;

    // 5. Test sqlite3_soft_heap_limit64
    // Query current limit (negative value)
    limitVal = sqlite3_soft_heap_limit64(-1);
    
    // Set various limits
    limitVal = sqlite3_soft_heap_limit64(0); // Disable
    limitVal = sqlite3_soft_heap_limit64(1024); // Small limit
    limitVal = sqlite3_soft_heap_limit64((sqlite3_int64)seed * 1000000); // Large limit
    limitVal = sqlite3_soft_heap_limit64(-1); // Query again

    // 6. Test sqlite3_memory_highwater
    sqlite3_int64 hw = sqlite3_memory_highwater(0); // Query
    sqlite3_int64 hw_reset = sqlite3_memory_highwater(1); // Reset
    (void)hw;
    (void)hw_reset;

    return 0;
}