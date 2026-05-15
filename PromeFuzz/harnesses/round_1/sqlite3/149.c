// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    const char *result = NULL;
    int rc;
    
    // Use first byte to determine which operations to perform
    uint8_t operation = Size > 0 ? Data[0] : 0;
    size_t offset = 1;
    
    // Test sqlite3_errmsg with NULL
    result = sqlite3_errmsg(NULL);
    (void)result;
    
    // Test sqlite3_db_name with NULL
    result = sqlite3_db_name(NULL, 0);
    (void)result;
    
    // Try to open a database
    const char *filename = ":memory:";
    if (operation & 0x01) {
        // Use file-based database
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            if (Size > offset) {
                size_t write_size = Size - offset;
                if (write_size > 1024) write_size = 1024;
                fwrite(Data + offset, 1, write_size, fp);
                offset += write_size;
            }
            fclose(fp);
            filename = "./dummy_file";
        }
    }
    
    // Try sqlite3_open
    rc = sqlite3_open(filename, &db);
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_errmsg with valid db
        result = sqlite3_errmsg(db);
        (void)result;
        
        // Test sqlite3_db_name with various indices
        for (int i = 0; i < 5; i++) {
            result = sqlite3_db_name(db, i);
            (void)result;
        }
        
        // Try sqlite3_open_v2 with different flags
        sqlite3 *db2 = NULL;
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (Size > offset) {
            flags = Data[offset] | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
            offset++;
        }
        
        rc = sqlite3_open_v2(filename, &db2, flags, NULL);
        if (db2) {
            sqlite3_close(db2);
        }
        
        // Try sqlite3_deserialize if we have enough data
        #ifndef SQLITE_OMIT_DESERIALIZE
        if (Size > offset + 16) {
            size_t data_size = Size - offset;
            if (data_size > 4096) data_size = 4096;
            
            // Ensure szDb is a multiple of page size (512 or 4096)
            sqlite3_int64 szDb = (data_size / 512) * 512;
            if (szDb < 512) szDb = 512;
            
            unsigned char *pData = (unsigned char*)malloc(data_size);
            if (pData) {
                memcpy(pData, Data + offset, data_size);
                
                unsigned mFlags = 0;
                if (Size > offset + data_size + 1) {
                    mFlags = Data[offset + data_size] & 0x0F;
                }
                
                // Clear FREEONCLOSE flag since we didn't allocate with sqlite3_malloc
                mFlags &= ~SQLITE_DESERIALIZE_FREEONCLOSE;
                
                rc = sqlite3_deserialize(db, "main", pData, szDb, data_size, mFlags);
                (void)rc;
                
                free(pData);
            }
        }
        #endif
        
        // Try sqlite3_serialize
        #ifndef SQLITE_OMIT_DESERIALIZE
        sqlite3_int64 piSize = 0;
        unsigned int serialize_flags = 0;
        if (Size > offset + 1) {
            serialize_flags = Data[offset] & 0x03;
        }
        
        unsigned char *serialized = sqlite3_serialize(db, "main", &piSize, serialize_flags);
        if (serialized) {
            // Free if not using NOCOPY flag
            if ((serialize_flags & SQLITE_SERIALIZE_NOCOPY) == 0) {
                sqlite3_free(serialized);
            }
        }
        #endif
        
        sqlite3_close(db);
    } else if (db) {
        // Database opened but with error
        result = sqlite3_errmsg(db);
        (void)result;
        sqlite3_close(db);
    }
    
    // Clean up dummy file if created
    if (operation & 0x01) {
        remove("./dummy_file");
    }
    
    return 0;
}