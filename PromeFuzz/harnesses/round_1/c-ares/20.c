// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_library_init_mem at ares_library_init.c:123:5 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
// ares_init at ares_init.c:67:5 in ares.h
// ares_destroy at ares_destroy.c:32:6 in ares.h
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
#include <stdio.h>

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
    // Use fuzzer input to control flags
    int flags = 0;
    if (Size > 0) {
        flags = Data[0] % 256;
    }

    // Initialize library with custom memory allocators
    int init_result = ares_library_init_mem(flags, custom_malloc, custom_free, custom_realloc);
    if (init_result != ARES_SUCCESS) {
        return 0;
    }

    // Initialize channel multiple times to explore different states
    ares_channel_t *channel = NULL;
    
    // First initialization attempt
    int channel_result = ares_init(&channel);
    
    // If successful, we can try to initialize again or modify state
    if (channel_result == ARES_SUCCESS && channel != NULL) {
        // Clean up the first channel properly
        ares_destroy(channel);
        
        // Try to create another channel to explore different states
        ares_channel_t *channel2 = NULL;
        int channel2_result = ares_init(&channel2);
        
        // Clean up second channel if created
        if (channel2_result == ARES_SUCCESS && channel2 != NULL) {
            ares_destroy(channel2);
        }
    }

    // Use fuzzer data to influence behavior
    if (Size > 1) {
        // Use second byte to decide whether to attempt reinitialization
        if (Data[1] % 2 == 0) {
            ares_channel_t *channel3 = NULL;
            int channel3_result = ares_init(&channel3);
            if (channel3_result == ARES_SUCCESS && channel3 != NULL) {
                ares_destroy(channel3);
            }
        }
    }

    // If the initial channel creation failed but channel was partially allocated
    if (channel_result != ARES_SUCCESS && channel != NULL) {
        ares_destroy(channel);
    }

    // Cleanup library
    ares_library_cleanup();

    return 0;
}