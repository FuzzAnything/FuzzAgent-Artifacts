// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_global_recover at sqlite3.c:174799:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:17081:16 in sqlite3.h
// sqlite3_realloc at sqlite3.c:17642:18 in sqlite3.h
// sqlite3_realloc at sqlite3.c:17642:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_133(const uint8_t *Data, size_t Size) {
    // Use the input data to generate parameters for the API calls
    // We'll use the first few bytes to determine how to call the functions
    
    if (Size < 1) return 0;
    
    // Extract some parameters from the fuzzer input
    uint8_t choice = Data[0];
    int int_param = 0;
    int reset_flag = 0;
    void *ptr = NULL;
    
    // Initialize some memory for sqlite3_realloc
    if (Size > 1) {
        int_param = (int)Data[1];
        // Ensure non-negative size for sqlite3_realloc
        if (int_param < 0) int_param = -int_param;
    }
    
    if (Size > 2) {
        reset_flag = Data[2] & 1; // 0 or 1
    }
    
    // Call sqlite3_release_memory with parameter from fuzzer input
    if (choice % 6 == 0) {
        sqlite3_release_memory(int_param);
    }
    
    // Call sqlite3_realloc with various parameters
    if (choice % 6 == 1) {
        // Try reallocating NULL pointer
        ptr = sqlite3_realloc(NULL, int_param);
        if (ptr) {
            // Try reallocating the allocated memory
            void *new_ptr = sqlite3_realloc(ptr, int_param * 2);
            if (new_ptr) {
                ptr = new_ptr;
            }
            sqlite3_free(ptr);
        }
    }
    
    // Call sqlite3_memory_highwater
    if (choice % 6 == 2) {
        sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
        (void)highwater; // Use result to avoid unused variable warning
    }
    
    // Call sqlite3_status
    if (choice % 6 == 3) {
        int current = 0;
        int highwater = 0;
        // Try different status parameters
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
        
        int op_index = choice % (sizeof(op_codes) / sizeof(op_codes[0]));
        sqlite3_status(op_codes[op_index], &current, &highwater, reset_flag);
    }
    
    // Call sqlite3_status64
    if (choice % 6 == 4) {
        sqlite3_int64 current64 = 0;
        sqlite3_int64 highwater64 = 0;
        // Try different status parameters
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
        
        int op_index = choice % (sizeof(op_codes) / sizeof(op_codes[0]));
        sqlite3_status64(op_codes[op_index], &current64, &highwater64, reset_flag);
    }
    
    // Call sqlite3_global_recover
    if (choice % 6 == 5) {
        sqlite3_global_recover();
    }
    
    // Also try some combinations of calls
    if (Size > 3) {
        // Get highwater mark and then try to release memory
        sqlite3_int64 hw = sqlite3_memory_highwater(0);
        (void)hw;
        
        int bytes_freed = sqlite3_release_memory(int_param);
        (void)bytes_freed;
        
        // Check status after memory release
        int current = 0, highwater = 0;
        sqlite3_status(SQLITE_STATUS_MEMORY_USED, &current, &highwater, 0);
    }
    
    return 0;
}