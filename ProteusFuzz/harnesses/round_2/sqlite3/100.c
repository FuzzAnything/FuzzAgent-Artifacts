// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Helper function to safely extract an integer from the fuzz input.
 */
static int get_int_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(int) > Size) {
        return 0;
    }
    int val;
    memcpy(&val, Data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

/*
 * Helper function to get a size_t from input, clamped to a safe maximum.
 */
static size_t get_size_from_input(const uint8_t *Data, size_t Size, size_t *offset) {
    if (*offset + sizeof(size_t) > Size) {
        return 0;
    }
    size_t val;
    memcpy(&val, Data + *offset, sizeof(size_t));
    *offset += sizeof(size_t);
    /* Clamp to a safe maximum to prevent huge allocations */
    if (val > 1024 * 1024) {
        val = 1024 * 1024;
    }
    return val;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    size_t offset = 0;
    sqlite3 *db = NULL;
    void *ptr = NULL;
    int rc;

    /* Ensure we have enough data for basic operations */
    if (Size < 4) {
        return 0;
    }

    /* 
     * Strategy: 
     * 1. Perform global memory operations first.
     * 2. Open a database connection.
     * 3. Perform connection-specific operations.
     * 4. Close the connection.
     * 5. Call sqlite3_shutdown ONLY at the very end, before the function returns.
     *    This prevents the "deadly signal" caused by accessing global state 
     *    after shutdown (e.g., during sqlite3_close).
     */

    /* 
     * Step 1: Test sqlite3_soft_heap_limit
     */
    int heap_limit = get_int_from_input(Data, Size, &offset);
    sqlite3_soft_heap_limit(heap_limit);

    /* 
     * Step 2: Test sqlite3_memory_highwater
     */
    int reset_flag = get_int_from_input(Data, Size, &offset);
    if (reset_flag > 1) reset_flag = 1;
    sqlite3_memory_highwater(reset_flag);

    /* 
     * Step 3: Test sqlite3_release_memory
     */
    int release_bytes = get_int_from_input(Data, Size, &offset);
    sqlite3_release_memory(release_bytes);

    /* 
     * Step 4: Open a database connection.
     */
    int use_file = (Data[0] & 1);
    const char *zDbName = use_file ? "./dummy_file" : ":memory:";
    
    if (use_file) {
        FILE *f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
        }
    }

    rc = sqlite3_open(zDbName, &db);
    if (rc != SQLITE_OK) {
        /* If open fails, we can still test global functions, but db-specific ones will fail gracefully */
    }

    /* 
     * Step 5: Test sqlite3_malloc and sqlite3_free
     */
    size_t alloc_size = get_size_from_input(Data, Size, &offset);
    
    if (alloc_size > 0) {
        ptr = sqlite3_malloc(alloc_size);
        if (ptr != NULL) {
            memset(ptr, 0xAA, alloc_size);
            
            /* Test sqlite3_db_release_memory if we have a valid db connection */
            if (db != NULL) {
                sqlite3_db_release_memory(db);
            }

            sqlite3_free(ptr);
            ptr = NULL;
        }
    }

    /* 
     * Step 6: Close the database connection.
     * CRITICAL: Do NOT call sqlite3_shutdown before this.
     * sqlite3_close relies on global mutexes and state that are destroyed by shutdown.
     */
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }

    /* Clean up dummy file if created */
    if (use_file) {
        remove("./dummy_file");
    }

    /* 
     * Step 7: Call sqlite3_shutdown as the very last operation.
     * This ensures all resources are cleaned up before the fuzzer exits the function,
     * and no subsequent SQLite API calls (like close) are made after shutdown.
     */
    sqlite3_shutdown();

    return 0;
}