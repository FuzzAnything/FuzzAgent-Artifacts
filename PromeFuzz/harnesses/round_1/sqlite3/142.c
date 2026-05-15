// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void* g_alloc_ptr = NULL;

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    // Free any previously allocated memory at start
    if (g_alloc_ptr != NULL) {
        sqlite3_free(g_alloc_ptr);
        g_alloc_ptr = NULL;
    }

    // Generate parameters from fuzzer input
    sqlite3_int64 param_int64 = 0;
    sqlite3_uint64 param_uint64 = 0;
    int reset_flag = 0;
    
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&param_int64, Data, sizeof(sqlite3_int64));
        memcpy(&param_uint64, Data, sizeof(sqlite3_uint64));
    } else if (Size > 0) {
        // Use available bytes for smaller values
        memcpy(&param_int64, Data, Size);
        memcpy(&param_uint64, Data, Size);
    }
    
    if (Size > 0) {
        reset_flag = Data[0] & 1;
    }

    // Test sqlite3_soft_heap_limit64 with various values
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(param_int64);
    
    // Test with special values
    sqlite3_soft_heap_limit64(-1);  // Query current limit
    sqlite3_soft_heap_limit64(0);   // Disable limit
    sqlite3_soft_heap_limit64(1024 * 1024);  // Set to 1MB
    
    // Restore original limit
    sqlite3_soft_heap_limit64(prev_limit);

    // Test sqlite3_malloc64 with various sizes
    size_t alloc_size = param_uint64 % (1024 * 1024);  // Limit to 1MB max
    void* ptr1 = sqlite3_malloc64(alloc_size);
    
    // Test sqlite3_msize on allocated pointer
    if (ptr1 != NULL) {
        sqlite3_uint64 msize1 = sqlite3_msize(ptr1);
        (void)msize1;  // Use result to avoid unused variable warning
        
        // Test sqlite3_realloc64
        size_t new_size = (param_uint64 / 2) % (1024 * 1024);
        void* ptr2 = sqlite3_realloc64(ptr1, new_size);
        
        if (ptr2 != NULL) {
            sqlite3_uint64 msize2 = sqlite3_msize(ptr2);
            (void)msize2;
            
            // Store pointer for potential use in next iteration
            g_alloc_ptr = ptr2;
        } else {
            // realloc failed, free original pointer
            sqlite3_free(ptr1);
        }
    }

    // Test sqlite3_memory_used
    sqlite3_int64 mem_used = sqlite3_memory_used();
    (void)mem_used;

    // Test sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    (void)highwater;
    
    // Also test without reset
    highwater = sqlite3_memory_highwater(0);
    (void)highwater;

    return 0;
}