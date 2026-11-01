// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:31408:18 in sqlite3.h
// sqlite3_realloc64 at sqlite3.c:31656:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:55190:27 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:55279:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    unsigned char *serialized = NULL;
    sqlite3_int64 szDb = 0;
    sqlite3_int64 szBuf = 0;

    // Initialize database connection in memory
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Create a simple table to ensure the database has content
    const char *sql = "CREATE TABLE test (id INTEGER PRIMARY KEY, data BLOB);";
    sqlite3_exec(db, sql, NULL, NULL, NULL);

    // Use input data to create a buffer for potential deserialization
    // We limit the size to prevent excessive memory usage during fuzzing
    size_t max_size = 1024 * 1024; // 1MB limit
    size_t use_size = (Size > max_size) ? max_size : Size;

    // Allocate memory using sqlite3_malloc64 to test that API
    void *malloced_mem = sqlite3_malloc64(use_size);
    if (malloced_mem) {
        if (use_size > 0) {
            memcpy(malloced_mem, Data, use_size);
        }
        
        // Try reallocating to test sqlite3_realloc64
        size_t new_size = use_size + 100;
        void *reallocated_mem = sqlite3_realloc64(malloced_mem, new_size);
        if (reallocated_mem) {
            sqlite3_free(reallocated_mem);
        } else {
            sqlite3_free(malloced_mem);
        }
    }

    // Test sqlite3_serialize with different flags
    // Determine flags based on input data to explore different states
    unsigned int serialize_flags = 0;
    if (Size > 0) {
        if (Data[0] & 0x01) {
            serialize_flags = SQLITE_SERIALIZE_NOCOPY;
        }
    }

    serialized = sqlite3_serialize(db, NULL, &szDb, serialize_flags);
    
    if (serialized) {
        szBuf = szDb;
        
        // Ensure we are not in a transaction before deserializing
        sqlite3_exec(db, "ROLLBACK;", NULL, NULL, NULL);

        // Determine deserialize flags
        unsigned int deser_flags = 0;
        if (Size > 1) {
            if (Data[1] & 0x01) {
                deser_flags = SQLITE_DESERIALIZE_FREEONCLOSE;
            }
        }

        // Safety check: If we used NOCOPY in serialize, we cannot use FREEONCLOSE in deserialize
        // because SQLite would try to free internal memory.
        if (serialize_flags & SQLITE_SERIALIZE_NOCOPY) {
            deser_flags &= ~SQLITE_DESERIALIZE_FREEONCLOSE;
        }

        rc = sqlite3_deserialize(db, NULL, serialized, szDb, szBuf, deser_flags);

        // Free the serialized buffer if we own it and didn't pass ownership to SQLite
        if (!(serialize_flags & SQLITE_SERIALIZE_NOCOPY)) {
            if (!(deser_flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                sqlite3_free(serialized);
                serialized = NULL;
            }
            // If FREEONCLOSE was set, SQLite will free it on close, so we don't free here.
        }
        // If NOCOPY was set, we never free it manually.
    } else {
        // If serialize returned NULL (e.g., NOCOPY failed), try standard serialization
        // to get data for deserialize test
        serialized = sqlite3_serialize(db, NULL, &szDb, 0);
        if (serialized) {
            szBuf = szDb;
            unsigned int deser_flags = 0;
            if (Size > 1 && (Data[1] & 0x01)) {
                deser_flags = SQLITE_DESERIALIZE_FREEONCLOSE;
            }
            rc = sqlite3_deserialize(db, NULL, serialized, szDb, szBuf, deser_flags);
            
            if (!(deser_flags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                sqlite3_free(serialized);
                serialized = NULL;
            }
        }
    }

    // Test sqlite3_malloc and sqlite3_free directly with input derived sizes
    if (Size > 0) {
        int alloc_size = Data[0];
        // Ensure alloc_size is reasonable
        if (alloc_size > 0 && alloc_size < 10000) {
            void *p = sqlite3_malloc(alloc_size);
            if (p) {
                // Write some data to ensure memory is valid
                memset(p, 0xAA, alloc_size);
                sqlite3_free(p);
            }
        }
    }

    // Cleanup
    sqlite3_close(db);

    return 0;
}