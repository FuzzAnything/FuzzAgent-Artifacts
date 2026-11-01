// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_complete at sqlite3.c:185350:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_status at sqlite3.c:24773:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. sqlite3_complete
    // Ensure the input is treated as a valid C string for sqlite3_complete
    // We create a buffer with a guaranteed null terminator.
    char *sql_buf = (char *)malloc(Size + 1);
    if (!sql_buf) return 0;
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    int complete_result = sqlite3_complete(sql_buf);
    (void)complete_result;

    // 2. sqlite3_free
    // Allocate a dummy pointer using sqlite3_malloc to test sqlite3_free
    void *dummy_ptr = sqlite3_malloc(Size);
    if (dummy_ptr) {
        sqlite3_free(dummy_ptr);
    }
    // Free the sql_buf allocated above using standard free to avoid mismatch
    // since sql_buf was allocated with malloc, not sqlite3_malloc.
    free(sql_buf);

    // 3. sqlite3_close
    // Passing NULL is a valid no-op according to the docs.
    sqlite3 *db = NULL;
    int close_result = sqlite3_close(db);
    (void)close_result;

    // 4. sqlite3_memory_highwater
    // Reset the highwater mark (pass 1)
    sqlite3_int64 hw_reset = sqlite3_memory_highwater(1);
    (void)hw_reset;

    // 5. sqlite3_memory_used
    sqlite3_int64 mem_used_1 = sqlite3_memory_used();
    (void)mem_used_1;

    // 6. sqlite3_status
    int current_val = 0;
    int highwater_val = 0;
    int status_result = sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current_val, &highwater_val, 0);
    (void)status_result;

    // 7. sqlite3_memory_used (again)
    sqlite3_int64 mem_used_2 = sqlite3_memory_used();
    (void)mem_used_2;

    // 8. sqlite3_hard_heap_limit64
    // Set a limit based on input data to explore different states
    sqlite3_int64 limit_val = 0;
    if (Size >= 8) {
        // Read 8 bytes from input, handling potential alignment issues safely
        uint8_t *p = (uint8_t *)Data;
        limit_val = (sqlite3_int64)p[0] | ((sqlite3_int64)p[1] << 8) | 
                    ((sqlite3_int64)p[2] << 16) | ((sqlite3_int64)p[3] << 24) |
                    ((sqlite3_int64)p[4] << 32) | ((sqlite3_int64)p[5] << 40) |
                    ((sqlite3_int64)p[6] << 48) | ((sqlite3_int64)p[7] << 56);
    } else {
        limit_val = 1024 * 1024; // Default 1MB
    }
    
    // If the value is negative, it acts as a query. If positive, it sets the limit.
    sqlite3_int64 old_hard_limit = sqlite3_hard_heap_limit64(limit_val);
    (void)old_hard_limit;

    // 9. sqlite3_soft_heap_limit64
    // Set a soft limit
    sqlite3_int64 soft_limit_val = 0;
    if (Size >= 16) {
        uint8_t *p = (uint8_t *)(Data + 8);
        soft_limit_val = (sqlite3_int64)p[0] | ((sqlite3_int64)p[1] << 8) | 
                         ((sqlite3_int64)p[2] << 16) | ((sqlite3_int64)p[3] << 24) |
                         ((sqlite3_int64)p[4] << 32) | ((sqlite3_int64)p[5] << 40) |
                         ((sqlite3_int64)p[6] << 48) | ((sqlite3_int64)p[7] << 56);
    } else {
        soft_limit_val = 512 * 1024; // Default 512KB
    }
    
    sqlite3_int64 old_soft_limit = sqlite3_soft_heap_limit64(soft_limit_val);
    (void)old_soft_limit;

    return 0;
}