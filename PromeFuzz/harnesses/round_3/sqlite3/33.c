// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Call sqlite3_sourceid - no parameters needed
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Mark as used to avoid unused variable warning
    
    // Call sqlite3_compileoption_get - use fuzzer input to determine N
    int n = 0;
    if (Size >= sizeof(int)) {
        memcpy(&n, Data, sizeof(int));
        // Use modulo to limit the range and avoid excessive iterations
        n = n % 100;
    }
    
    // Loop through compile options up to n
    for (int i = 0; i < n; i++) {
        const char *option = sqlite3_compileoption_get(i);
        (void)option; // Mark as used
    }
    
    // Call sqlite3_sleep - use fuzzer input for sleep duration
    int sleep_ms = 0;
    if (Size >= 2 * sizeof(int)) {
        memcpy(&sleep_ms, Data + sizeof(int), sizeof(int));
        // Ensure non-negative to avoid platform-specific behavior
        if (sleep_ms < 0) {
            sleep_ms = 0;
        }
        // Limit sleep time to avoid long fuzzing delays
        if (sleep_ms > 100) {
            sleep_ms = 100;
        }
    }
    int actual_sleep = sqlite3_sleep(sleep_ms);
    (void)actual_sleep; // Mark as used
    
    // Call sqlite3_free - allocate some memory first using sqlite3_malloc
    void *mem_to_free = NULL;
    if (Size > 0) {
        // Allocate a small amount of memory
        size_t alloc_size = Size % 1024; // Limit allocation size
        mem_to_free = sqlite3_malloc((int)alloc_size);
        if (mem_to_free && Size >= alloc_size) {
            // Copy some fuzzer data into the allocated memory
            memcpy(mem_to_free, Data, alloc_size);
        }
    }
    sqlite3_free(mem_to_free);
    
    return 0;
}