// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    // 1. sqlite3_complete - use input as SQL string
    char *sql_str = NULL;
    if (Size > 0) {
        // Use sqlite3_malloc instead of malloc for memory that will be freed with sqlite3_free
        sql_str = (char *)sqlite3_malloc(Size + 1);
        if (sql_str) {
            // Ensure we don't read beyond Data bounds
            size_t copy_size = Size;
            if (copy_size > 0) {
                memcpy(sql_str, Data, copy_size);
                sql_str[copy_size] = '\0';
                int complete_result = sqlite3_complete(sql_str);
                (void)complete_result; // Use result to avoid unused warning
            }
        }
    }
    
    // 2. sqlite3_free - free memory allocated by SQLite
    if (sql_str) {
        sqlite3_free(sql_str);
    } else {
        // If no sql_str was allocated, free some other memory
        void *mem = sqlite3_malloc(10);
        if (mem) {
            sqlite3_free(mem);
        }
    }
    
    // 3. sqlite3_close - close a database connection
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_close(db);
    }
    
    // 4. sqlite3_memory_highwater - get peak memory usage
    int reset_flag = (Size > 0) ? (Data[0] & 1) : 0;
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    (void)highwater;
    
    // 5. sqlite3_memory_used - get current memory usage
    sqlite3_int64 used = sqlite3_memory_used();
    (void)used;
    
    // 6. sqlite3_status - get runtime status
    int current = 0;
    int highwater_stat = 0;
    int reset_flag_stat = (Size > 1) ? (Data[1] & 1) : 0;
    
    // Try different status operations based on input
    int op = (Size > 2) ? (Data[2] % 10) : 0;
    sqlite3_status(op, &current, &highwater_stat, reset_flag_stat);
    
    // 7. sqlite3_memory_used - again
    used = sqlite3_memory_used();
    (void)used;
    
    // 8. sqlite3_hard_heap_limit64 - set hard heap limit
    sqlite3_int64 limit = 0;
    if (Size >= 8) {
        // Use first 8 bytes as limit value, but ensure we don't read beyond bounds
        memcpy(&limit, Data, sizeof(limit));
    } else if (Size > 0) {
        // Use first byte scaled up
        limit = (sqlite3_int64)Data[0] * 1024 * 1024;
    } else {
        limit = 1024 * 1024; // 1MB default
    }
    
    sqlite3_int64 prev_hard = sqlite3_hard_heap_limit64(limit);
    (void)prev_hard;
    
    // 9. sqlite3_soft_heap_limit64 - set soft heap limit
    sqlite3_int64 soft_limit = limit / 2; // Use half of hard limit
    if (soft_limit <= 0) {
        soft_limit = 1024 * 1024; // 1MB default
    }
    
    sqlite3_int64 prev_soft = sqlite3_soft_heap_limit64(soft_limit);
    (void)prev_soft;
    
    return 0;
}