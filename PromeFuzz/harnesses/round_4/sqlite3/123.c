// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <string.h>

static void test_memory_functions(const uint8_t *data, size_t size) {
    // Use first byte to determine reset flag (0 or 1)
    int reset_flag = (size > 0) ? (data[0] & 1) : 0;
    
    // Use next bytes to form allocation sizes
    sqlite3_uint64 alloc_size = 0;
    sqlite3_uint64 realloc_size = 0;
    
    if (size > 1) {
        // Use up to 8 bytes for alloc_size
        size_t bytes_to_copy = (size - 1) > 8 ? 8 : (size - 1);
        memcpy(&alloc_size, data + 1, bytes_to_copy);
    }
    
    if (size > 9) {
        // Use up to 8 bytes for realloc_size
        size_t bytes_to_copy = (size - 9) > 8 ? 8 : (size - 9);
        memcpy(&realloc_size, data + 9, bytes_to_copy);
    }
    
    // Cap sizes to reasonable values to avoid excessive memory usage
    if (alloc_size > 1024 * 1024) alloc_size = 1024 * 1024;
    if (realloc_size > 1024 * 1024) realloc_size = 1024 * 1024;
    
    // Test sqlite3_malloc64
    void *ptr = sqlite3_malloc64(alloc_size);
    
    // Test sqlite3_msize
    sqlite3_uint64 msize_result = sqlite3_msize(ptr);
    
    // Test sqlite3_realloc64
    void *realloc_ptr = sqlite3_realloc64(ptr, realloc_size);
    
    // Test sqlite3_memory_used
    sqlite3_int64 mem_used = sqlite3_memory_used();
    
    // Test sqlite3_memory_highwater
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    
    // Test sqlite3_soft_heap_limit64
    // Use alloc_size as the limit value (cast to sqlite3_int64)
    sqlite3_int64 old_limit = sqlite3_soft_heap_limit64((sqlite3_int64)alloc_size);
    
    // Query the current limit
    sqlite3_int64 current_limit = sqlite3_soft_heap_limit64(-1);
    
    // Clean up allocated memory
    if (realloc_ptr) {
        sqlite3_free(realloc_ptr);
    } else if (ptr) {
        sqlite3_free(ptr);
    }
    
    // Prevent unused variable warnings
    (void)msize_result;
    (void)mem_used;
    (void)highwater;
    (void)old_limit;
    (void)current_limit;
}

int LLVMFuzzerTestOneInput_123(const uint8_t *Data, size_t Size) {
    // Test with various input patterns
    if (Size == 0) {
        // Test with minimal input
        test_memory_functions(Data, Size);
        return 0;
    }
    
    // Test the main functionality
    test_memory_functions(Data, Size);
    
    // Additional test: sqlite3_msize with NULL pointer
    sqlite3_uint64 null_msize = sqlite3_msize(NULL);
    (void)null_msize;
    
    // Additional test: sqlite3_realloc64 with NULL pointer (should act like malloc)
    if (Size > 0) {
        sqlite3_uint64 malloc_size = Data[0];
        if (malloc_size > 1024 * 1024) malloc_size = 1024 * 1024;
        void *null_realloc_ptr = sqlite3_realloc64(NULL, malloc_size);
        if (null_realloc_ptr) {
            sqlite3_free(null_realloc_ptr);
        }
    }
    
    // Additional test: sqlite3_memory_highwater with both reset flags
    sqlite3_int64 highwater_no_reset = sqlite3_memory_highwater(0);
    sqlite3_int64 highwater_reset = sqlite3_memory_highwater(1);
    (void)highwater_no_reset;
    (void)highwater_reset;
    
    return 0;
}