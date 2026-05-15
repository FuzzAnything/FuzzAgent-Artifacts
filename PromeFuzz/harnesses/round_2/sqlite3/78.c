// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:17649:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_msize at sqlite3.c:17461:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    // Use first byte to determine operation mode
    if (Size == 0) return 0;
    
    uint8_t mode = Data[0] % 6;
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    // Extract 64-bit values from fuzzer input
    sqlite3_int64 int64_param = 0;
    sqlite3_uint64 uint64_param = 0;
    
    if (remaining_size >= sizeof(sqlite3_int64)) {
        memcpy(&int64_param, remaining_data, sizeof(sqlite3_int64));
        remaining_data += sizeof(sqlite3_int64);
        remaining_size -= sizeof(sqlite3_int64);
    }
    
    if (remaining_size >= sizeof(sqlite3_uint64)) {
        memcpy(&uint64_param, remaining_data, sizeof(sqlite3_uint64));
    }
    
    switch (mode) {
        case 0: {
            // Test sqlite3_soft_heap_limit64
            sqlite3_int64 previous_limit = sqlite3_soft_heap_limit64(int64_param);
            // Query current limit
            sqlite3_soft_heap_limit64(-1);
            // Disable limit
            sqlite3_soft_heap_limit64(0);
            // Set to a small value
            sqlite3_soft_heap_limit64(1024);
            // Query again
            sqlite3_soft_heap_limit64(-1);
            // Restore previous limit
            sqlite3_soft_heap_limit64(previous_limit);
            break;
        }
        
        case 1: {
            // Test sqlite3_malloc64
            void *ptr = sqlite3_malloc64(uint64_param);
            if (ptr) {
                // Test sqlite3_msize on allocated memory
                sqlite3_uint64 allocated_size = sqlite3_msize(ptr);
                // Free the memory
                sqlite3_free(ptr);
            }
            break;
        }
        
        case 2: {
            // Test sqlite3_memory_highwater
            int reset_flag = (int64_param & 1) ? 1 : 0;
            sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
            // Also test with opposite reset flag
            sqlite3_memory_highwater(!reset_flag);
            break;
        }
        
        case 3: {
            // Test sqlite3_memory_used
            sqlite3_int64 current_usage = sqlite3_memory_used();
            // Allocate some memory and check again
            void *temp_ptr = sqlite3_malloc64(100);
            sqlite3_int64 new_usage = sqlite3_memory_used();
            if (temp_ptr) {
                sqlite3_free(temp_ptr);
            }
            break;
        }
        
        case 4: {
            // Test sqlite3_msize with various inputs
            void *ptr = sqlite3_malloc64(uint64_param % 1024); // Limit size
            if (ptr) {
                sqlite3_uint64 size = sqlite3_msize(ptr);
                // Test with NULL pointer
                sqlite3_uint64 null_size = sqlite3_msize(NULL);
                sqlite3_free(ptr);
            }
            // Test with explicitly NULL
            sqlite3_msize(NULL);
            break;
        }
        
        case 5: {
            // Test sqlite3_realloc64
            void *original_ptr = sqlite3_malloc64(uint64_param % 512);
            if (original_ptr) {
                // Reallocate to different size
                sqlite3_uint64 new_size = (uint64_param >> 8) % 1024;
                void *new_ptr = sqlite3_realloc64(original_ptr, new_size);
                if (new_ptr) {
                    // Test msize on reallocated memory
                    sqlite3_uint64 realloc_size = sqlite3_msize(new_ptr);
                    sqlite3_free(new_ptr);
                } else {
                    // If realloc failed, original pointer is still valid
                    sqlite3_free(original_ptr);
                }
            }
            // Test realloc with NULL pointer (should act like malloc)
            void *ptr_from_null = sqlite3_realloc64(NULL, uint64_param % 256);
            if (ptr_from_null) {
                sqlite3_free(ptr_from_null);
            }
            break;
        }
    }
    
    // Additional combination tests
    // Test memory allocation and checking highwater mark
    void *test_ptr = sqlite3_malloc64(Size % 2048);
    if (test_ptr) {
        sqlite3_memory_used();
        sqlite3_memory_highwater(0);
        sqlite3_free(test_ptr);
    }
    
    // Test setting heap limit and then allocating
    sqlite3_int64 old_limit = sqlite3_soft_heap_limit64(4096);
    void *limited_ptr = sqlite3_malloc64(1024);
    if (limited_ptr) {
        sqlite3_msize(limited_ptr);
        sqlite3_free(limited_ptr);
    }
    sqlite3_soft_heap_limit64(old_limit);
    
    return 0;
}