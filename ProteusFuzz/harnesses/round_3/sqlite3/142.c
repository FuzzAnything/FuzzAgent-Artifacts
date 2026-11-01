// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static void cleanup_memory(void **ptrs, int count) {
    for (int i = 0; i < count; i++) {
        if (ptrs[i] != NULL) {
            sqlite3_free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Track allocated pointers to ensure proper cleanup
    void *ptrs[10];
    int ptr_count = 0;
    memset(ptrs, 0, sizeof(ptrs));

    // Initialize SQLite memory tracking
    sqlite3_memory_highwater(1);

    // Parse input to determine operations
    // Use first byte to decide operation type
    int op = Data[0] % 6;
    size_t idx = 1;

    // Helper macro to check bounds
    #define CHECK_IDX(n) (idx + (n) <= Size)

    switch (op) {
        case 0: // Test sqlite3_malloc
            if (CHECK_IDX(1)) {
                int size = (int)Data[idx];
                idx++;
                if (size > 0 && size < 10000000) {
                    void *p = sqlite3_malloc(size);
                    if (p != NULL) {
                        memset(p, 0xAA, size); // Write pattern
                        if (ptr_count < 10) {
                            ptrs[ptr_count++] = p;
                        }
                    }
                }
            }
            break;

        case 1: // Test sqlite3_malloc64
            if (CHECK_IDX(8)) {
                sqlite3_uint64 size64 = 0;
                memcpy(&size64, &Data[idx], sizeof(sqlite3_uint64));
                idx += 8;
                if (size64 > 0 && size64 < 10000000000ULL) {
                    void *p = sqlite3_malloc64(size64);
                    if (p != NULL) {
                        size_t write_size = (size64 < 10000000000ULL) ? (size_t)size64 : 10000000000ULL;
                        memset(p, 0xBB, write_size);
                        if (ptr_count < 10) {
                            ptrs[ptr_count++] = p;
                        }
                    }
                }
            }
            break;

        case 2: // Test sqlite3_realloc64
            if (CHECK_IDX(8)) {
                sqlite3_uint64 new_size = 0;
                memcpy(&new_size, &Data[idx], sizeof(sqlite3_uint64));
                idx += 8;
                
                if (ptr_count > 0 && CHECK_IDX(1)) {
                    int ptr_idx = (Data[idx] % ptr_count);
                    idx++;
                    void *old_ptr = ptrs[ptr_idx];
                    void *new_ptr = sqlite3_realloc64(old_ptr, new_size);
                    if (new_ptr != NULL) {
                        ptrs[ptr_idx] = new_ptr;
                        // Write pattern to new size
                        if (new_size > 0) {
                            size_t write_size = (new_size < 10000000000ULL) ? (size_t)new_size : 10000000000ULL;
                            memset(new_ptr, 0xCC, write_size);
                        }
                    }
                    // If realloc fails, original pointer remains valid and is not freed
                } else if (ptr_count == 0) {
                    // Realloc with NULL (acts like malloc)
                    if (new_size > 0 && new_size < 10000000000ULL) {
                        void *p = sqlite3_realloc64(NULL, new_size);
                        if (p != NULL) {
                            size_t write_size = (new_size < 10000000000ULL) ? (size_t)new_size : 10000000000ULL;
                            memset(p, 0xDD, write_size);
                            if (ptr_count < 10) {
                                ptrs[ptr_count++] = p;
                            }
                        }
                    }
                }
            }
            break;

        case 3: // Test sqlite3_msize
            if (ptr_count > 0 && CHECK_IDX(1)) {
                int ptr_idx = (Data[idx] % ptr_count);
                idx++;
                void *p = ptrs[ptr_idx];
                sqlite3_uint64 size = sqlite3_msize(p);
                (void)size; // Use the size to avoid unused variable warning
                // Also test msize on NULL
                sqlite3_uint64 size_null = sqlite3_msize(NULL);
                (void)size_null;
            }
            break;

        case 4: // Test sqlite3_memory_used
            {
                sqlite3_int64 used = sqlite3_memory_used();
                (void)used;
            }
            break;

        case 5: // Test sqlite3_memory_highwater
            if (CHECK_IDX(1)) {
                int reset = (Data[idx] % 2);
                idx++;
                sqlite3_int64 hw = sqlite3_memory_highwater(reset);
                (void)hw;
            }
            break;
    }

    // Cleanup all allocated memory
    cleanup_memory(ptrs, ptr_count);

    return 0;
}