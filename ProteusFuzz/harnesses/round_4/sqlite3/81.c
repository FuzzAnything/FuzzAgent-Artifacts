// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_soft_heap_limit at sqlite3.c:31185:17 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:31273:26 in sqlite3.h
// sqlite3_release_memory at sqlite3.c:31088:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_db_release_memory at sqlite3.c:186530:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    if (Size == 0) {
        return 0;
    }

    // Initialize SQLite
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    // 1. Test sqlite3_soft_heap_limit
    int limit = 0;
    if (Size >= 4) {
        limit = *(int32_t *)(Data);
    } else if (Size > 0) {
        limit = (int32_t)Data[0];
    }
    sqlite3_soft_heap_limit(limit);

    // 2. Test sqlite3_memory_highwater
    sqlite3_int64 hw1 = sqlite3_memory_highwater(0);
    sqlite3_int64 hw2 = sqlite3_memory_highwater(1);
    (void)hw1;
    (void)hw2;

    // 3. Test sqlite3_release_memory
    int release_amount = 0;
    if (Size >= 8) {
        release_amount = *(int32_t *)(Data + 4);
    } else if (Size > 4) {
        release_amount = (int32_t)Data[4];
    }
    int freed = sqlite3_release_memory(release_amount);
    (void)freed;

    // 4. Create a database connection
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    // 5. Test sqlite3_db_release_memory
    int db_freed = sqlite3_db_release_memory(db);
    (void)db_freed;

    // 6. Test sqlite3_malloc
    int alloc_size = 0;
    if (Size >= 12) {
        alloc_size = *(int32_t *)(Data + 8);
    } else if (Size > 8) {
        alloc_size = (int32_t)Data[8];
    }
    
    void *ptr = sqlite3_malloc(alloc_size);
    if (ptr != NULL) {
        size_t write_size = (alloc_size > 0) ? (size_t)alloc_size : 0;
        if (Size > 0) {
            size_t copy_len = (Size < write_size) ? Size : write_size;
            memcpy(ptr, Data, copy_len);
        }
        sqlite3_free(ptr);
    }

    // Cleanup database connection BEFORE shutting down the library
    if (db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }

    // Test sqlite3_shutdown
    rc = sqlite3_shutdown();
    (void)rc;

    // Re-initialize for next iteration
    sqlite3_initialize();

    return 0;
}