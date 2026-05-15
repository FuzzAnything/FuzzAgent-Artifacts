// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
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

static int create_dummy_file(const uint8_t *data, size_t size) {
    FILE *fp = fopen("./dummy_file", "wb");
    if (!fp) return 0;
    size_t written = fwrite(data, 1, size, fp);
    fclose(fp);
    return written == size;
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    const char *errmsg;
    const char *db_name;
    unsigned char *serialized = NULL;
    sqlite3_int64 serialized_size = 0;
    
    // Create dummy file for database operations
    if (!create_dummy_file(Data, Size)) {
        return 0;
    }
    
    // Test sqlite3_open with dummy file
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_errmsg
        errmsg = sqlite3_errmsg(db);
        (void)errmsg; // Use to avoid unused variable warning
        
        // Test sqlite3_db_name for main and temp databases
        for (int i = 0; i < 3; i++) {
            db_name = sqlite3_db_name(db, i);
            (void)db_name; // Use to avoid unused variable warning
        }
        
        // Test sqlite3_serialize with various flags and schemas
        const char *schemas[] = {"main", "temp", NULL};
        unsigned int flags[] = {0, SQLITE_SERIALIZE_NOCOPY};
        
        for (int s = 0; s < 3; s++) {
            for (int f = 0; f < 2; f++) {
                serialized = sqlite3_serialize(db, schemas[s], &serialized_size, flags[f]);
                
                if (serialized && serialized_size > 0) {
                    // Test sqlite3_deserialize with the serialized data
                    // Only deserialize if we have a valid buffer and size
                    if (serialized_size >= 512 && (serialized_size % 512) == 0) {
                        // Create a second database connection for deserialization
                        rc = sqlite3_open(":memory:", &db2);
                        if (rc == SQLITE_OK && db2) {
                            // Try deserializing with various flags
                            unsigned int deserialize_flags[] = {
                                0, 
                                SQLITE_DESERIALIZE_FREEONCLOSE,
                                SQLITE_DESERIALIZE_RESIZEABLE,
                                SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE
                            };
                            
                            for (int df = 0; df < 4; df++) {
                                // Only attempt deserialization if we have enough buffer
                                if (serialized_size <= Size) {
                                    unsigned char *copy_buf = sqlite3_malloc64(serialized_size);
                                    if (copy_buf) {
                                        memcpy(copy_buf, serialized, serialized_size);
                                        rc = sqlite3_deserialize(db2, "main", copy_buf, 
                                                                serialized_size, serialized_size, 
                                                                deserialize_flags[df]);
                                        // If flag doesn't include FREEONCLOSE, we need to free
                                        if (!(deserialize_flags[df] & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                                            sqlite3_free(copy_buf);
                                        }
                                    }
                                }
                            }
                            
                            // Test sqlite3_open_v2 with various flags
                            sqlite3 *db3 = NULL;
                            int open_flags[] = {
                                SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
                                SQLITE_OPEN_READONLY,
                                SQLITE_OPEN_READWRITE,
                                SQLITE_OPEN_MEMORY,
                                SQLITE_OPEN_NOMUTEX,
                                SQLITE_OPEN_FULLMUTEX,
                                SQLITE_OPEN_SHAREDCACHE,
                                SQLITE_OPEN_PRIVATECACHE
                            };
                            
                            for (int of = 0; of < 8; of++) {
                                rc = sqlite3_open_v2("./dummy_file", &db3, open_flags[of], NULL);
                                if (db3) {
                                    sqlite3_close(db3);
                                    db3 = NULL;
                                }
                            }
                            
                            sqlite3_close(db2);
                            db2 = NULL;
                        }
                    }
                    
                    // Free serialized buffer if not using NOCOPY flag
                    if (!(flags[f] & SQLITE_SERIALIZE_NOCOPY)) {
                        sqlite3_free(serialized);
                    }
                    serialized = NULL;
                }
            }
        }
        
        sqlite3_close(db);
    }
    
    // Test sqlite3_open with various inputs
    // Use different parts of input for different test cases
    if (Size > 10) {
        // Test with in-memory database
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
        }
        
        // Test with empty string (temporary database)
        rc = sqlite3_open("", &db);
        if (rc == SQLITE_OK && db) {
            sqlite3_close(db);
        }
        
        // Test with URI filename if input suggests it
        if (Size > 20 && Data[0] == 'f' && Data[1] == 'i' && Data[2] == 'l' && Data[3] == 'e' && Data[4] == ':') {
            char *uri = malloc(Size + 1);
            if (uri) {
                memcpy(uri, Data, Size);
                uri[Size] = '\0';
                rc = sqlite3_open_v2(uri, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, NULL);
                if (db) {
                    sqlite3_close(db);
                }
                free(uri);
            }
        }
    }
    
    // Test with invalid database handle for error messages
    errmsg = sqlite3_errmsg(NULL);
    (void)errmsg;
    
    cleanup_dummy_file();
    
    return 0;
}