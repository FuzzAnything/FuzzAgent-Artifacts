// This fuzz driver is generated for library cares, aiming to fuzz the following functions:
// ares_library_initialized at ares_library_init.c:161:5 in ares.h
// ares_library_init at ares_library_init.c:108:5 in ares.h
// ares_library_initialized at ares_library_init.c:161:5 in ares.h
// ares_library_cleanup at ares_library_init.c:139:6 in ares.h
// ares_library_initialized at ares_library_init.c:161:5 in ares.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "ares.h"

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    // Use first 4 bytes of fuzzer input as flags for ares_library_init
    int flags = 0;
    if (Size >= sizeof(int)) {
        flags = *(const int *)Data;
    }
    
    // Call functions in specified order
    ares_library_initialized();
    ares_library_init(flags);
    ares_library_initialized();
    ares_library_cleanup();
    ares_library_initialized();
    
    return 0;
}