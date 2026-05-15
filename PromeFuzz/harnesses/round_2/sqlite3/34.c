// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // 1. Call sqlite3_sourceid
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Use result to avoid unused variable warning
    
    // 2. Call sqlite3_compileoption_get with various indices
    // Use fuzz input to determine which indices to try
    int compile_option_index = 0;
    if (Size > 0) {
        // Use first byte to determine starting index
        compile_option_index = Data[0];
    }
    
    // Try a few indices around the calculated one
    for (int i = 0; i < 5; i++) {
        const char *option = sqlite3_compileoption_get(compile_option_index + i);
        (void)option; // Use result to avoid unused variable warning
    }
    
    // 3. Call sqlite3_sleep with duration from fuzz input
    int sleep_duration = 0;
    if (Size > 1) {
        // Use second byte for sleep duration (convert to int)
        sleep_duration = (int)Data[1];
    }
    int actual_sleep = sqlite3_sleep(sleep_duration);
    (void)actual_sleep; // Use result to avoid unused variable warning
    
    // 4. Call sqlite3_free with various allocations
    // Allocate some memory using sqlite3_malloc and free it
    void *mem1 = sqlite3_malloc(Size);
    if (mem1) {
        // Copy fuzz data into allocated memory
        if (Size > 0) {
            memcpy(mem1, Data, Size);
        }
        sqlite3_free(mem1);
    }
    
    // Try with fixed size allocations
    void *mem2 = sqlite3_malloc(128);
    sqlite3_free(mem2);
    
    // Test with NULL pointer (should be safe)
    sqlite3_free(NULL);
    
    // Test with small allocation
    if (Size > 2) {
        size_t alloc_size = Data[2] % 256;
        void *mem3 = sqlite3_malloc(alloc_size);
        sqlite3_free(mem3);
    }
    
    return 0;
}