// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:31649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_status64 at sqlite3.c:24748:16 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:31160:26 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static void fuzz_memory_allocation(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;
    
    int size_to_alloc = (Data[0] << 8) | Data[1];
    if (size_to_alloc <= 0) {
        size_to_alloc = 1;
    }
    if (size_to_alloc > 1024 * 1024) {
        size_to_alloc = 1024 * 1024;
    }

    // Test malloc
    void *ptr = sqlite3_malloc(size_to_alloc);
    if (ptr != NULL) {
        memset(ptr, 0xAB, size_to_alloc);
        sqlite3_free(ptr);
    }
    
    // Test realloc with NULL (acts as malloc)
    ptr = sqlite3_realloc(NULL, size_to_alloc);
    if (ptr != NULL) {
        memset(ptr, 0xCD, size_to_alloc);
        sqlite3_free(ptr);
    }
    
    // Test realloc with existing pointer
    // Ensure we have enough data before accessing Data[2]
    if (Size >= 3) {
        ptr = sqlite3_malloc(size_to_alloc);
        if (ptr != NULL) {
            int new_size = (Data[2] % 2) ? (size_to_alloc * 2) : (size_to_alloc / 2);
            if (new_size < 0) new_size = 0;
            if (new_size > 1024 * 1024) new_size = 1024 * 1024;
            
            void *new_ptr = sqlite3_realloc(ptr, new_size);
            // Important: After realloc, the original 'ptr' is invalid.
            // We must only use 'new_ptr'. If new_ptr is NULL, the original
            // allocation 'ptr' remains valid and must be freed.
            if (new_ptr != NULL) {
                if (new_size > 0) {
                    memset(new_ptr, 0xEF, new_size);
                }
                sqlite3_free(new_ptr);
            } else {
                // If realloc fails and new_size > 0, the original ptr is still valid.
                // If new_size <= 0, sqlite3_realloc acts like free, so ptr is already freed.
                // To be safe and avoid double-free if new_size <= 0 logic varies in edge cases,
                // we check if new_size > 0 before freeing the original.
                if (new_size > 0) {
                    sqlite3_free(ptr);
                }
            }
        }
    }
}

static void fuzz_status_and_limits(const uint8_t *Data, size_t Size) {
    sqlite3_int64 current_val = 0;
    sqlite3_int64 highwater_val = 0;
    int reset_flag = 0;
    sqlite3_int64 limit_val = 0;
    
    if (Size >= 1) {
        reset_flag = (Data[0] % 2);
    }
    
    // Test sqlite3_status64 with various op codes
    for (int i = 0; i < 16; i++) {
        int op = (Size > (size_t)(i + 1)) ? (Data[i + 1] % 20) : (i % 20);
        
        int ret = sqlite3_status64(op, &current_val, &highwater_val, reset_flag);
        (void)ret;
    }
    
    // Test heap limits
    if (Size >= 8) {
        limit_val = ((sqlite3_int64)Data[4] << 24) | ((sqlite3_int64)Data[5] << 16) | 
                    ((sqlite3_int64)Data[6] << 8) | Data[7];
        if (limit_val < 0) limit_val = -1; // Query mode
        if (limit_val > 1024 * 1024 * 1024) limit_val = 1024 * 1024 * 1024;
        
        sqlite3_soft_heap_limit64(limit_val);
        sqlite3_hard_heap_limit64(limit_val);
        
        // Test with negative values (query)
        sqlite3_soft_heap_limit64(-1);
        sqlite3_hard_heap_limit64(-1);
        
        // Test with zero (disable)
        sqlite3_soft_heap_limit64(0);
        sqlite3_hard_heap_limit64(0);
    }
    
    // Test memory tracking functions
    (void)sqlite3_memory_used();
    (void)sqlite3_memory_highwater(0);
    (void)sqlite3_memory_highwater(1);
}

int LLVMFuzzerTestOneInput_138(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_memory_allocation(Data, Size);
    fuzz_status_and_limits(Data, Size);
    
    return 0;
}