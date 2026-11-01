// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

static void consume_memory(void *ptr, size_t size) {
    if (ptr && size > 0) {
        volatile uint8_t *p = (volatile uint8_t *)ptr;
        // Limit write to avoid excessive time in fuzzer
        size_t limit = (size < 1024) ? size : 1024;
        for (size_t i = 0; i < limit; i++) {
            p[i] = 0xAA;
        }
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Parse input for sizes
    sqlite3_uint64 size1 = 0;
    sqlite3_uint64 size2 = 0;
    sqlite3_uint64 size3 = 0;

    if (Size >= 8) {
        size1 = ((sqlite3_uint64)Data[0] << 56) | ((sqlite3_uint64)Data[1] << 48) |
                ((sqlite3_uint64)Data[2] << 40) | ((sqlite3_uint64)Data[3] << 32) |
                ((sqlite3_uint64)Data[4] << 24) | ((sqlite3_uint64)Data[5] << 16) |
                ((sqlite3_uint64)Data[6] << 8) | (sqlite3_uint64)Data[7];
    }
    if (Size >= 16) {
        size2 = ((sqlite3_uint64)Data[8] << 56) | ((sqlite3_uint64)Data[9] << 48) |
                ((sqlite3_uint64)Data[10] << 40) | ((sqlite3_uint64)Data[11] << 32) |
                ((sqlite3_uint64)Data[12] << 24) | ((sqlite3_uint64)Data[13] << 16) |
                ((sqlite3_uint64)Data[14] << 8) | (sqlite3_uint64)Data[15];
    }
    if (Size >= 24) {
        size3 = ((sqlite3_uint64)Data[16] << 56) | ((sqlite3_uint64)Data[17] << 48) |
                ((sqlite3_uint64)Data[18] << 40) | ((sqlite3_uint64)Data[19] << 32) |
                ((sqlite3_uint64)Data[20] << 24) | ((sqlite3_uint64)Data[21] << 16) |
                ((sqlite3_uint64)Data[22] << 8) | (sqlite3_uint64)Data[23];
    }

    // Cap sizes to prevent huge allocations
    if (size1 > 1024 * 1024) size1 = 1024 * 1024;
    if (size2 > 1024 * 1024) size2 = 1024 * 1024;
    if (size3 > 1024 * 1024) size3 = 1024 * 1024;

    void *ptr1 = NULL;
    void *ptr2 = NULL;
    void *ptr3 = NULL;
    void *ptr4 = NULL;

    // 1. Test sqlite3_malloc64
    ptr1 = sqlite3_malloc64(size1);
    if (ptr1) {
        consume_memory(ptr1, size1);
        sqlite3_uint64 msize1 = sqlite3_msize(ptr1);
        (void)msize1;
    }

    // 2. Test sqlite3_malloc
    int size1_int = (int)(size1 % 100000);
    ptr2 = sqlite3_malloc(size1_int);
    if (ptr2) {
        consume_memory(ptr2, (size_t)size1_int);
        sqlite3_uint64 msize2 = sqlite3_msize(ptr2);
        (void)msize2;
    }

    // 3. Test sqlite3_realloc64
    // We need to track if ptr1 is still valid after this call.
    // If size2 == 0, realloc frees ptr1 and returns NULL.
    // If size2 > 0 and realloc fails, ptr1 is valid.
    // If size2 > 0 and realloc succeeds, ptr1 is freed, ptr3 is valid.
    
    int realloc_failed = 0;
    int realloc_freed_original = 0;

    ptr3 = sqlite3_realloc64(ptr1, size2);

    if (ptr3 == NULL) {
        if (size2 == 0) {
            // realloc(ptr, 0) frees ptr.
            realloc_freed_original = 1;
        } else {
            // size2 > 0 and returned NULL -> allocation failed, original untouched.
            realloc_failed = 1;
        }
    } else {
        // Success, original ptr1 is freed internally.
        realloc_freed_original = 1;
    }

    if (ptr3) {
        consume_memory(ptr3, size2);
        sqlite3_uint64 msize3 = sqlite3_msize(ptr3);
        (void)msize3;
    }

    // 4. Test sqlite3_realloc64 with NULL (acts as malloc)
    ptr4 = sqlite3_realloc64(NULL, size3);
    if (ptr4) {
        consume_memory(ptr4, size3);
        sqlite3_uint64 msize4 = sqlite3_msize(ptr4);
        (void)msize4;
    }

    // 5. Test sqlite3_realloc64 shrinking to 0 (acts as free)
    // We use ptr4 for this test.
    if (ptr4) {
        void *ptr5 = sqlite3_realloc64(ptr4, 0);
        // ptr5 should be NULL. ptr4 is now freed.
        (void)ptr5;
        // ptr4 is invalid now.
    }

    // 6. Test memory stats
    sqlite3_int64 used_after = sqlite3_memory_used();
    sqlite3_int64 highwater_reset = sqlite3_memory_highwater(1);
    (void)used_after;
    (void)highwater_reset;

    // Cleanup
    
    // Free ptr2 (allocated with malloc)
    if (ptr2) {
        sqlite3_free(ptr2);
    }

    // Free ptr3 (result of realloc on ptr1)
    if (ptr3) {
        sqlite3_free(ptr3);
    }

    // Free ptr1 if it wasn't freed by realloc
    // If realloc_freed_original is 0, then ptr1 is still valid.
    if (!realloc_freed_original && ptr1) {
        sqlite3_free(ptr1);
    }

    // ptr4 was freed by realloc(ptr4, 0) if it was non-NULL.
    // If ptr4 was NULL, nothing to do.
    // If ptr4 was non-NULL, it is now freed. Do not free again.

    return 0;
}