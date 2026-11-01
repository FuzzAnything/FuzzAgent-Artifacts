// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
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

static void cleanup(void *ptr) {
    if (ptr != NULL) {
        sqlite3_free(ptr);
    }
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test sqlite3_memory_used and sqlite3_memory_highwater
    sqlite3_int64 used = sqlite3_memory_used();
    (void)used;
    
    sqlite3_int64 highwater = sqlite3_memory_highwater(0);
    (void)highwater;

    // Reset highwater mark
    sqlite3_int64 old_highwater = sqlite3_memory_highwater(1);
    (void)old_highwater;

    // Extract size from input data
    sqlite3_uint64 size32 = 0;
    sqlite3_uint64 size64 = 0;
    
    if (Size >= 4) {
        memcpy(&size32, Data, 4);
        size32 &= 0x7FFFFFFF; // Ensure it fits in signed int32 range for sqlite3_malloc
    }
    
    if (Size >= 8) {
        memcpy(&size64, Data, 8);
    } else if (Size > 0) {
        size64 = (sqlite3_uint64)Data[0] * 256 + (Size > 1 ? Data[1] : 0);
    }

    // Limit sizes to prevent immediate OOM or undefined behavior in fuzzer
    if (size32 > 1024 * 1024) size32 = 1024 * 1024;
    if (size64 > 1024 * 1024) size64 = 1024 * 1024;

    // Test sqlite3_malloc
    void *ptr_malloc = sqlite3_malloc((int)size32);
    if (ptr_malloc != NULL) {
        sqlite3_uint64 msize_malloc = sqlite3_msize(ptr_malloc);
        (void)msize_malloc;
        cleanup(ptr_malloc);
    }

    // Test sqlite3_malloc64
    void *ptr_malloc64 = sqlite3_malloc64(size64);
    if (ptr_malloc64 != NULL) {
        sqlite3_uint64 msize_malloc64 = sqlite3_msize(ptr_malloc64);
        (void)msize_malloc64;
        cleanup(ptr_malloc64);
    }

    // Test sqlite3_realloc64 with NULL (acts like malloc)
    void *ptr_realloc64_new = sqlite3_realloc64(NULL, size64);
    if (ptr_realloc64_new != NULL) {
        sqlite3_uint64 msize_realloc64_new = sqlite3_msize(ptr_realloc64_new);
        (void)msize_realloc64_new;
        cleanup(ptr_realloc64_new);
    }

    // Test sqlite3_realloc64 with existing allocation
    void *ptr_base = sqlite3_malloc64(size64 > 0 ? size64 : 1);
    if (ptr_base != NULL) {
        // Try to realloc to different sizes
        sqlite3_uint64 new_size = (size64 > 0) ? (size64 * 2) : 10;
        if (new_size > 1024 * 1024) new_size = 1024 * 1024;
        
        void *ptr_realloc64 = sqlite3_realloc64(ptr_base, new_size);
        if (ptr_realloc64 != NULL) {
            sqlite3_uint64 msize_realloc64 = sqlite3_msize(ptr_realloc64);
            (void)msize_realloc64;
            cleanup(ptr_realloc64);
        } else {
            // If realloc failed, the original pointer is still valid
            cleanup(ptr_base);
        }
    }

    // Test sqlite3_realloc64 with zero size (acts like free)
    void *ptr_to_free = sqlite3_malloc64(100);
    if (ptr_to_free != NULL) {
        void *ptr_realloc_zero = sqlite3_realloc64(ptr_to_free, 0);
        if (ptr_realloc_zero != NULL) {
            // Should not happen if implementation follows spec (0 size -> free)
            cleanup(ptr_realloc_zero);
        }
        // ptr_to_free is now freed
    }

    // Final check on memory stats
    (void)sqlite3_memory_used();
    (void)sqlite3_memory_highwater(0);

    return 0;
}