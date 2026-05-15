// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    // Initialize database connection
    sqlite3 *db = NULL;
    int rc;
    
    // Try opening database with different flags
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        // If opening fails, try with a file
        sqlite3_close(db);
        db = NULL;
        rc = sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
        if (rc != SQLITE_OK) {
            return 0;
        }
    }
    
    // Test sqlite3_db_name for main and temp databases
    const char *main_name = sqlite3_db_name(db, 0);
    const char *temp_name = sqlite3_db_name(db, 1);
    (void)main_name;  // Use to avoid unused variable warning
    (void)temp_name;
    
    // Test sqlite3_errmsg (should be "not an error" initially)
    const char *err_msg = sqlite3_errmsg(db);
    (void)err_msg;
    
    // Prepare data for deserialize if we have enough input
    if (Size >= 512) {  // Minimum page size
        // Extract parameters from fuzzer input
        size_t page_size = 512;  // Common SQLite page size
        if (Size >= 4096 && (Data[0] & 1)) {
            page_size = 4096;  // Try larger page size sometimes
        }
        
        // Calculate sizes
        sqlite3_int64 szDb = (Size / page_size) * page_size;
        if (szDb < page_size) szDb = page_size;
        sqlite3_int64 szBuf = szDb;
        
        // Allocate buffer with sqlite3_malloc for FREEONCLOSE flag
        unsigned char *pData = (unsigned char *)sqlite3_malloc(szBuf);
        if (pData) {
            // Copy fuzzer data into buffer
            size_t copy_size = (Size < szDb) ? Size : szDb;
            memcpy(pData, Data, copy_size);
            
            // Try different flags combinations
            unsigned int flags = 0;
            if (Size > 0 && (Data[0] & 2)) {
                flags |= SQLITE_DESERIALIZE_READONLY;
            }
            if (Size > 1 && (Data[1] & 1)) {
                flags |= SQLITE_DESERIALIZE_FREEONCLOSE;
            }
            if (Size > 1 && (Data[1] & 2)) {
                flags |= SQLITE_DESERIALIZE_RESIZEABLE;
            }
            
            // Try deserializing to main database
            rc = sqlite3_deserialize(db, "main", pData, szDb, szBuf, flags);
            
            // If deserialize succeeded, try to serialize it back
            if (rc == SQLITE_OK) {
                sqlite3_int64 serial_size = 0;
                unsigned int serialize_flags = 0;
                if (Size > 2 && (Data[2] & 1)) {
                    serialize_flags |= SQLITE_SERIALIZE_NOCOPY;
                }
                
                unsigned char *serialized = sqlite3_serialize(db, "main", &serial_size, serialize_flags);
                if (serialized && !(serialize_flags & SQLITE_SERIALIZE_NOCOPY)) {
                    // Free the buffer if we got a copy
                    sqlite3_free(serialized);
                }
                
                // Try with temp schema (should fail)
                rc = sqlite3_deserialize(db, "temp", pData, szDb, szBuf, flags);
                
                // Test sqlite3_db_name again after operations
                main_name = sqlite3_db_name(db, 0);
                temp_name = sqlite3_db_name(db, 1);
                (void)main_name;
                (void)temp_name;
            }
            
            // Clean up if FREEONCLOSE wasn't set or deserialize failed
            if (!(flags & SQLITE_DESERIALIZE_FREEONCLOSE) || rc != SQLITE_OK) {
                sqlite3_free(pData);
            }
        }
    }
    
    // Test sqlite3_open_v2 with different flags
    sqlite3 *db2 = NULL;
    int open_flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        if (Data[0] & 4) open_flags |= SQLITE_OPEN_MEMORY;
        if (Data[0] & 8) open_flags |= SQLITE_OPEN_NOMUTEX;
        if (Data[0] & 16) open_flags |= SQLITE_OPEN_FULLMUTEX;
    }
    
    rc = sqlite3_open_v2(":memory:", &db2, open_flags, NULL);
    if (rc == SQLITE_OK) {
        // Test error message on second database
        err_msg = sqlite3_errmsg(db2);
        (void)err_msg;
        
        // Test db_name on second database
        main_name = sqlite3_db_name(db2, 0);
        (void)main_name;
        
        sqlite3_close(db2);
    }
    
    // Final error message check
    err_msg = sqlite3_errmsg(db);
    (void)err_msg;
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}