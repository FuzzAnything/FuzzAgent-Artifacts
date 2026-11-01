// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void safe_free(void *ptr) {
    if (ptr != NULL) {
        sqlite3_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_106(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize SQLite (optional as functions auto-init, but good practice)
    if (sqlite3_initialize() != SQLITE_OK) {
        return 0;
    }

    // Test sqlite3_memory_used and sqlite3_memory_highwater
    sqlite3_int64 mem_used = sqlite3_memory_used();
    sqlite3_int64 mem_highwater = sqlite3_memory_highwater(0);
    
    // Reset highwater mark
    sqlite3_int64 prev_highwater = sqlite3_memory_highwater(1);

    // Use fuzz input to determine allocation sizes
    // Ensure sizes are within reasonable bounds to avoid immediate crashes or OOM
    sqlite3_uint64 size64 = 0;
    int size32 = 0;
    
    if (Size >= 8) {
        size64 = *(const sqlite3_uint64 *)(Data);
        // Cap at a reasonable limit for fuzzing (e.g., 10MB) to prevent resource exhaustion
        if (size64 > 10 * 1024 * 1024) {
            size64 = 10 * 1024 * 1024;
        }
    } else if (Size >= 4) {
        size64 = *(const uint32_t *)(Data);
    } else if (Size >= 1) {
        size64 = Data[0];
    }

    // Convert to 32-bit for sqlite3_malloc if needed, capping at INT_MAX
    if (size64 > 2147483647) {
        size32 = 2147483647;
    } else {
        size32 = (int)size64;
    }

    // Test sqlite3_malloc64
    void *ptr64 = sqlite3_malloc64(size64);
    if (ptr64 != NULL) {
        // Write some data to ensure the allocation is usable
        memset(ptr64, 0xAA, size64);
        
        // Test sqlite3_msize
        sqlite3_uint64 msize = sqlite3_msize(ptr64);
        
        // Test sqlite3_realloc64 with same pointer
        sqlite3_uint64 new_size64 = size64 * 2;
        if (new_size64 > 10 * 1024 * 1024) new_size64 = 10 * 1024 * 1024;
        
        void *ptr64_realloc = sqlite3_realloc64(ptr64, new_size64);
        if (ptr64_realloc != NULL) {
            memset(ptr64_realloc, 0xBB, new_size64);
            safe_free(ptr64_realloc);
        } else {
            // If realloc failed, original pointer is still valid
            safe_free(ptr64);
        }
    }

    // Test sqlite3_malloc
    void *ptr32 = sqlite3_malloc(size32);
    if (ptr32 != NULL) {
        memset(ptr32, 0xCC, size32);
        
        // Test sqlite3_msize
        sqlite3_uint64 msize32 = sqlite3_msize(ptr32);
        
        // Test sqlite3_realloc64 on 32-bit allocation
        sqlite3_uint64 realloc_size = size64 + 100;
        if (realloc_size > 10 * 1024 * 1024) realloc_size = 10 * 1024 * 1024;
        
        void *ptr32_realloc = sqlite3_realloc64(ptr32, realloc_size);
        if (ptr32_realloc != NULL) {
            memset(ptr32_realloc, 0xDD, realloc_size);
            safe_free(ptr32_realloc);
        } else {
            safe_free(ptr32);
        }
    }

    // Test edge cases: NULL pointer realloc
    void *ptr_null_realloc = sqlite3_realloc64(NULL, size64);
    if (ptr_null_realloc != NULL) {
        memset(ptr_null_realloc, 0xEE, size64);
        safe_free(ptr_null_realloc);
    }

    // Test edge cases: zero size
    void *ptr_zero = sqlite3_malloc64(0);
    if (ptr_zero != NULL) {
        safe_free(ptr_zero);
    }
    
    // Test negative size (cast to unsigned, but logic inside handles it)
    // We rely on the library's internal check for <= 0
    
    // Check memory stats again
    (void)sqlite3_memory_used();
    (void)sqlite3_memory_highwater(0);

    // Shutdown (optional, but good for cleanup)
    sqlite3_shutdown();

    return 0;
}