// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    sqlite3 *db = NULL;
    int rc;

    // Initialize database connection in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Create a simple table to ensure there is something to serialize
    const char *create_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    // Extract parameters from fuzz input
    uint8_t op = Data[0] % 10;
    uint8_t flag = Data[1] % 4;
    sqlite3_uint64 realloc_size = 0;
    if (Size > 4) {
        realloc_size = ((sqlite3_uint64)Data[4] << 24) | 
                       ((sqlite3_uint64)Data[5] << 16) | 
                       ((sqlite3_uint64)Data[6] << 8) | 
                       ((sqlite3_uint64)Data[7]);
    }

    // Fuzz sqlite3_malloc and sqlite3_free
    if (op == 0) {
        int alloc_size = 0;
        if (Size > 8) {
            alloc_size = ((int)Data[8] << 8) | Data[9];
        }
        if (alloc_size <= 0) alloc_size = 100;
        if (alloc_size > 1000000) alloc_size = 1000000;

        void *ptr = sqlite3_malloc(alloc_size);
        if (ptr) {
            memset(ptr, 0, alloc_size);
            sqlite3_free(ptr);
        }
    }

    // Fuzz sqlite3_malloc64 and sqlite3_free
    if (op == 1) {
        if (realloc_size == 0) realloc_size = 100;
        if (realloc_size > 1000000000ULL) realloc_size = 1000000000ULL;

        void *ptr = sqlite3_malloc64(realloc_size);
        if (ptr) {
            memset(ptr, 0, realloc_size);
            sqlite3_free(ptr);
        }
    }

    // Fuzz sqlite3_realloc64
    if (op == 2) {
        void *ptr = NULL;
        if (Size > 10) {
            int init_size = ((int)Data[10] << 8) | Data[11];
            if (init_size <= 0) init_size = 100;
            if (init_size > 1000000) init_size = 1000000;
            ptr = sqlite3_malloc(init_size);
        }
        
        if (realloc_size == 0) realloc_size = 100;
        if (realloc_size > 1000000000ULL) realloc_size = 1000000000ULL;

        void *new_ptr = sqlite3_realloc64(ptr, realloc_size);
        if (new_ptr) {
            sqlite3_free(new_ptr);
        }
    }

    // Fuzz sqlite3_serialize
    if (op == 3 || op == 4) {
        sqlite3_int64 db_size = 0;
        unsigned int flags = 0;
        if (flag & 1) flags |= SQLITE_SERIALIZE_NOCOPY;

        unsigned char *serialized = sqlite3_serialize(db, NULL, &db_size, flags);
        if (serialized) {
            // If not NOCOPY, we must free it
            if (!(flags & SQLITE_SERIALIZE_NOCOPY)) {
                sqlite3_free(serialized);
            }
        }
    }

    // Fuzz sqlite3_deserialize
    if (op == 5 || op == 6) {
        // First serialize to get valid data
        sqlite3_int64 db_size = 0;
        unsigned char *serialized = sqlite3_serialize(db, NULL, &db_size, 0);
        if (serialized && db_size > 0) {
            // Prepare buffer for deserialization
            sqlite3_int64 buf_size = db_size;
            if (Size > 12) {
                buf_size = ((sqlite3_int64)Data[12] << 24) | 
                           ((sqlite3_int64)Data[13] << 16) | 
                           ((sqlite3_int64)Data[14] << 8) | 
                           ((sqlite3_int64)Data[15]);
                if (buf_size < db_size) buf_size = db_size;
                if (buf_size > 1000000000LL) buf_size = 1000000000LL;
            }

            unsigned char *deserialize_buf = sqlite3_malloc((int)buf_size);
            if (deserialize_buf) {
                memcpy(deserialize_buf, serialized, db_size);
                
                unsigned int deserialize_flags = 0;
                if (flag & 2) deserialize_flags |= SQLITE_DESERIALIZE_FREEONCLOSE;
                if (flag & 3) deserialize_flags |= SQLITE_DESERIALIZE_RESIZEABLE;

                // Try to deserialize into a new connection
                sqlite3 *db2 = NULL;
                if (sqlite3_open(":memory:", &db2) == SQLITE_OK) {
                    rc = sqlite3_deserialize(db2, NULL, deserialize_buf, db_size, buf_size, deserialize_flags);
                    sqlite3_close(db2);
                } else {
                    // If we can't create a new db, try the existing one (might fail with SQLITE_BUSY)
                    rc = sqlite3_deserialize(db, NULL, deserialize_buf, db_size, buf_size, deserialize_flags);
                }

                if (!(deserialize_flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                    sqlite3_free(deserialize_buf);
                }
            }
            sqlite3_free(serialized);
        }
    }

    // Cleanup
    sqlite3_close(db);

    return 0;
}