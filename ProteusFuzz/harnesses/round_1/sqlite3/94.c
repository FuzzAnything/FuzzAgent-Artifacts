// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:25040:16 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to safely open a database connection.
 * Uses a dummy file path as per instructions.
 */
static sqlite3 *safe_open_db(void) {
    sqlite3 *db = NULL;
    /* Open a file-based database to ensure full functionality */
    if (sqlite3_open("./dummy_file", &db) != SQLITE_OK) {
        return NULL;
    }
    return db;
}

/*
 * Helper function to close the database and cleanup.
 */
static void safe_close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Helper to extract an integer from the fuzz input buffer safely.
 */
static int get_int_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + 4 > Size) {
        *offset = Size;
        return 0;
    }
    int val = (int)(Data[*offset] | (Data[*offset + 1] << 8) |
                    (Data[*offset + 2] << 16) | (Data[*offset + 3] << 24));
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    sqlite3 *db = safe_open_db();
    if (!db) {
        return 0;
    }

    size_t offset = 0;
    int op_code = get_int_from_input(Data, Size, &offset);
    int reset_flag = get_int_from_input(Data, Size, &offset);
    int release_n = get_int_from_input(Data, Size, &offset);
    int heap_limit = get_int_from_input(Data, Size, &offset);
    int mem_reset = get_int_from_input(Data, Size, &offset);

    /*
     * 1. Fuzz sqlite3_db_status
     * Inputs: db, op, pCur, pHiwtr, resetFlg
     * We use a variety of valid and invalid op codes to explore states.
     * Only using defined constants to avoid compilation errors.
     */
    int pCur = 0;
    int pHiwtr = 0;
    
    /* Array of known valid status options and some edge cases */
    int ops_to_try[] = {
        SQLITE_DBSTATUS_LOOKASIDE_USED,
        SQLITE_DBSTATUS_CACHE_USED,
        SQLITE_DBSTATUS_SCHEMA_USED,
        SQLITE_DBSTATUS_CACHE_HIT,
        SQLITE_DBSTATUS_CACHE_MISS,
        SQLITE_DBSTATUS_CACHE_WRITE,
        SQLITE_DBSTATUS_DEFERRED_FKS,
        -1,
        0,
        op_code
    };
    int num_ops = sizeof(ops_to_try) / sizeof(ops_to_try[0]);
    
    for (int i = 0; i < num_ops; i++) {
        sqlite3_db_status(db, ops_to_try[i], &pCur, &pHiwtr, reset_flag % 2);
        sqlite3_db_status(db, ops_to_try[i], &pCur, &pHiwtr, 0);
        sqlite3_db_status(db, ops_to_try[i], &pCur, &pHiwtr, 1);
    }

    /*
     * 2. Fuzz sqlite3_release_memory
     * Input: N bytes to release.
     * Try positive, negative, zero, and large values.
     */
    sqlite3_release_memory(release_n);
    sqlite3_release_memory(0);
    sqlite3_release_memory(-1);
    sqlite3_release_memory(1024);
    sqlite3_release_memory(1000000000);

    /*
     * 3. Fuzz sqlite3_malloc
     * Input: N bytes to allocate.
     * We must free the allocated memory to avoid leaks.
     */
    int alloc_size = get_int_from_input(Data, Size, &offset);
    
    /* Test various sizes including edge cases */
    int sizes_to_try[] = {
        alloc_size,
        0,
        -1,
        1,
        1024,
        65536,
        1000000
    };
    int num_sizes = sizeof(sizes_to_try) / sizeof(sizes_to_try[0]);

    for (int i = 0; i < num_sizes; i++) {
        void *ptr = sqlite3_malloc(sizes_to_try[i]);
        if (ptr != NULL) {
            /* Write some data to ensure the memory is valid */
            int safe_size = sizes_to_try[i] > 0 ? sizes_to_try[i] : 0;
            if (safe_size > 0) {
                memset(ptr, 0xAA, safe_size);
            }
            sqlite3_free(ptr);
        }
    }

    /*
     * 4. Fuzz sqlite3_db_release_memory
     * Input: db connection.
     * This function is safe to call multiple times.
     */
    sqlite3_db_release_memory(db);

    /*
     * 5. Fuzz sqlite3_soft_heap_limit
     * Input: N (soft heap limit).
     * Note: This is deprecated but still functional.
     */
    sqlite3_soft_heap_limit(heap_limit);
    sqlite3_soft_heap_limit(0);
    sqlite3_soft_heap_limit(-1);
    sqlite3_soft_heap_limit(1000000000);

    /*
     * 6. Fuzz sqlite3_memory_highwater
     * Input: resetFlag (0 or 1).
     */
    sqlite3_int64 hw1 = sqlite3_memory_highwater(mem_reset % 2);
    sqlite3_int64 hw2 = sqlite3_memory_highwater(0);
    sqlite3_int64 hw3 = sqlite3_memory_highwater(1);

    /* Cleanup */
    safe_close_db(db);

    return 0;
}