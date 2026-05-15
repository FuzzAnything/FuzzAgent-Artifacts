// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from the fuzzer input for sqlite3_complete
    char *sql_str = NULL;
    if (Size > 0) {
        sql_str = (char *)malloc(Size + 1);
        if (sql_str) {
            memcpy(sql_str, Data, Size);
            sql_str[Size] = '\0';
        }
    } else {
        // If no data, use a minimal string
        sql_str = (char *)malloc(1);
        if (sql_str) {
            sql_str[0] = '\0';
        }
    }

    // 1. sqlite3_complete
    if (sql_str) {
        int complete_result = sqlite3_complete(sql_str);
        (void)complete_result; // Use result to avoid unused variable warning
    }

    // 2. sqlite3_free - free the string we allocated
    sqlite3_free(sql_str);

    // 3. sqlite3_close - open a database connection first, then close it
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK) {
        sqlite3_close(db);
    } else {
        // If open failed, still call close with NULL (harmless no-op)
        sqlite3_close(NULL);
    }

    // 4. sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(0);
    (void)highwater;

    // 5. sqlite3_memory_used
    sqlite3_int64 memory_used = sqlite3_memory_used();
    (void)memory_used;

    // 6. sqlite3_status - test with different opcodes
    int current = 0;
    int highwater_status = 0;
    
    // Try multiple status parameters
    int status_opcodes[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_SCRATCH_USED,
        SQLITE_STATUS_SCRATCH_OVERFLOW,
        SQLITE_STATUS_MALLOC_SIZE,
        SQLITE_STATUS_PARSER_STACK,
        SQLITE_STATUS_MALLOC_COUNT
    };
    
    for (size_t i = 0; i < sizeof(status_opcodes)/sizeof(status_opcodes[0]); i++) {
        int reset_flag = (Size > 0 && (Data[0] & 1)) ? 1 : 0;
        sqlite3_status(status_opcodes[i], &current, &highwater_status, reset_flag);
    }

    // 7. sqlite3_memory_used (again)
    memory_used = sqlite3_memory_used();
    (void)memory_used;

    // 8. sqlite3_hard_heap_limit64
    sqlite3_int64 hard_limit = -1; // Query current limit
    if (Size > 0) {
        // Use first byte to set a limit (0-255 bytes)
        hard_limit = Data[0];
        sqlite3_int64 prev_hard = sqlite3_hard_heap_limit64(hard_limit);
        (void)prev_hard;
        
        // Also try with larger values based on input
        if (Size > 1) {
            sqlite3_int64 limit2 = (sqlite3_int64)Data[1] * 1024;
            sqlite3_hard_heap_limit64(limit2);
        }
    }
    // Query again
    sqlite3_hard_heap_limit64(-1);

    // 9. sqlite3_soft_heap_limit64
    sqlite3_int64 soft_limit = -1; // Query current limit
    if (Size > 0) {
        // Use second byte (if available) for soft limit
        soft_limit = (Size > 1) ? (sqlite3_int64)Data[1] * 1024 : 1024;
        sqlite3_int64 prev_soft = sqlite3_soft_heap_limit64(soft_limit);
        (void)prev_soft;
        
        // Try different values
        if (Size > 2) {
            sqlite3_soft_heap_limit64((sqlite3_int64)Data[2] * 4096);
        }
    }
    // Query again
    sqlite3_soft_heap_limit64(-1);

    return 0;
}