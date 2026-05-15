// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_sourceid at sqlite3.c:252014:24 in sqlite3.h
// sqlite3_compileoption_get at sqlite3.c:176054:24 in sqlite3.h
// sqlite3_sleep at sqlite3.c:174998:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Call sqlite3_sourceid - no parameters needed
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Use variable to avoid unused warning
    
    // Call sqlite3_compileoption_get - use fuzzer input to determine N
    int N = 0;
    if (Size >= sizeof(int)) {
        memcpy(&N, Data, sizeof(int));
    } else if (Size > 0) {
        N = Data[0];
    }
    
    const char *compile_option = sqlite3_compileoption_get(N);
    (void)compile_option; // Use variable to avoid unused warning
    
    // Call sqlite3_sleep - use fuzzer input to determine sleep time
    int sleep_ms = 0;
    if (Size >= 2 * sizeof(int)) {
        memcpy(&sleep_ms, Data + sizeof(int), sizeof(int));
    } else if (Size > sizeof(int)) {
        sleep_ms = Data[sizeof(int)];
    }
    
    int actual_sleep = sqlite3_sleep(sleep_ms);
    (void)actual_sleep; // Use variable to avoid unused warning
    
    // Call sqlite3_free - free memory if we have any
    if (Size > 0) {
        // Allocate some memory to free
        void *mem_to_free = sqlite3_malloc(Size);
        if (mem_to_free) {
            sqlite3_free(mem_to_free);
        }
    } else {
        // Test with NULL pointer (should be safe)
        sqlite3_free(NULL);
    }
    
    return 0;
}