// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
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
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 memory and serialization APIs.
 * Target APIs:
 *  - sqlite3_malloc
 *  - sqlite3_malloc64
 *  - sqlite3_realloc64
 *  - sqlite3_free
 *  - sqlite3_serialize
 *  - sqlite3_deserialize
 */

static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    unsigned char *serialized = NULL;
    unsigned char *deserialized_buf = NULL;
    sqlite3_int64 szDb = 0;
    int rc;

    /* Guard against empty input */
    if (Size == 0) {
        return 0;
    }

    /*
     * Step 1: Prepare environment.
     * Initialize a database connection in memory to use with serialize/deserialize.
     */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /*
     * Create a simple schema to ensure serialize returns non-empty data.
     */
    const char *init_sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, init_sql, NULL, NULL, NULL);

    /*
     * Step 2: Fuzz memory allocation APIs (malloc, malloc64, realloc64, free).
     * We use the fuzz input to drive sizes and patterns.
     */
    if (Size >= 2) {
        int size32 = (Data[0] % 1000) + 1;
        sqlite3_uint64 size64 = ((sqlite3_uint64)Data[1] << 16) | (Size > 2 ? Data[2] : 0);
        if (size64 == 0) size64 = 1;
        if (size64 > 1024 * 1024) size64 = 1024 * 1024; /* Cap size for safety */

        void *ptr32 = sqlite3_malloc(size32);
        if (ptr32) {
            memset(ptr32, Data[0] % 256, size32);
            sqlite3_free(ptr32);
        }

        void *ptr64 = sqlite3_malloc64(size64);
        if (ptr64) {
            memset(ptr64, Data[1] % 256, size64);
            sqlite3_free(ptr64);
        }

        /* Test realloc64 */
        void *ptrRealloc = sqlite3_malloc64(size64);
        if (ptrRealloc) {
            sqlite3_uint64 newSize = size64 + (Size > 3 ? Data[3] : 0);
            newSize = newSize % 1000;
            if (newSize > 1024 * 1024) newSize = 1024 * 1024;
            void *ptrNew = sqlite3_realloc64(ptrRealloc, newSize);
            if (ptrNew) {
                memset(ptrNew, (Size > 4 ? Data[4] : 0) % 256, newSize);
                sqlite3_free(ptrNew);
            } else {
                sqlite3_free(ptrRealloc);
            }
        }

        /* Test realloc64 with NULL (acts like malloc) */
        void *ptrNull = sqlite3_realloc64(NULL, size64);
        if (ptrNull) {
            memset(ptrNull, (Size > 5 ? Data[5] : 0) % 256, size64);
            sqlite3_free(ptrNull);
        }

        /* Test realloc64 with zero size (acts like free) */
        void *ptrZero = sqlite3_malloc64(size64);
        if (ptrZero) {
            void *ptrZeroRes = sqlite3_realloc64(ptrZero, 0);
            (void)ptrZeroRes; /* Should be NULL, original memory freed */
        }
    }

    /*
     * Step 3: Fuzz sqlite3_serialize.
     * Serialize the "main" database.
     */
    serialized = sqlite3_serialize(db, "main", &szDb, 0);
    if (serialized && szDb > 0) {
        /*
         * Use the serialized data to drive further fuzzing or just verify it.
         * We can also try to deserialize it back.
         */
        if (Size >= 2) {
            unsigned int flags = Data[0] & 0x03; /* Use first byte for flags */
            int rcDeser;
            sqlite3_int64 bufSize = szDb + (Data[1] % 1000);
            if (bufSize < szDb) bufSize = szDb;

            /* Allocate buffer for deserialization */
            deserialized_buf = sqlite3_malloc64(bufSize);
            if (deserialized_buf) {
                memcpy(deserialized_buf, serialized, szDb);
                memset(deserialized_buf + szDb, 0, bufSize - szDb);

                /* Try to deserialize into a new in-memory DB */
                sqlite3 *db2 = NULL;
                rc = sqlite3_open(":memory:", &db2);
                if (rc == SQLITE_OK && db2) {
                    rcDeser = sqlite3_deserialize(db2, "main", deserialized_buf, szDb, bufSize, flags);
                    (void)rcDeser; /* Ignore result, just exercise the path */
                    
                    /* 
                     * CRITICAL FIX: If SQLITE_DESERIALIZE_FREEONCLOSE is set, 
                     * SQLite takes ownership of the buffer. We must NOT free it here.
                     * If it is not set, we must free it.
                     */
                    if (!(flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                        sqlite3_free(deserialized_buf);
                        deserialized_buf = NULL;
                    } else {
                        /* SQLite will free deserialized_buf when db2 is closed */
                        deserialized_buf = NULL; 
                    }
                    
                    cleanup_db(db2);
                } else {
                    /* If db2 failed to open, we still own the buffer */
                    sqlite3_free(deserialized_buf);
                    deserialized_buf = NULL;
                }
            }
        }

        sqlite3_free(serialized);
        serialized = NULL;
    }

    /*
     * Step 4: Fuzz sqlite3_serialize with NOCOPY flag.
     */
    if (Size >= 2) {
        unsigned int flags = (Data[0] & 0x01) ? SQLITE_SERIALIZE_NOCOPY : 0;
        unsigned char *nocopy = sqlite3_serialize(db, "main", &szDb, flags);
        if (nocopy) {
            /* With NOCOPY, we must not free the buffer unless we allocated it.
             * Since we didn't allocate it, we just check it and move on.
             * Note: The buffer is valid until the next write or close.
             */
            (void)nocopy;
            /* Do not free nocopy here as it is managed by SQLite */
        }
    }

    /*
     * Step 5: Fuzz sqlite3_deserialize with various flags.
     * We create a new DB and try to deserialize into it.
     */
    if (Size >= 4) {
        sqlite3 *db3 = NULL;
        rc = sqlite3_open(":memory:", &db3);
        if (rc == SQLITE_OK && db3) {
            /* Get a fresh serialization to use as source */
            unsigned char *src = sqlite3_serialize(db, "main", &szDb, 0);
            if (src && szDb > 0) {
                unsigned int flags = Data[2] & 0x07; /* Use 3 bits for flags */
                sqlite3_int64 bufSize = szDb + (Data[3] % 1000);
                if (bufSize < szDb) bufSize = szDb;

                unsigned char *dst = sqlite3_malloc64(bufSize);
                if (dst) {
                    memcpy(dst, src, szDb);
                    memset(dst + szDb, 0, bufSize - szDb);

                    int rcDeser = sqlite3_deserialize(db3, "main", dst, szDb, bufSize, flags);
                    (void)rcDeser;

                    /* 
                     * CRITICAL FIX: If FREEONCLOSE is set, SQLite owns the buffer.
                     * We must NOT free it here.
                     */
                    if (!(flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                        sqlite3_free(dst);
                    }
                    /* If FREEONCLOSE is set, SQLite will free it on close */
                }
                sqlite3_free(src);
            }
            cleanup_db(db3);
        }
    }

    /*
     * Step 6: Cleanup.
     * Close the main database connection.
     */
    cleanup_db(db);

    return 0;
}