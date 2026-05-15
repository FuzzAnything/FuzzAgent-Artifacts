// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_complete at sqlite3.c:170606:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Ensure sqlite3 is initialized
    sqlite3_initialize();
    
    // Prepare null-terminated string for sqlite3_complete
    char *sql_str = NULL;
    if (Size > 0) {
        sql_str = (char *)malloc(Size + 1);
        if (sql_str) {
            memcpy(sql_str, Data, Size);
            sql_str[Size] = '\0';
        }
    } else {
        // Use empty string if no data
        sql_str = (char *)malloc(1);
        if (sql_str) {
            sql_str[0] = '\0';
        }
    }
    
    // 1. sqlite3_complete
    if (sql_str) {
        sqlite3_complete(sql_str);
    }
    
    // 2. sqlite3_free
    // Allocate some memory using sqlite3_malloc to free
    void *mem_to_free = sqlite3_malloc(100);
    if (mem_to_free) {
        // Write some data to the allocated memory
        if (Size > 0 && mem_to_free) {
            size_t copy_size = Size > 100 ? 100 : Size;
            memcpy(mem_to_free, Data, copy_size);
        }
        sqlite3_free(mem_to_free);
    }
    // Also test with NULL
    sqlite3_free(NULL);
    
    // 3. sqlite3_close
    // Create a database connection to close
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        sqlite3_close(db);
    }
    // Test with NULL
    sqlite3_close(NULL);
    
    // 4. sqlite3_memory_highwater
    // Test with resetFlag = 0 (don't reset)
    sqlite3_int64 highwater0 = sqlite3_memory_highwater(0);
    (void)highwater0; // Use variable to avoid unused warning
    
    // Test with resetFlag = 1 (reset)
    sqlite3_int64 highwater1 = sqlite3_memory_highwater(1);
    (void)highwater1;
    
    // 5. sqlite3_memory_used
    sqlite3_int64 mem_used1 = sqlite3_memory_used();
    (void)mem_used1;
    
    // 6. sqlite3_status
    int current = 0;
    int highwater = 0;
    // Test with different op codes derived from input
    int op_codes[] = {
        SQLITE_STATUS_MEMORY_USED,
        SQLITE_STATUS_PAGECACHE_USED,
        SQLITE_STATUS_PAGECACHE_OVERFLOW,
        SQLITE_STATUS_SCRATCH_USED,
        SQLITE_STATUS_SCRATCH_OVERFLOW,
        SQLITE_STATUS_MALLOC_SIZE,
        SQLITE_STATUS_PARSER_STACK,
        SQLITE_STATUS_PAGECACHE_SIZE,
        SQLITE_STATUS_SCRATCH_SIZE,
        SQLITE_STATUS_MALLOC_COUNT
    };
    
    size_t num_ops = sizeof(op_codes) / sizeof(op_codes[0]);
    if (num_ops > 0) {
        // Use input data to select op code and reset flag
        int op_index = 0;
        int reset_flag = 0;
        
        if (Size > 0) {
            op_index = Data[0] % num_ops;
            if (Size > 1) {
                reset_flag = Data[1] & 1;
            }
        }
        
        sqlite3_status(op_codes[op_index], &current, &highwater, reset_flag);
        
        // Also test with reset_flag = 1
        if (Size > 2) {
            reset_flag = Data[2] & 1;
            sqlite3_status(op_codes[op_index], &current, &highwater, reset_flag);
        }
    }
    
    // 7. sqlite3_memory_used (again)
    sqlite3_int64 mem_used2 = sqlite3_memory_used();
    (void)mem_used2;
    
    // 8. sqlite3_hard_heap_limit64
    sqlite3_int64 hard_limit = 0;
    if (Size >= 8) {
        // Use first 8 bytes as limit value
        memcpy(&hard_limit, Data, sizeof(hard_limit));
    } else if (Size > 0) {
        // Use available bytes
        hard_limit = 0;
        for (size_t i = 0; i < Size && i < sizeof(hard_limit); i++) {
            ((uint8_t *)&hard_limit)[i] = Data[i];
        }
    }
    // Also test with negative value (should leave limit unchanged)
    sqlite3_int64 prev_hard_limit = sqlite3_hard_heap_limit64(hard_limit);
    (void)prev_hard_limit;
    
    // Test with -1 to query current limit
    sqlite3_int64 current_hard_limit = sqlite3_hard_heap_limit64(-1);
    (void)current_hard_limit;
    
    // 9. sqlite3_soft_heap_limit64
    sqlite3_int64 soft_limit = 0;
    if (Size >= 8) {
        // Use bytes 8-15 as limit value if available, otherwise use first 8 bytes
        size_t offset = Size >= 16 ? 8 : 0;
        memcpy(&soft_limit, Data + offset, sizeof(soft_limit));
    } else if (Size > 0) {
        // Use available bytes
        soft_limit = 0;
        for (size_t i = 0; i < Size && i < sizeof(soft_limit); i++) {
            ((uint8_t *)&soft_limit)[i] = Data[i];
        }
    }
    sqlite3_int64 prev_soft_limit = sqlite3_soft_heap_limit64(soft_limit);
    (void)prev_soft_limit;
    
    // Test with -1 to query current limit
    sqlite3_int64 current_soft_limit = sqlite3_soft_heap_limit64(-1);
    (void)current_soft_limit;
    
    // Cleanup
    if (sql_str) {
        free(sql_str);
    }
    
    // Shutdown sqlite3
    sqlite3_shutdown();
    
    return 0;
}