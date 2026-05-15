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
#include <stdint.h>
#include <stdlib.h>

int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    // Call sqlite3_sourceid - no arguments needed
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Avoid unused variable warning
    
    // Call sqlite3_compileoption_get - use first byte of input as N parameter
    int N = 0;
    if (Size > 0) {
        N = (int)Data[0];
    }
    const char *compile_option = sqlite3_compileoption_get(N);
    (void)compile_option; // Avoid unused variable warning
    
    // Call sqlite3_sleep - use second byte of input as sleep milliseconds
    int sleep_ms = 0;
    if (Size > 1) {
        sleep_ms = (int)Data[1];
    }
    int actual_sleep = sqlite3_sleep(sleep_ms);
    (void)actual_sleep; // Avoid unused variable warning
    
    // Call sqlite3_free - allocate some memory using sqlite3_malloc then free it
    // Use third byte of input as allocation size (capped at reasonable value)
    size_t alloc_size = 0;
    if (Size > 2) {
        alloc_size = (size_t)Data[2];
        // Cap at 1024 bytes to avoid excessive allocations
        if (alloc_size > 1024) {
            alloc_size = 1024;
        }
    }
    
    void *mem_to_free = NULL;
    if (alloc_size > 0) {
        mem_to_free = sqlite3_malloc((int)alloc_size);
    }
    sqlite3_free(mem_to_free);
    
    return 0;
}