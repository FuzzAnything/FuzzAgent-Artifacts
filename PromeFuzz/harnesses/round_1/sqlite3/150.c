// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

int LLVMFuzzerTestOneInput_150(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    /* Use first byte to determine test path */
    uint8_t test_selector = Data[0];
    size_t data_offset = 1;
    
    /* Prepare dummy file for operations that need a file */
    write_dummy_file(Data, Size);
    
    /* Test sqlite3_errmsg with various database states */
    {
        sqlite3 *db = NULL;
        const char *errmsg = NULL;
        
        /* Test with NULL database handle */
        errmsg = sqlite3_errmsg(NULL);
        
        /* Test with valid but closed database */
        if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
            errmsg = sqlite3_errmsg(db);
            sqlite3_close(db);
            db = NULL;
        }
        
        /* Test with database that had an error */
        if (sqlite3_open("./dummy_file", &db) == SQLITE_OK) {
            /* Force an error by trying to prepare invalid SQL */
            sqlite3_stmt *stmt = NULL;
            sqlite3_prepare_v2(db, "INVALID SQL", -1, &stmt, NULL);
            errmsg = sqlite3_errmsg(db);
            if (stmt) sqlite3_finalize(stmt);
            sqlite3_close(db);
        }
    }
    
    /* Test sqlite3_db_name with various database states */
    {
        sqlite3 *db = NULL;
        const char *dbname = NULL;
        
        /* Test with NULL database handle */
        dbname = sqlite3_db_name(NULL, 0);
        dbname = sqlite3_db_name(NULL, 1);
        dbname = sqlite3_db_name(NULL, 100);
        
        /* Test with valid database */
        if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
            /* Test main database (N=0) */
            dbname = sqlite3_db_name(db, 0);
            
            /* Test temp database (N=1) - may be NULL if not created */
            dbname = sqlite3_db_name(db, 1);
            
            /* Test out of range N values */
            dbname = sqlite3_db_name(db, -1);
            dbname = sqlite3_db_name(db, 100);
            
            /* Use data to generate N value */
            int N = (Size > data_offset) ? (Data[data_offset] % 10) : 0;
            dbname = sqlite3_db_name(db, N);
            
            sqlite3_close(db);
        }
    }
    
    /* Test sqlite3_open with various inputs */
    {
        sqlite3 *db = NULL;
        int rc;
        
        /* Test with memory database */
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK) sqlite3_close(db);
        
        /* Test with empty string (temporary database) */
        rc = sqlite3_open("", &db);
        if (rc == SQLITE_OK) sqlite3_close(db);
        
        /* Test with dummy file */
        rc = sqlite3_open("./dummy_file", &db);
        if (rc == SQLITE_OK) sqlite3_close(db);
        
        /* Test with URI filename if data suggests */
        if (Size > data_offset + 10) {
            char uri[256];
            snprintf(uri, sizeof(uri), "file:./dummy_file?mode=ro&cache=private");
            rc = sqlite3_open(uri, &db);
            if (rc == SQLITE_OK) sqlite3_close(db);
        }
    }
    
    /* Test sqlite3_open_v2 with various flags */
    {
        sqlite3 *db = NULL;
        int rc;
        
        /* Use data to generate flags */
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        if (Size > data_offset) {
            uint8_t flag_byte = Data[data_offset];
            if (flag_byte & 0x01) flags |= SQLITE_OPEN_READONLY;
            if (flag_byte & 0x02) flags |= SQLITE_OPEN_MEMORY;
            if (flag_byte & 0x04) flags |= SQLITE_OPEN_NOMUTEX;
            if (flag_byte & 0x08) flags |= SQLITE_OPEN_FULLMUTEX;
            if (flag_byte & 0x10) flags |= SQLITE_OPEN_URI;
            data_offset++;
        }
        
        /* Test with various combinations */
        rc = sqlite3_open_v2(":memory:", &db, flags, NULL);
        if (rc == SQLITE_OK) sqlite3_close(db);
        
        rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
        if (rc == SQLITE_OK) sqlite3_close(db);
        
        /* Test with empty VFS name */
        rc = sqlite3_open_v2(":memory:", &db, flags, "");
        if (rc == SQLITE_OK) sqlite3_close(db);
    }
    
    /* Test sqlite3_deserialize with various inputs */
    {
        sqlite3 *db = NULL;
        
        if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
            /* Prepare buffer for deserialization */
            size_t buf_size = (Size > data_offset) ? (Size - data_offset) : 0;
            unsigned char *pData = NULL;
            
            if (buf_size > 0) {
                /* Allocate buffer using sqlite3_malloc as required */
                pData = (unsigned char *)sqlite3_malloc64(buf_size);
                if (pData) {
                    /* Copy fuzzing data into buffer */
                    memcpy(pData, Data + data_offset, buf_size);
                    
                    /* Try to make it look like a valid database header */
                    if (buf_size >= 100) {
                        /* Set page size to typical value (512) */
                        pData[16] = 0x00;
                        pData[17] = 0x02;
                        
                        /* Set file format write version to 1 (rollback mode) */
                        pData[18] = 0x01;
                        pData[19] = 0x01;
                    }
                    
                    /* Test deserialize with various parameters */
                    unsigned mFlags = 0;
                    if (test_selector & 0x01) mFlags |= SQLITE_DESERIALIZE_FREEONCLOSE;
                    if (test_selector & 0x02) mFlags |= SQLITE_DESERIALIZE_RESIZEABLE;
                    if (test_selector & 0x04) mFlags |= SQLITE_DESERIALIZE_READONLY;
                    
                    sqlite3_int64 szDb = buf_size;
                    sqlite3_int64 szBuf = buf_size;
                    
                    /* Try with main schema */
                    sqlite3_deserialize(db, "main", pData, szDb, szBuf, mFlags);
                    
                    /* Try with temp schema (should fail) */
                    sqlite3_deserialize(db, "temp", pData, szDb, szBuf, mFlags);
                    
                    /* Try with NULL schema */
                    sqlite3_deserialize(db, NULL, pData, szDb, szBuf, mFlags);
                    
                    /* If FREEONCLOSE not set, free the buffer ourselves */
                    if (!(mFlags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                        sqlite3_free(pData);
                    }
                }
            }
            
            sqlite3_close(db);
        }
    }
    
    /* Test sqlite3_serialize */
    {
        sqlite3 *db = NULL;
        
        if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
            sqlite3_int64 size = 0;
            unsigned char *serialized = NULL;
            
            /* Test with NOCOPY flag */
            unsigned int flags = 0;
            if (test_selector & 0x08) flags |= SQLITE_SERIALIZE_NOCOPY;
            
            /* Serialize main database */
            serialized = sqlite3_serialize(db, "main", &size, flags);
            
            /* Serialize temp database */
            serialized = sqlite3_serialize(db, "temp", &size, flags);
            
            /* Serialize with NULL schema */
            serialized = sqlite3_serialize(db, NULL, &size, flags);
            
            /* If we got a buffer and NOCOPY was not set, it's our responsibility */
            if (serialized && !(flags & SQLITE_SERIALIZE_NOCOPY)) {
                sqlite3_free(serialized);
            }
            
            sqlite3_close(db);
        }
    }
    
    /* Test combination of operations */
    {
        sqlite3 *db = NULL;
        
        if (sqlite3_open_v2(":memory:", &db, 
                           SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
                           NULL) == SQLITE_OK) {
            
            /* Get error message */
            const char *err = sqlite3_errmsg(db);
            
            /* Get database names */
            for (int i = 0; i < 3; i++) {
                const char *name = sqlite3_db_name(db, i);
                (void)name; /* Suppress unused warning */
            }
            
            /* Try to serialize */
            sqlite3_int64 size = 0;
            unsigned char *serialized = sqlite3_serialize(db, "main", &size, 0);
            if (serialized) {
                /* Try to deserialize the serialized data back */
                sqlite3_deserialize(db, "main", serialized, size, size, 
                                   SQLITE_DESERIALIZE_FREEONCLOSE);
            }
            
            sqlite3_close(db);
        }
    }
    
    return 0;
}