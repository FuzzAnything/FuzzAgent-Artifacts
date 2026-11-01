// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite memory management and serialization APIs.
 * Targets: sqlite3_malloc64, sqlite3_serialize, sqlite3_deserialize,
 *          sqlite3_realloc64, sqlite3_malloc, sqlite3_free.
 */

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 size = 0;
    int rc;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /* Initialize database connection in memory */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Create a simple table to ensure there is data to serialize */
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO test (data) VALUES (X'0102030405');";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    sqlite3_exec(db, insert_sql, NULL, NULL, NULL);

    /*
     * Scenario 1: Test sqlite3_malloc64 and sqlite3_free
     * Use input data to determine allocation size, clamping to reasonable limits
     * to avoid immediate OOM or excessive resource usage.
     */
    if (Size >= 8) {
        sqlite3_uint64 alloc_size = 0;
        memcpy(&alloc_size, Data, 8);
        /* Cap allocation size to prevent excessive memory usage during fuzzing */
        if (alloc_size > 1024 * 1024) {
            alloc_size = 1024 * 1024;
        }
        void *ptr = sqlite3_malloc64(alloc_size);
        if (ptr != NULL) {
            /* Write some data to the allocated buffer to exercise memory usage */
            memset(ptr, 0xAB, alloc_size > Size ? Size : alloc_size);
            sqlite3_free(ptr);
            ptr = NULL; /* Prevent double-free */
        }
    }

    /*
     * Scenario 2: Test sqlite3_malloc and sqlite3_free
     * Similar logic but using 32-bit size.
     */
    if (Size >= 4) {
        int alloc_size_32 = 0;
        memcpy(&alloc_size_32, Data, 4);
        if (alloc_size_32 <= 0) alloc_size_32 = 1;
        if (alloc_size_32 > 1024 * 1024) alloc_size_32 = 1024 * 1024;
        
        void *ptr_32 = sqlite3_malloc(alloc_size_32);
        if (ptr_32 != NULL) {
            memset(ptr_32, 0xCD, alloc_size_32 > Size ? Size : alloc_size_32);
            sqlite3_free(ptr_32);
            ptr_32 = NULL;
        }
    }

    /*
     * Scenario 3: Test sqlite3_realloc64
     * Allocate initial buffer, then attempt to resize based on input.
     */
    if (Size >= 8) {
        sqlite3_uint64 initial_size = 64;
        sqlite3_uint64 new_size = 0;
        memcpy(&new_size, Data, 8);
        if (new_size > 1024 * 1024) new_size = 1024 * 1024;
        
        void *ptr_realloc = sqlite3_malloc64(initial_size);
        if (ptr_realloc != NULL) {
            void *ptr_new = sqlite3_realloc64(ptr_realloc, new_size);
            if (ptr_new != NULL) {
                memset(ptr_new, 0xEF, new_size > Size ? Size : new_size);
                sqlite3_free(ptr_new);
                ptr_new = NULL;
            } else {
                /* If realloc fails, the original pointer is still valid */
                sqlite3_free(ptr_realloc);
                ptr_realloc = NULL;
            }
        }
    }

    /*
     * Scenario 4: Test sqlite3_serialize
     * Serialize the main database schema.
     */
    serialized = sqlite3_serialize(db, NULL, &size, 0);
    
    /* 
     * Perform deserialization tests on a separate connection BEFORE freeing serialized.
     * The serialized buffer must remain valid until the connection using it is closed.
     */
    if (serialized != NULL) {
        sqlite3 *db2 = NULL;
        if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
            sqlite3_int64 szDb = size;
            sqlite3_int64 szBuf = size;
            
            /* Vary buffer size based on input to test resizing logic */
            if (Size >= 8) {
                sqlite3_uint64 extra_space = 0;
                memcpy(&extra_space, Data, 8);
                if (extra_space > 1024 * 1024) extra_space = 1024 * 1024;
                szBuf += extra_space;
            }

            /* Vary flags based on input */
            unsigned int flags = 0;
            if (Size >= 1) {
                flags = Data[0] & 0x0F; /* Use lower 4 bits as flags */
            }

            /* Attempt deserialization */
            rc = sqlite3_deserialize(db2, NULL, serialized, szDb, szBuf, flags);
            (void)rc; /* Ignore return code for fuzzing, just ensure no crash */
            
            /* Clean up db2. This may internally free the buffer if flags included FREEONCLOSE,
               but we don't know that here. We must NOT free serialized ourselves if we used
               SQLITE_DESERIALIZE_FREEONCLOSE. However, we didn't set that flag in 'flags'
               (we masked it to 0x0F, and FREEONCLOSE is 0x01, so it might be set).
               
               Correction: If we pass SQLITE_DESERIALIZE_FREEONCLOSE (0x01) in flags,
               sqlite3_deserialize will take ownership of the buffer.
               If we do NOT pass it, we are responsible for freeing it.
               
               Let's check if we passed the flag.
            */
            if (flags & SQLITE_DESERIALIZE_FREEONCLOSE) {
                /* sqlite3_deserialize took ownership. Do NOT free here. */
                /* Just close db2. */
                sqlite3_close(db2);
            } else {
                /* We own the buffer. Close db2 first, then free serialized. */
                sqlite3_close(db2);
                sqlite3_free(serialized);
                serialized = NULL;
            }
        }
    }

    /*
     * Scenario 5: Test sqlite3_serialize with SQLITE_SERIALIZE_NOCOPY
     * This requires a prior deserialize to work, so we try it on the original db
     * which might not have a contiguous buffer, expecting NULL or valid pointer.
     */
    unsigned char *nocopy_buf = sqlite3_serialize(db, NULL, NULL, SQLITE_SERIALIZE_NOCOPY);
    /* Do not free nocopy_buf if it returns a pointer, as it points to internal memory */
    (void)nocopy_buf;

    /* Clean up the main database connection */
    sqlite3_close(db);

    return 0;
}