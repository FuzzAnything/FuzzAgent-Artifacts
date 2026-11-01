// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite memory management APIs:
 * - sqlite3_memory_used
 * - sqlite3_memory_highwater
 * - sqlite3_malloc
 * - sqlite3_malloc64
 * - sqlite3_realloc64
 * - sqlite3_msize
 *
 * This driver attempts to exercise various memory allocation paths,
 * including edge cases like zero/negative sizes, large allocations,
 * and reallocation chains.
 */

static void cleanup_allocated(void **ptrs, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (ptrs[i] != NULL) {
            sqlite3_free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
}

/* Helper to read a size from the input buffer */
static sqlite3_uint64 read_size(const uint8_t *Data, size_t *idx, size_t max_size) {
    if (*idx + sizeof(sqlite3_uint64) > max_size) {
        return 0;
    }
    sqlite3_uint64 val = 0;
    memcpy(&val, Data + *idx, sizeof(sqlite3_uint64));
    *idx += sizeof(sqlite3_uint64);
    return val;
}

/* Helper to read an int from the input buffer */
static int read_int(const uint8_t *Data, size_t *idx, size_t max_size) {
    if (*idx + sizeof(int) > max_size) {
        return 0;
    }
    int val = 0;
    memcpy(&val, Data + *idx, sizeof(int));
    *idx += sizeof(int);
    return val;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Read initial memory stats
    sqlite3_int64 mem_used_start = sqlite3_memory_used();
    sqlite3_int64 mem_highwater_start = sqlite3_memory_highwater(0);

    // Use first few bytes to determine operation type
    int op_type = Data[0] % 10;
    size_t idx = 1;

    // Store allocated pointers to free later
    void *allocs[64];
    size_t alloc_count = 0;

    // Initialize all allocs to NULL
    memset(allocs, 0, sizeof(allocs));

    // Perform operations based on op_type
    switch (op_type) {
        case 0: {
            // Test sqlite3_malloc with various sizes
            int size = read_int(Data, &idx, Size);
            if (size < 0) size = -size; // Ensure positive for malloc
            if (size > 1024 * 1024) size = 1024 * 1024; // Cap size
            void *p = sqlite3_malloc(size);
            if (p != NULL) {
                allocs[alloc_count++] = p;
                sqlite3_msize(p); // Query size
            }
            break;
        }
        case 1: {
            // Test sqlite3_malloc64 with various sizes
            sqlite3_uint64 size = read_size(Data, &idx, Size);
            if (size > 1024 * 1024 * 1024) size = 1024 * 1024 * 1024; // Cap size
            void *p = sqlite3_malloc64(size);
            if (p != NULL) {
                allocs[alloc_count++] = p;
                sqlite3_msize(p); // Query size
            }
            break;
        }
        case 2: {
            // Test sqlite3_realloc64
            int alloc_idx = read_int(Data, &idx, Size);
            if (alloc_count == 0) {
                alloc_idx = 0;
            } else {
                alloc_idx = alloc_idx % (int)alloc_count;
                if (alloc_idx < 0) alloc_idx = 0;
            }
            
            sqlite3_uint64 new_size = read_size(Data, &idx, Size);
            if (new_size > 1024 * 1024 * 1024) new_size = 1024 * 1024 * 1024;
            
            void *old_ptr = allocs[alloc_idx];
            void *new_ptr = sqlite3_realloc64(old_ptr, new_size);
            
            if (new_ptr != NULL) {
                allocs[alloc_idx] = new_ptr;
                sqlite3_msize(new_ptr);
            } else {
                // If realloc failed and new_size > 0, old memory is still valid
                // We keep it in allocs[alloc_idx]
            }
            break;
        }
        case 3: {
            // Test sqlite3_memory_used and sqlite3_memory_highwater
            sqlite3_int64 used = sqlite3_memory_used();
            sqlite3_int64 highwater = sqlite3_memory_highwater(0);
            (void)used;
            (void)highwater;
            break;
        }
        case 4: {
            // Reset highwater mark
            sqlite3_memory_highwater(1);
            break;
        }
        case 5: {
            // Test sqlite3_msize on NULL
            sqlite3_uint64 size = sqlite3_msize(NULL);
            (void)size;
            break;
        }
        case 6: {
            // Allocate multiple blocks and query sizes
            int count = read_int(Data, &idx, Size);
            count = count % 10;
            if (count < 0) count = 0;
            
            for (int i = 0; i < count && alloc_count < 64; i++) {
                int size = read_int(Data, &idx, Size);
                if (size < 0) size = -size;
                if (size > 1024 * 1024) size = 1024 * 1024;
                
                void *p = sqlite3_malloc(size);
                if (p != NULL) {
                    allocs[alloc_count++] = p;
                    sqlite3_msize(p);
                }
            }
            break;
        }
        case 7: {
            // Test realloc with NULL (should act like malloc)
            sqlite3_uint64 size = read_size(Data, &idx, Size);
            if (size > 1024 * 1024 * 1024) size = 1024 * 1024 * 1024;
            
            void *p = sqlite3_realloc64(NULL, size);
            if (p != NULL) {
                allocs[alloc_count++] = p;
                sqlite3_msize(p);
            }
            break;
        }
        case 8: {
            // Test realloc with size 0 (should act like free)
            int alloc_idx = read_int(Data, &idx, Size);
            if (alloc_count == 0) {
                alloc_idx = 0;
            } else {
                alloc_idx = alloc_idx % (int)alloc_count;
                if (alloc_idx < 0) alloc_idx = 0;
            }
            
            void *old_ptr = allocs[alloc_idx];
            void *new_ptr = sqlite3_realloc64(old_ptr, 0);
            
            if (new_ptr == NULL) {
                // Successfully freed
                allocs[alloc_idx] = NULL;
                // Note: We don't decrement alloc_count to avoid shifting array,
                // just mark as NULL and handle in cleanup.
            }
            break;
        }
        case 9: {
            // Test edge case: very small allocation
            void *p = sqlite3_malloc(1);
            if (p != NULL) {
                allocs[alloc_count++] = p;
                sqlite3_msize(p);
            }
            break;
        }
        default:
            break;
    }

    // Check memory stats after operations
    sqlite3_int64 mem_used_end = sqlite3_memory_used();
    sqlite3_int64 mem_highwater_end = sqlite3_memory_highwater(0);
    
    // Cleanup
    cleanup_allocated(allocs, 64); // Pass full array size, cleanup handles NULLs

    // Final memory check
    sqlite3_int64 mem_used_final = sqlite3_memory_used();
    sqlite3_int64 mem_highwater_final = sqlite3_memory_highwater(0);

    // Return 0 to indicate successful fuzzing
    return 0;
}