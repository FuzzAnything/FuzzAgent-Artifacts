// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void dummy_file_write(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void dummy_file_cleanup(void) {
    remove("./dummy_file");
}

static void test_db_name(sqlite3 *db) {
    // Test with valid indices
    const char *main_name = sqlite3_db_name(db, 0);
    const char *temp_name = sqlite3_db_name(db, 1);
    
    // Test with out-of-range indices (should return NULL)
    const char *out_of_range = sqlite3_db_name(db, -1);
    out_of_range = sqlite3_db_name(db, 100);
    
    // Copy strings if we need them long-term (though we don't in fuzzing)
    if (main_name) {
        char *copy = strdup(main_name);
        free(copy);
    }
}

static void test_deserialize(sqlite3 *db, const uint8_t *data, size_t size) {
    // Only proceed if we have enough data for basic parameters
    if (size < 512) return;
    
    // Use sqlite3_malloc for buffer when using FREEONCLOSE flag
    unsigned char *buf = sqlite3_malloc(size);
    if (!buf) return;
    
    memcpy(buf, data, size);
    
    // Try different schemas
    const char *schemas[] = {"main", "temp", NULL};
    unsigned flags[] = {0, SQLITE_DESERIALIZE_FREEONCLOSE, SQLITE_DESERIALIZE_RESIZEABLE};
    
    for (int i = 0; i < 2; i++) {  // Skip "temp" as it's not allowed
        for (int j = 0; j < 3; j++) {
            // Calculate sizes - ensure szBuf >= szDb and szDb is multiple of page size
            sqlite3_int64 szDb = (size / 512) * 512;  // Make multiple of 512
            if (szDb < 512) szDb = 512;
            sqlite3_int64 szBuf = szDb;
            
            // Adjust buffer if needed for FREEONCLOSE
            if (flags[j] & SQLITE_DESERIALIZE_FREEONCLOSE) {
                // Buffer must be allocated by sqlite3_malloc
                unsigned char *new_buf = sqlite3_malloc(szBuf);
                if (new_buf) {
                    memcpy(new_buf, buf, szBuf > size ? size : szBuf);
                    sqlite3_free(buf);
                    buf = new_buf;
                }
            }
            
            int rc = sqlite3_deserialize(db, schemas[i], buf, szDb, szBuf, flags[j]);
            
            // If using FREEONCLOSE, SQLite will free the buffer on close or error
            if (!(flags[j] & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                // We still own the buffer
                sqlite3_free(buf);
                // Reallocate for next iteration
                buf = sqlite3_malloc(size);
                if (!buf) break;
                memcpy(buf, data, size);
            } else {
                // SQLite owns the buffer now, allocate new one for next iteration
                buf = sqlite3_malloc(size);
                if (!buf) break;
                memcpy(buf, data, size);
            }
        }
    }
    
    sqlite3_free(buf);
}

static void test_serialize(sqlite3 *db) {
    sqlite3_int64 size = 0;
    unsigned flags[] = {0, SQLITE_SERIALIZE_NOCOPY};
    
    const char *schemas[] = {"main", "temp", NULL};
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            unsigned char *serialized = sqlite3_serialize(db, schemas[i], &size, flags[j]);
            
            if (serialized) {
                // If we didn't use NOCOPY flag, we own the memory
                if (!(flags[j] & SQLITE_SERIALIZE_NOCOPY)) {
                    sqlite3_free(serialized);
                }
                // Otherwise, SQLite owns it and we must not modify/free it
            }
        }
    }
}

int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Write data to dummy file for file-based operations
    dummy_file_write(Data, Size);
    
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    
    // Test sqlite3_open with dummy file
    int rc = sqlite3_open("./dummy_file", &db);
    
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_db_name
        test_db_name(db);
        
        // Test sqlite3_errmsg - get error message if any
        const char *err = sqlite3_errmsg(db);
        
        // Test sqlite3_deserialize with fuzzer data
        test_deserialize(db, Data, Size);
        
        // Test sqlite3_serialize
        test_serialize(db);
        
        // Close the database
        sqlite3_close(db);
    }
    
    // Test sqlite3_open_v2 with various flags
    int flags[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI
    };
    
    for (int i = 0; i < 5; i++) {
        rc = sqlite3_open_v2("./dummy_file", &db2, flags[i], NULL);
        if (rc == SQLITE_OK && db2) {
            test_db_name(db2);
            const char *err2 = sqlite3_errmsg(db2);
            sqlite3_close(db2);
        }
    }
    
    // Test with in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK && db) {
        test_db_name(db);
        test_deserialize(db, Data, Size);
        test_serialize(db);
        sqlite3_close(db);
    }
    
    // Test with empty string (creates temporary on-disk database)
    rc = sqlite3_open("", &db);
    if (rc == SQLITE_OK && db) {
        test_db_name(db);
        sqlite3_close(db);
    }
    
    // Clean up dummy file
    dummy_file_cleanup();
    
    return 0;
}