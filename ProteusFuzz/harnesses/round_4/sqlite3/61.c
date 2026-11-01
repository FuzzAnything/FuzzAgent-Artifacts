// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_serialize, sqlite3_deserialize, and related memory functions.
 * This driver creates a database connection, performs operations to generate state,
 * then attempts to serialize and deserialize the database using various flags and
 * memory management strategies to explore code paths.
 */

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    unsigned char *serialized = NULL;
    sqlite3_int64 size = 0;
    int rc;
    const char *sql_create = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO test (id, data) VALUES (1, ?);";
    sqlite3_stmt *stmt = NULL;

    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Initialize the database schema to ensure there is something to serialize */
    rc = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Insert some data if the input buffer is large enough to avoid empty DB edge cases */
    if (Size > 0) {
        rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
        if (rc == SQLITE_OK && stmt != NULL) {
            sqlite3_bind_blob(stmt, 1, (const void *)Data, Size, SQLITE_STATIC);
            sqlite3_step(stmt);
            sqlite3_finalize(stmt);
        }
    }

    /* 2. Explore program states with sqlite3_serialize */
    
    /* Case A: Standard serialization (allocates memory) */
    serialized = sqlite3_serialize(db, "main", &size, 0);
    if (serialized != NULL) {
        /* Perform some operations on the serialized data to trigger internal logic */
        /* We don't modify it here as it's a copy, but we verify it exists */
        if (size > 0) {
            /* Attempt to deserialize into a new connection or same connection */
            /* We use the same connection to test the reopen logic */
            unsigned char *new_buf = sqlite3_malloc64(size + 100);
            if (new_buf != NULL) {
                memcpy(new_buf, serialized, size);
                /* Add some padding to test buffer size handling */
                memset(new_buf + size, 0, 100);
                
                /* Try to deserialize with FREEONCLOSE */
                rc = sqlite3_deserialize(db, "main", new_buf, size, size + 100, SQLITE_DESERIALIZE_FREEONCLOSE);
                /* Note: If this fails, the buffer is freed automatically if FREEONCLOSE is set */
                
                /* If it succeeded, we need to serialize again to get a fresh pointer for cleanup */
                if (rc == SQLITE_OK) {
                    unsigned char *temp_ser = sqlite3_serialize(db, "main", NULL, 0);
                    if (temp_ser != NULL) {
                        sqlite3_free(temp_ser);
                    }
                }
            }
        }
        sqlite3_free(serialized);
        serialized = NULL;
    }

    /* Case B: Serialize with NOCOPY (returns internal buffer or NULL) */
    serialized = sqlite3_serialize(db, "main", &size, SQLITE_SERIALIZE_NOCOPY);
    /* Do not free this if it is not NULL, as it points to internal memory */
    if (serialized != NULL) {
        /* Just verify access, do not modify */
        (void)serialized[0];
    }

    /* Case C: Serialize to NULL schema (main) */
    serialized = sqlite3_serialize(db, NULL, &size, 0);
    if (serialized != NULL) {
        sqlite3_free(serialized);
        serialized = NULL;
    }

    /* Case D: Test memory allocation functions directly with fuzz data */
    if (Size > 0) {
        sqlite3_uint64 alloc_size = (Size < 1000) ? Size : 1000;
        void *ptr = sqlite3_malloc64(alloc_size);
        if (ptr != NULL) {
            /* Write fuzz data */
            memcpy(ptr, Data, alloc_size);
            
            /* Test realloc */
            void *ptr2 = sqlite3_realloc64(ptr, alloc_size * 2);
            if (ptr2 != NULL) {
                /* Write more data if possible */
                if (Size > alloc_size) {
                    memcpy((char*)ptr2 + alloc_size, Data, alloc_size);
                }
                sqlite3_free(ptr2);
            } else {
                /* If realloc failed, original ptr is still valid */
                sqlite3_free(ptr);
            }
        }
    }

    /* 3. Cleanup */
    sqlite3_close(db);

    return 0;
}