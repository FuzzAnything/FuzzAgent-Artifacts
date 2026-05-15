// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    // Use the input data to generate parameters for the API functions
    // We'll interpret the data as a sequence of 64-bit values where possible
    
    // Extract parameters from fuzzer input
    sqlite3_int64 limit_param = 0;
    sqlite3_uint64 malloc_size = 0;
    sqlite3_uint64 realloc_size = 0;
    int reset_flag = 0;
    
    // Read parameters from input data if available
    if (Size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_param, Data, sizeof(sqlite3_int64));
        Data += sizeof(sqlite3_int64);
        Size -= sizeof(sqlite3_int64);
    }
    
    if (Size >= sizeof(sqlite3_uint64)) {
        memcpy(&malloc_size, Data, sizeof(sqlite3_uint64));
        Data += sizeof(sqlite3_uint64);
        Size -= sizeof(sqlite3_uint64);
    }
    
    if (Size >= sizeof(sqlite3_uint64)) {
        memcpy(&realloc_size, Data, sizeof(sqlite3_uint64));
        Data += sizeof(sqlite3_uint64);
        Size -= sizeof(sqlite3_uint64);
    }
    
    if (Size >= sizeof(int)) {
        memcpy(&reset_flag, Data, sizeof(int));
        // Ensure reset_flag is 0 or 1 as per API expectations
        reset_flag = reset_flag & 1;
    }
    
    // 1. Test sqlite3_soft_heap_limit64
    // Try different limit values including edge cases
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(limit_param);
    
    // Test querying current limit
    sqlite3_int64 current_limit = sqlite3_soft_heap_limit64(-1);
    
    // Test disabling limit
    sqlite3_soft_heap_limit64(0);
    
    // Restore previous limit
    if (prev_limit >= 0) {
        sqlite3_soft_heap_limit64(prev_limit);
    }
    
    // 2. Test sqlite3_malloc64 and sqlite3_msize
    // Limit allocation size to avoid excessive memory usage
    sqlite3_uint64 safe_malloc_size = malloc_size % (1024 * 1024); // Limit to 1MB
    
    void *allocated_ptr = sqlite3_malloc64(safe_malloc_size);
    if (allocated_ptr != NULL) {
        // Test sqlite3_msize on allocated memory
        sqlite3_uint64 allocated_size = sqlite3_msize(allocated_ptr);
        
        // 3. Test sqlite3_realloc64
        sqlite3_uint64 safe_realloc_size = realloc_size % (1024 * 1024); // Limit to 1MB
        void *reallocated_ptr = sqlite3_realloc64(allocated_ptr, safe_realloc_size);
        
        if (reallocated_ptr != NULL) {
            // Test sqlite3_msize on reallocated memory
            sqlite3_uint64 reallocated_size = sqlite3_msize(reallocated_ptr);
            sqlite3_free(reallocated_ptr);
        } else {
            // If realloc failed, original pointer is still valid
            sqlite3_free(allocated_ptr);
        }
    }
    
    // 4. Test sqlite3_memory_used
    sqlite3_int64 memory_used = sqlite3_memory_used();
    
    // 5. Test sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    
    // Test with opposite reset flag
    sqlite3_int64 highwater2 = sqlite3_memory_highwater(!reset_flag);
    
    // Additional test: allocate and free to exercise memory system
    void *temp_ptr = sqlite3_malloc64(128);
    if (temp_ptr != NULL) {
        sqlite3_free(temp_ptr);
    }
    
    // Test with different limit values
    sqlite3_soft_heap_limit64(1024 * 1024); // 1MB limit
    sqlite3_soft_heap_limit64(512 * 1024);  // 512KB limit
    sqlite3_soft_heap_limit64(-1);          // Query again
    
    return 0;
}