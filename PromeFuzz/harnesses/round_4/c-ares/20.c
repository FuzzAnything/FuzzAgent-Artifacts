// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_library_init_mem at ares_library_init.c:123:5 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void* custom_malloc(size_t size) {
    return malloc(size);
}

static void custom_free(void* ptr) {
    free(ptr);
}

static void* custom_realloc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    // Initialize library with custom memory allocators
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] % 256;  // Use first byte for flags
    }
    
    int init_result = ares_library_init_mem(flags, custom_malloc, custom_free, custom_realloc);
    
    // Only proceed if library initialization succeeded
    if (init_result == ARES_SUCCESS) {
        ares_channel_t* channel = NULL;
        
        // Initialize channel
        int channel_result = ares_init(&channel);
        
        // Clean up channel if initialization succeeded
        if (channel_result == ARES_SUCCESS && channel != NULL) {
            // Note: ares_destroy is not in the target list but is needed for cleanup
            // We'll use it anyway since it's part of proper channel cleanup
            ares_destroy(channel);
        }
    }
    
    // Clean up library
    ares_library_cleanup();
    
    return 0;
}