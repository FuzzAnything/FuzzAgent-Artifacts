// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_memory_api(const uint8_t *Data, size_t Size) {
    if (Size == 0) return;

    // Use fuzzer input to determine allocation sizes and operations
    sqlite3_uint64 size64 = 0;
    int size32 = 0;
    
    if (Size >= 8) {
        memcpy(&size64, Data, 8);
        // Limit size to prevent immediate OOM or excessive memory usage
        if (size64 > 1024 * 1024) size64 = 1024 * 1024; 
    } else if (Size >= 4) {
        memcpy(&size32, Data, 4);
        if (size32 > 1024 * 1024) size32 = 1024 * 1024;
    }

    // Test sqlite3_memory_used and sqlite3_memory_highwater
    sqlite3_int64 used_before = sqlite3_memory_used();
    sqlite3_int64 hw_before = sqlite3_memory_highwater(0);
    
    // Reset highwater mark
    sqlite3_int64 hw_reset = sqlite3_memory_highwater(1);
    (void)hw_reset;

    // Test sqlite3_malloc
    void *ptr_malloc = sqlite3_malloc(size32);
    if (ptr_malloc) {
        sqlite3_uint64 msize_malloc = sqlite3_msize(ptr_malloc);
        (void)msize_malloc;
        // Write some data to avoid unused variable warnings and trigger memory patterns
        memset(ptr_malloc, 0x41, size32 > 0 ? size32 : 1);
    }

    // Test sqlite3_malloc64
    void *ptr_malloc64 = sqlite3_malloc64(size64);
    if (ptr_malloc64) {
        sqlite3_uint64 msize_malloc64 = sqlite3_msize(ptr_malloc64);
        (void)msize_malloc64;
        memset(ptr_malloc64, 0x42, size64 > 0 ? size64 : 1);
    }

    // Test sqlite3_realloc on malloc pointer
    if (ptr_malloc) {
        sqlite3_uint64 new_size = size64 + 100;
        if (new_size > 1024 * 1024) new_size = 1024 * 1024;
        void *ptr_realloc = sqlite3_realloc64(ptr_malloc, new_size);
        if (ptr_realloc) {
            sqlite3_uint64 msize_realloc = sqlite3_msize(ptr_realloc);
            (void)msize_realloc;
            memset(ptr_realloc, 0x43, new_size > 0 ? new_size : 1);
            ptr_malloc = ptr_realloc; // Update pointer
        } else {
            // If realloc fails, original memory is still valid (if new_size > 0)
            // We should free it later
        }
    }

    // Test sqlite3_realloc on malloc64 pointer
    if (ptr_malloc64) {
        sqlite3_uint64 new_size64 = size64 * 2;
        if (new_size64 > 1024 * 1024) new_size64 = 1024 * 1024;
        void *ptr_realloc64 = sqlite3_realloc64(ptr_malloc64, new_size64);
        if (ptr_realloc64) {
            sqlite3_uint64 msize_realloc64 = sqlite3_msize(ptr_realloc64);
            (void)msize_realloc64;
            memset(ptr_realloc64, 0x44, new_size64 > 0 ? new_size64 : 1);
            ptr_malloc64 = ptr_realloc64; // Update pointer
        }
    }

    // Test sqlite3_realloc with NULL (acts as malloc)
    void *ptr_realloc_null = sqlite3_realloc64(NULL, size64);
    if (ptr_realloc_null) {
        sqlite3_uint64 msize_null = sqlite3_msize(ptr_realloc_null);
        (void)msize_null;
        memset(ptr_realloc_null, 0x45, size64 > 0 ? size64 : 1);
    }

    // Test sqlite3_msize on NULL
    sqlite3_uint64 msize_null_ptr = sqlite3_msize(NULL);
    (void)msize_null_ptr;

    // Check memory status after allocations
    sqlite3_int64 used_after = sqlite3_memory_used();
    sqlite3_int64 hw_after = sqlite3_memory_highwater(0);
    (void)used_after;
    (void)hw_after;

    // Cleanup: Free all allocated memory
    if (ptr_malloc) {
        sqlite3_free(ptr_malloc);
    }
    if (ptr_malloc64) {
        sqlite3_free(ptr_malloc64);
    }
    if (ptr_realloc_null) {
        sqlite3_free(ptr_realloc_null);
    }

    // Final check
    sqlite3_int64 used_final = sqlite3_memory_used();
    sqlite3_int64 hw_final = sqlite3_memory_highwater(1);
    (void)used_final;
    (void)hw_final;
}

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    fuzz_memory_api(Data, Size);
    return 0;
}