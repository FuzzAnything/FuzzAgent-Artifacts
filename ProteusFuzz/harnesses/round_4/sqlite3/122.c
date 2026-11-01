// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void dummy_memory_alarm_callback(void *pArg, sqlite3_int64 nBytes, int nAlloc) {
    (void)pArg;
    (void)nBytes;
    (void)nAlloc;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract values from input data to drive fuzzing
    int op_code = 0;
    int reset_flag = 0;
    sqlite3_int64 limit_val = 0;
    
    if (Size >= 1) op_code = Data[0];
    if (Size >= 2) reset_flag = Data[1] % 2;
    if (Size >= 10) {
        // Construct a 64-bit integer from up to 8 bytes
        uint64_t raw = 0;
        size_t len = (Size < 8) ? Size : 8;
        memcpy(&raw, Data + 2, len);
        limit_val = (sqlite3_int64)raw;
    }

    // Prepare pointers for status functions
    sqlite3_int64 pCurrent64 = 0;
    sqlite3_int64 pHighwater64 = 0;
    int pCurrent32 = 0;
    int pHighwater32 = 0;

    // 1. Fuzz sqlite3_status64
    // Valid op codes are required to avoid internal crashes when accessing arrays.
    // We use known valid op codes: SQLITE_STATUS_MEMORY_USED (0), SQLITE_STATUS_PAGECACHE_USED (2), etc.
    // We also test invalid op codes but ensure pointers are valid to avoid NULL deref in internal logic.
    
    // Test with valid op codes
    sqlite3_status64(SQLITE_STATUS_MEMORY_USED, &pCurrent64, &pHighwater64, reset_flag);
    sqlite3_status64(SQLITE_STATUS_PAGECACHE_USED, &pCurrent64, &pHighwater64, reset_flag);
    sqlite3_status64(SQLITE_STATUS_PAGECACHE_OVERFLOW, &pCurrent64, &pHighwater64, reset_flag);
    sqlite3_status64(SQLITE_STATUS_MALLOC_SIZE, &pCurrent64, &pHighwater64, reset_flag);
    sqlite3_status64(SQLITE_STATUS_PARSER_STACK, &pCurrent64, &pHighwater64, reset_flag);
    
    // Test with an invalid op code (should return SQLITE_MISUSE) but with valid pointers
    // to prevent the internal crash seen in the ASAN log.
    if (op_code >= 0 && op_code < 100) {
        // Only test specific known invalid ranges if we want, but generally passing 
        // valid pointers is safer for fuzzing internal logic without triggering 
        // immediate NULL derefs in the library implementation.
        sqlite3_status64(op_code, &pCurrent64, &pHighwater64, reset_flag);
    }

    // 2. Fuzz sqlite3_memory_alarm
    // Pass the dummy callback and fuzzed arguments
    sqlite3_memory_alarm(dummy_memory_alarm_callback, (void*)(size_t)limit_val, limit_val);
    // Test with NULL callback (allowed by API)
    sqlite3_memory_alarm(NULL, (void*)(size_t)limit_val, limit_val);

    // 3. Fuzz sqlite3_memory_used
    // This function takes no arguments, just call it.
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;

    // 4. Fuzz sqlite3_memory_highwater
    sqlite3_int64 hw1 = sqlite3_memory_highwater(reset_flag);
    // Call again with inverted flag
    sqlite3_int64 hw2 = sqlite3_memory_highwater(!reset_flag);
    (void)hw1;
    (void)hw2;

    // 5. Fuzz sqlite3_soft_heap_limit64
    // Pass the fuzzed limit value
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(limit_val);
    (void)prev_limit;
    
    // Try setting to 0 (disables limiter) and -1 (query)
    sqlite3_soft_heap_limit64(0);
    sqlite3_soft_heap_limit64(-1);

    // 6. Fuzz sqlite3_status
    // Similar to status64 but 32-bit. Ensure valid pointers.
    sqlite3_status(SQLITE_STATUS_MEMORY_USED, &pCurrent32, &pHighwater32, reset_flag);
    sqlite3_status(SQLITE_STATUS_PAGECACHE_USED, &pCurrent32, &pHighwater32, reset_flag);
    
    // Test invalid op code with valid pointers
    if (op_code >= 0 && op_code < 100) {
        sqlite3_status(op_code, &pCurrent32, &pHighwater32, reset_flag);
    }

    return 0;
}