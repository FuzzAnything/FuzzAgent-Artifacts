// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Static callback function for sqlite3_memory_alarm.
 * This function is required to match the signature expected by the API,
 * even though the current implementation of sqlite3_memory_alarm ignores it.
 */
static void dummy_memory_alarm_callback(void *pArg, sqlite3_int64 nBytes, int isHard) {
    /* No-op callback */
    (void)pArg;
    (void)nBytes;
    (void)isHard;
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    /*
     * Step 1: Prepare environment and extract data.
     * We use the input data to drive various parameters for the API calls.
     * Since these APIs are global state queries or setters, we don't need
     * a database connection or file for this specific set of functions.
     */
    if (Size < 4) {
        /* Minimum size required to extract a few integers */
        return 0;
    }

    /* Extract parameters from fuzz input */
    uint32_t param1 = 0;
    uint32_t param2 = 0;
    uint32_t param3 = 0;
    uint32_t param4 = 0;

    if (Size >= 4) param1 = *(uint32_t *)(Data);
    if (Size >= 8) param2 = *(uint32_t *)(Data + 4);
    if (Size >= 12) param3 = *(uint32_t *)(Data + 8);
    if (Size >= 16) param4 = *(uint32_t *)(Data + 12);

    /* Convert to sqlite3_int64 for heap limit functions */
    sqlite3_int64 limit_val = (sqlite3_int64)param1;
    /* Introduce negative values for query modes */
    if (param1 & 0x80000000) {
        limit_val = -limit_val;
    }

    /*
     * Step 2 & 3: Invoke target functions diversely.
     */

    /* 1. sqlite3_memory_alarm */
    /* The API ignores arguments in the stub implementation, but we call it anyway. */
    sqlite3_memory_alarm(dummy_memory_alarm_callback, (void *)(uintptr_t)param2, limit_val);

    /* 2. sqlite3_soft_heap_limit64 */
    /* Test setting and querying (negative value) */
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(limit_val);
    (void)prev_soft_limit; /* Suppress unused variable warning */

    /* Query current limit */
    sqlite3_int64 current_soft_limit = sqlite3_soft_heap_limit64(-1);
    (void)current_soft_limit;

    /* Test with zero to disable */
    sqlite3_soft_heap_limit64(0);

    /* 3. sqlite3_status64 */
    sqlite3_int64 pCurrent64 = 0;
    sqlite3_int64 pHighwater64 = 0;
    int resetFlag64 = (param3 & 1) ? 1 : 0;
    
    /* Test with SQLITE_STATUS_MEMORY_USED (0) */
    int rc_status64 = sqlite3_status64(0, &pCurrent64, &pHighwater64, resetFlag64);
    (void)rc_status64;

    /* Test with SQLITE_STATUS_PAGECACHE_USED (2) if available, or just vary op code */
    int op_code = param3 % 10; /* Vary operation code to explore different metrics */
    rc_status64 = sqlite3_status64(op_code, &pCurrent64, &pHighwater64, resetFlag64);
    (void)rc_status64;

    /* 4. sqlite3_status */
    int pCurrent32 = 0;
    int pHighwater32 = 0;
    int resetFlag32 = (param4 & 1) ? 1 : 0;
    
    /* Test with SQLITE_STATUS_MEMORY_USED (0) */
    int rc_status = sqlite3_status(0, &pCurrent32, &pHighwater32, resetFlag32);
    (void)rc_status;

    /* Vary operation code */
    int op_code32 = param4 % 10;
    rc_status = sqlite3_status(op_code32, &pCurrent32, &pHighwater32, resetFlag32);
    (void)rc_status;

    /* 5. sqlite3_memory_highwater */
    /* Query without reset */
    sqlite3_int64 highwater_no_reset = sqlite3_memory_highwater(0);
    (void)highwater_no_reset;

    /* Query with reset */
    sqlite3_int64 highwater_reset = sqlite3_memory_highwater(1);
    (void)highwater_reset;

    /* 6. sqlite3_memory_used */
    sqlite3_int64 current_used = sqlite3_memory_used();
    (void)current_used;

    /*
     * Step 4: Wrap logic is complete.
     * Step 2: Cleanup is not strictly required for these global state functions
     * as they do not allocate resources that need explicit freeing in this context.
     * However, we ensure we return 0 to indicate successful processing of the input.
     */

    return 0;
}