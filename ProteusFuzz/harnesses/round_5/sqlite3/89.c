// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_memory_alarm at sqlite3.c:31137:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_callback(void *pArg, sqlite3_int64 nCur, int nHigh) {
    (void)pArg;
    (void)nCur;
    (void)nHigh;
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Extract values from input data to drive fuzzing logic
    sqlite3_int64 heap_limit = 0;
    int reset_flag = 0;
    int op = 0;
    void *callback_arg = NULL;

    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&heap_limit, Data, sizeof(sqlite3_int64));
    }
    if (Size >= sizeof(sqlite3_int64) + sizeof(int)) {
        memcpy(&reset_flag, Data + sizeof(sqlite3_int64), sizeof(int));
    }
    if (Size >= sizeof(sqlite3_int64) + sizeof(int) + sizeof(int)) {
        memcpy(&op, Data + sizeof(sqlite3_int64) + sizeof(int), sizeof(int));
    }
    if (Size >= sizeof(sqlite3_int64) + sizeof(int) + sizeof(int) + sizeof(void*)) {
        memcpy(&callback_arg, Data + sizeof(sqlite3_int64) + sizeof(int) + sizeof(int), sizeof(void*));
    }

    // 1. sqlite3_memory_used
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;

    // 2. sqlite3_status
    int current_32 = 0;
    int highwater_32 = 0;
    int ret_status = sqlite3_status(op, &current_32, &highwater_32, reset_flag);
    (void)ret_status;

    // 3. sqlite3_soft_heap_limit64
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(heap_limit);
    (void)prev_limit;

    // 4. sqlite3_memory_alarm
    int ret_alarm = sqlite3_memory_alarm(dummy_callback, callback_arg, heap_limit);
    (void)ret_alarm;

    // 5. sqlite3_status64
    sqlite3_int64 current_64 = 0;
    sqlite3_int64 highwater_64 = 0;
    int ret_status64 = sqlite3_status64(op, &current_64, &highwater_64, reset_flag);
    (void)ret_status64;

    // 6. sqlite3_memory_highwater
    sqlite3_int64 mem_highwater = sqlite3_memory_highwater(reset_flag);
    (void)mem_highwater;

    // Additional exploration: call functions with diverse states
    // Reset highwater marks
    sqlite3_memory_highwater(1);
    sqlite3_status64(SQLITE_STATUS_MEMORY_USED, &current_64, &highwater_64, 1);
    
    // Set extreme heap limits
    sqlite3_soft_heap_limit64(-1); // Query current
    sqlite3_soft_heap_limit64(0);  // Disable limit
    sqlite3_soft_heap_limit64(1024); // Small limit
    sqlite3_soft_heap_limit64(-1); // Restore query

    // Call status with different operations if possible (hardcoded known values)
    int known_ops[] = {SQLITE_STATUS_MEMORY_USED, SQLITE_STATUS_PAGECACHE_USED, SQLITE_STATUS_PAGECACHE_OVERFLOW, SQLITE_STATUS_MALLOC_SIZE};
    for (int i = 0; i < 4; i++) {
        sqlite3_status(known_ops[i], &current_32, &highwater_32, 0);
        sqlite3_status64(known_ops[i], &current_64, &highwater_64, 0);
    }

    return 0;
}