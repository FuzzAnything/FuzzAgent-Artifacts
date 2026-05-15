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

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // 1. Call sqlite3_sourceid
    const char *source_id = sqlite3_sourceid();
    (void)source_id; // Suppress unused variable warning
    
    // 2. Call sqlite3_compileoption_get with N derived from fuzz input
    int N = 0;
    if (Size > 0) {
        N = Data[0]; // Use first byte as N value
    }
    const char *compile_option = sqlite3_compileoption_get(N);
    (void)compile_option; // Suppress unused variable warning
    
    // 3. Call sqlite3_sleep with milliseconds derived from fuzz input
    int ms = 0;
    if (Size > 1) {
        ms = Data[1]; // Use second byte as milliseconds
    }
    int slept_ms = sqlite3_sleep(ms);
    (void)slept_ms; // Suppress unused variable warning
    
    // 4. Call sqlite3_free with memory that might be allocated
    // First try to allocate some memory using sqlite3_malloc
    void *mem_to_free = NULL;
    if (Size > 2) {
        // Use third byte as allocation size (capped to avoid excessive memory)
        size_t alloc_size = Data[2] % 256; // Keep it small for fuzzing
        if (alloc_size > 0) {
            mem_to_free = sqlite3_malloc(alloc_size);
        }
    }
    sqlite3_free(mem_to_free);
    
    return 0;
}