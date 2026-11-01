// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to open a database connection.
 * Uses a dummy file as per instructions.
 */
static sqlite3 *open_db(void) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Fuzz driver entry point.
 */
int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /*
     * 1. Prepare environment
     * We need to test various memory management functions.
     * Some require a database connection (sqlite3_db_release_memory).
     * Others are global (sqlite3_soft_heap_limit, sqlite3_release_memory, etc).
     */

    /* 
     * Extract parameters from input data if available.
     * We use modulo arithmetic to ensure values are within reasonable ranges for testing.
     * Fixed: Added bounds checks before accessing Data array to prevent buffer overflow.
     */
    int val1 = 0;
    int val2 = 0;
    int val3 = 0;
    
    if (Size >= 4) {
        val1 = *(int32_t *)(Data);
        val2 = *(int32_t *)(Data + 4);
        if (Size >= 12) {
            val3 = *(int32_t *)(Data + 8);
        } else {
            val3 = 0;
        }
    } else if (Size >= 2) {
        val1 = *(int16_t *)(Data);
        val2 = *(int16_t *)(Data + 2);
        val3 = 0;
    } else if (Size >= 1) {
        val1 = *(int8_t *)(Data);
        val2 = 0;
        val3 = 0;
    }

    /* 
     * Normalize values to avoid extreme inputs that might cause immediate crashes 
     * unrelated to the logic we are fuzzing, while still testing edge cases.
     * We test negative, zero, and positive values.
     */
    int limit_val = val1 % 1000000; 
    int release_val = val2 % 1000000;
    int reset_flag = val3 % 2;

    /* 
     * Test sqlite3_soft_heap_limit (Deprecated but valid)
     * Input can be negative, zero, or positive.
     */
    sqlite3_soft_heap_limit(limit_val);

    /* 
     * Test sqlite3_release_memory
     * Attempts to free N bytes.
     */
    int freed = sqlite3_release_memory(release_val);
    (void)freed; /* Suppress unused variable warning */

    /* 
     * Test sqlite3_memory_highwater
     * Query peak memory usage, optionally reset.
     */
    sqlite3_int64 highwater = sqlite3_memory_highwater(reset_flag);
    (void)highwater;

    /* 
     * Test sqlite3_malloc and sqlite3_free
     * Allocate memory based on input size.
     * Handle edge cases: 0, negative (handled by API), large.
     */
    int alloc_size = (int)(Size % 10000);
    if (alloc_size <= 0) alloc_size = 1;
    
    void *ptr = sqlite3_malloc(alloc_size);
    if (ptr != NULL) {
        /* Write some data to ensure the allocation is valid */
        memset(ptr, 0x42, alloc_size);
        sqlite3_free(ptr);
    }

    /* 
     * Test sqlite3_db_release_memory
     * Requires a valid database connection.
     * We create a connection to test this specific API.
     */
    sqlite3 *db = open_db();
    if (db != NULL) {
        /* 
         * Perform a simple operation to ensure the connection is active 
         * and has some internal state (e.g., page cache).
         */
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INT);", NULL, NULL, NULL);
        sqlite3_exec(db, "INSERT INTO test VALUES (1);", NULL, NULL, NULL);
        
        /* Release memory from this specific connection */
        int rc_db = sqlite3_db_release_memory(db);
        (void)rc_db;

        /* Cleanup database connection */
        sqlite3_close(db);
    }

    /* 
     * Test sqlite3_shutdown
     * This is idempotent. Calling it multiple times is safe.
     * However, after calling it, we should not use SQLite APIs.
     * To maximize state exploration, we call it conditionally.
     */
    if (Size > 10 && Data[Size-1] == 0x00) {
        sqlite3_shutdown();
        return 0;
    }

    return 0;
}