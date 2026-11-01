// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_msize at sqlite3.c:31468:27 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:31262:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* 
 * Fuzz driver for SQLite3 memory management functions.
 * Targets: sqlite3_malloc, sqlite3_malloc64, sqlite3_realloc64, 
 *          sqlite3_msize, sqlite3_memory_used, sqlite3_memory_highwater.
 */

static void cleanup_memory(void **ptrs, int count) {
    for (int i = 0; i < count; i++) {
        if (ptrs[i] != NULL) {
            sqlite3_free(ptrs[i]);
            ptrs[i] = NULL;
        }
    }
}

int LLVMFuzzerTestOneInput_140(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* 
     * We maintain a small pool of allocated pointers to test realloc and msize.
     * We use a fixed size array to avoid dynamic allocation of the tracker itself
     * during the fuzzing loop, keeping overhead low.
     */
    #define MAX_TRACKED 10
    void *tracked_ptrs[MAX_TRACKED] = {0};
    int tracked_count = 0;

    /* 
     * Parse input to determine operations.
     * Byte 0: Operation selector (0-5)
     * Byte 1: Index of pointer to operate on (0-9) or flags
     * Rest: Size parameters
     */
    uint8_t op = Data[0] % 6;
    uint8_t idx = Data[1] % MAX_TRACKED;

    sqlite3_uint64 size64 = 0;
    int size32 = 0;

    /* Extract size from input data if available */
    if (Size >= 6) {
        size64 = 0;
        for (int i = 2; i < 8 && i < Size; i++) {
            size64 = (size64 << 8) | Data[i];
        }
        /* Ensure size is not excessively large to prevent OOM during fuzzing */
        if (size64 > 1024 * 1024) { 
            size64 = 1024 * 1024; 
        }
        size32 = (int)size64;
    } else if (Size >= 4) {
        size32 = (int)Data[2] | ((int)Data[3] << 8);
        if (size32 < 0) size32 = 0;
        if (size32 > 1024 * 1024) size32 = 1024 * 1024;
        size64 = (sqlite3_uint64)size32;
    } else {
        /* Default small size if not enough data */
        size32 = 100;
        size64 = 100;
    }

    /* 
     * Perform the selected operation 
     */
    switch (op) {
        case 0: {
            /* sqlite3_malloc */
            void *ptr = sqlite3_malloc(size32);
            if (ptr != NULL) {
                /* Store in tracker if space available */
                if (tracked_count < MAX_TRACKED) {
                    tracked_ptrs[tracked_count++] = ptr;
                } else {
                    /* If full, free immediately */
                    sqlite3_free(ptr);
                }
            }
            break;
        }
        case 1: {
            /* sqlite3_malloc64 */
            void *ptr = sqlite3_malloc64(size64);
            if (ptr != NULL) {
                if (tracked_count < MAX_TRACKED) {
                    tracked_ptrs[tracked_count++] = ptr;
                } else {
                    sqlite3_free(ptr);
                }
            }
            break;
        }
        case 2: {
            /* sqlite3_realloc64 */
            void *old_ptr = (idx < tracked_count) ? tracked_ptrs[idx] : NULL;
            void *new_ptr = sqlite3_realloc64(old_ptr, size64);
            
            if (new_ptr != NULL) {
                if (idx < tracked_count) {
                    tracked_ptrs[idx] = new_ptr;
                } else {
                    /* If we reallocated NULL (acting as malloc) or an untracked ptr */
                    if (old_ptr == NULL && tracked_count < MAX_TRACKED) {
                        tracked_ptrs[tracked_count++] = new_ptr;
                    } else {
                        /* If we reallocated an untracked non-NULL, we can't track it safely */
                        sqlite3_free(new_ptr);
                    }
                }
            } else {
                /* realloc failed. If old_ptr was tracked, it remains valid. */
                /* If old_ptr was NULL, nothing to do. */
            }
            break;
        }
        case 3: {
            /* sqlite3_msize */
            if (idx < tracked_count) {
                void *ptr = tracked_ptrs[idx];
                sqlite3_uint64 sz = sqlite3_msize(ptr);
                /* Use sz to prevent unused variable warning */
                (void)sz;
            } else {
                /* Test msize with NULL */
                sqlite3_uint64 sz = sqlite3_msize(NULL);
                (void)sz;
            }
            break;
        }
        case 4: {
            /* sqlite3_memory_used */
            sqlite3_int64 used = sqlite3_memory_used();
            (void)used;
            break;
        }
        case 5: {
            /* sqlite3_memory_highwater */
            int reset = (Data[1] & 1);
            sqlite3_int64 hw = sqlite3_memory_highwater(reset);
            (void)hw;
            break;
        }
    }

    /* 
     * Cleanup all tracked memory before returning.
     * This ensures no leaks between fuzzing iterations.
     */
    cleanup_memory(tracked_ptrs, tracked_count);

    return 0;
}