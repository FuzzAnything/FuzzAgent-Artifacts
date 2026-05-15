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
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_memory_operations(const uint8_t *data, size_t size) {
    sqlite3_uint64 alloc_size = 0;
    sqlite3_int64 limit_val = 0;
    
    // Extract parameters from fuzzer input
    if (size >= sizeof(sqlite3_uint64)) {
        memcpy(&alloc_size, data, sizeof(sqlite3_uint64));
        data += sizeof(sqlite3_uint64);
        size -= sizeof(sqlite3_uint64);
    }
    
    if (size >= sizeof(sqlite3_int64)) {
        memcpy(&limit_val, data, sizeof(sqlite3_int64));
        data += sizeof(sqlite3_int64);
        size -= sizeof(sqlite3_int64);
    }
    
    int reset_flag = 0;
    if (size > 0) {
        reset_flag = data[0] & 1;
    }
    
    // Test sqlite3_soft_heap_limit64
    sqlite3_int64 prev_limit = sqlite3_soft_heap_limit64(limit_val);
    // Query current limit
    sqlite3_soft_heap_limit64(-1);
    // Disable limit
    sqlite3_soft_heap_limit64(0);
    // Restore previous limit
    sqlite3_soft_heap_limit64(prev_limit);
    
    // Test sqlite3_malloc64
    void *ptr = sqlite3_malloc64(alloc_size);
    if (ptr) {
        // Test sqlite3_msize on allocated memory
        sqlite3_uint64 msize = sqlite3_msize(ptr);
        
        // Test sqlite3_realloc64
        sqlite3_uint64 realloc_size = alloc_size / 2; // Try different size
        void *new_ptr = sqlite3_realloc64(ptr, realloc_size);
        if (new_ptr) {
            sqlite3_msize(new_ptr);
            ptr = new_ptr;
        }
        
        // Free allocated memory
        sqlite3_free(ptr);
    }
    
    // Test sqlite3_memory_highwater with different reset flags
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    // Also test without reset
    sqlite3_memory_highwater(0);
    
    // Test sqlite3_memory_used
    sqlite3_int64 used = sqlite3_memory_used();
    
    // Additional test: allocate and free multiple blocks
    for (int i = 0; i < 10 && size > 0; i++) {
        sqlite3_uint64 small_size = data[i % size] + 1;
        void *small_ptr = sqlite3_malloc64(small_size);
        if (small_ptr) {
            sqlite3_msize(small_ptr);
            sqlite3_free(small_ptr);
        }
    }
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    // Initialize SQLite memory subsystem if needed
    // The memory functions handle initialization automatically
    
    // Run memory operations with fuzzer data
    fuzz_memory_operations(Data, Size);
    
    return 0;
}