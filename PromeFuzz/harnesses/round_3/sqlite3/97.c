// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static sqlite3_uint64 read_uint64(const uint8_t *data, size_t size, size_t offset) {
    sqlite3_uint64 value = 0;
    if (offset < size) {
        size_t bytes_to_copy = size - offset;
        if (bytes_to_copy > sizeof(value)) bytes_to_copy = sizeof(value);
        memcpy(&value, data + offset, bytes_to_copy);
    }
    return value;
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    // Limit allocation sizes to avoid excessive memory usage
    const sqlite3_uint64 MAX_ALLOC_SIZE = 1024 * 1024; // 1MB
    const sqlite3_int64 MAX_HEAP_LIMIT = 10 * 1024 * 1024; // 10MB
    
    // Read parameters from fuzzer input
    sqlite3_uint64 alloc_size1 = read_uint64(Data, Size, 0) % MAX_ALLOC_SIZE;
    sqlite3_uint64 alloc_size2 = read_uint64(Data, Size, 8) % MAX_ALLOC_SIZE;
    sqlite3_int64 heap_limit = (sqlite3_int64)(read_uint64(Data, Size, 16) % MAX_HEAP_LIMIT);
    int reset_flag = (Size >= 25) ? (Data[24] & 1) : 0;
    
    // Test sqlite3_memory_used and sqlite3_memory_highwater
    sqlite3_int64 current_memory = sqlite3_memory_used();
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    
    // Test sqlite3_soft_heap_limit64
    sqlite3_int64 original_limit = sqlite3_soft_heap_limit64(-1); // Query current limit
    sqlite3_soft_heap_limit64(heap_limit); // Set new limit
    sqlite3_int64 current_limit = sqlite3_soft_heap_limit64(-1); // Query again
    
    // Test sqlite3_malloc64 and sqlite3_msize
    void *ptr1 = sqlite3_malloc64(alloc_size1);
    if (ptr1) {
        sqlite3_uint64 size1 = sqlite3_msize(ptr1);
        
        // Test sqlite3_realloc64
        void *ptr2 = sqlite3_realloc64(ptr1, alloc_size2);
        if (ptr2) {
            sqlite3_uint64 size2 = sqlite3_msize(ptr2);
            sqlite3_free(ptr2);
        }
        // When sqlite3_realloc64 returns NULL, the original pointer remains valid
        // and must be freed separately. Do not free ptr1 here as it was already
        // handled by sqlite3_realloc64 when it returned a new pointer.
    }
    
    // Test with NULL pointer for sqlite3_msize
    sqlite3_uint64 null_size = sqlite3_msize(NULL);
    
    // Test with zero allocation
    void *zero_ptr = sqlite3_malloc64(0);
    if (zero_ptr) {
        sqlite3_uint64 zero_size = sqlite3_msize(zero_ptr);
        sqlite3_free(zero_ptr);
    }
    
    // Test realloc with NULL pointer (should act like malloc)
    void *realloc_null = sqlite3_realloc64(NULL, alloc_size1 % 1024);
    if (realloc_null) {
        sqlite3_free(realloc_null);
    }
    
    // Restore original heap limit
    sqlite3_soft_heap_limit64(original_limit);
    
    // Call memory functions again to see if anything changed
    sqlite3_memory_used();
    sqlite3_memory_highwater(0);
    
    return 0;
}