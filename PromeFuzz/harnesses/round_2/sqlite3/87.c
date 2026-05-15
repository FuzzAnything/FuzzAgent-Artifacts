// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (fp) {
        fwrite(data, 1, size, fp);
        fclose(fp);
    }
}

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_87(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    const char *errmsg;
    const char *db_name;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    
    /* Use first byte to control behavior */
    uint8_t control = Data[0];
    const uint8_t *remaining_data = Data + 1;
    size_t remaining_size = Size - 1;
    
    /* Write data to dummy file for file-based operations */
    write_dummy_file(remaining_data, remaining_size);
    
    /* Test sqlite3_open */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db != NULL) {
        /* Test sqlite3_errmsg */
        errmsg = sqlite3_errmsg(db);
        (void)errmsg; /* Use result to avoid unused variable warning */
        
        /* Test sqlite3_db_name */
        for (int i = 0; i < 3; i++) {
            db_name = sqlite3_db_name(db, i);
            (void)db_name;
        }
        
        /* Test sqlite3_serialize with various schemas and flags */
        const char *schemas[] = {"main", "temp", NULL};
        unsigned int flags[] = {0, SQLITE_SERIALIZE_NOCOPY};
        
        for (int s = 0; s < 3; s++) {
            for (int f = 0; f < 2; f++) {
                serialized = sqlite3_serialize(db, schemas[s], &serialized_size, flags[f]);
                if (serialized != NULL && !(flags[f] & SQLITE_SERIALIZE_NOCOPY)) {
                    sqlite3_free(serialized);
                    serialized = NULL;
                }
            }
        }
        
        /* Test sqlite3_deserialize if we have enough data */
        if (remaining_size >= 512) { /* Minimum reasonable size for a database */
            unsigned int deserialize_flags[] = {0, SQLITE_DESERIALIZE_FREEONCLOSE, 
                                               SQLITE_DESERIALIZE_RESIZEABLE};
            
            /* Create a copy of the data for deserialization */
            unsigned char *deserialize_data = (unsigned char*)sqlite3_malloc64(remaining_size);
            if (deserialize_data) {
                memcpy(deserialize_data, remaining_data, remaining_size);
                
                /* Try to open an in-memory database for deserialization */
                rc = sqlite3_open(":memory:", &db2);
                if (rc == SQLITE_OK && db2 != NULL) {
                    for (int f = 0; f < 3; f++) {
                        /* Only use main schema (temp is not allowed for deserialize) */
                        rc = sqlite3_deserialize(db2, "main", deserialize_data, 
                                                remaining_size, remaining_size, 
                                                deserialize_flags[f]);
                        (void)rc;
                        
                        /* If FREEONCLOSE flag is set, SQLite will free the buffer */
                        if (deserialize_flags[f] & SQLITE_DESERIALIZE_FREEONCLOSE) {
                            /* Don't free it ourselves - SQLite will do it */
                            deserialize_data = NULL;
                            break;
                        }
                    }
                    sqlite3_close(db2);
                }
                
                if (deserialize_data) {
                    sqlite3_free(deserialize_data);
                }
            }
        }
        
        sqlite3_close(db);
    } else {
        /* Even if open failed, db might be set with error information */
        if (db) {
            errmsg = sqlite3_errmsg(db);
            (void)errmsg;
            sqlite3_close(db);
        }
    }
    
    /* Test sqlite3_open_v2 with various flags */
    int open_flags[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_NOMUTEX,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_FULLMUTEX
    };
    
    for (int i = 0; i < 6; i++) {
        rc = sqlite3_open_v2("./dummy_file", &db, open_flags[i], NULL);
        if (rc == SQLITE_OK && db != NULL) {
            /* Quick test of other functions with this connection */
            errmsg = sqlite3_errmsg(db);
            (void)errmsg;
            
            /* Test serialize with this connection */
            serialized = sqlite3_serialize(db, "main", &serialized_size, 0);
            if (serialized != NULL) {
                sqlite3_free(serialized);
            }
            
            sqlite3_close(db);
        } else if (db) {
            sqlite3_close(db);
        }
    }
    
    /* Clean up */
    remove_dummy_file();
    
    return 0;
}