// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc64 at sqlite3.c:17401:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
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

static void remove_dummy_file(void) {
    remove("./dummy_file");
}

static unsigned char *allocate_buffer(size_t size) {
    if (size == 0) return NULL;
    return (unsigned char *)sqlite3_malloc64(size);
}

static void free_buffer(unsigned char *buf) {
    if (buf) sqlite3_free(buf);
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    // Write input to dummy file for database operations
    write_dummy_file(Data, Size);
    
    sqlite3 *db = NULL;
    sqlite3 *db2 = NULL;
    int rc;
    sqlite3_int64 serialized_size = 0;
    unsigned char *serialized_data = NULL;
    const char *db_name = NULL;
    const char *error_msg = NULL;
    
    // Test sqlite3_open with dummy file
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK) {
        // Clean up and try other operations
        if (db) sqlite3_close(db);
        db = NULL;
        
        // Try opening in-memory database
        rc = sqlite3_open(":memory:", &db);
        if (rc != SQLITE_OK) {
            if (db) sqlite3_close(db);
            remove_dummy_file();
            return 0;
        }
    }
    
    // Test sqlite3_db_name with various indices
    for (int i = 0; i < 3; i++) {
        db_name = sqlite3_db_name(db, i);
        // db_name may be NULL if index out of range
    }
    
    // Test sqlite3_errmsg
    error_msg = sqlite3_errmsg(db);
    
    // Test sqlite3_serialize with different schemas and flags
    const char *schemas[] = {"main", "temp", NULL};
    unsigned int flags[] = {0, SQLITE_SERIALIZE_NOCOPY};
    
    for (size_t s = 0; s < sizeof(schemas)/sizeof(schemas[0]); s++) {
        for (size_t f = 0; f < sizeof(flags)/sizeof(flags[0]); f++) {
            serialized_data = sqlite3_serialize(db, schemas[s], &serialized_size, flags[f]);
            
            if (serialized_data) {
                // If we got serialized data, test sqlite3_deserialize
                if (serialized_size > 0) {
                    // Create another database connection for deserialization
                    rc = sqlite3_open(":memory:", &db2);
                    if (rc == SQLITE_OK && db2) {
                        unsigned int deserialize_flags[] = {0, SQLITE_DESERIALIZE_FREEONCLOSE, 
                                                           SQLITE_DESERIALIZE_RESIZEABLE,
                                                           SQLITE_DESERIALIZE_FREEONCLOSE | SQLITE_DESERIALIZE_RESIZEABLE};
                        
                        for (size_t df = 0; df < sizeof(deserialize_flags)/sizeof(deserialize_flags[0]); df++) {
                            // For FREEONCLOSE flag, we need to make a copy since sqlite3_serialize
                            // returns memory we might need to free differently
                            unsigned char *copy_buf = NULL;
                            sqlite3_int64 buf_size = serialized_size;
                            
                            if (deserialize_flags[df] & SQLITE_DESERIALIZE_FREEONCLOSE) {
                                copy_buf = allocate_buffer(serialized_size);
                                if (copy_buf && serialized_data) {
                                    memcpy(copy_buf, serialized_data, serialized_size);
                                } else {
                                    continue;
                                }
                            } else {
                                copy_buf = serialized_data;
                                buf_size = serialized_size;
                            }
                            
                            rc = sqlite3_deserialize(db2, "main", copy_buf, serialized_size, 
                                                    buf_size, deserialize_flags[df]);
                            
                            // If we didn't use FREEONCLOSE, we still own the buffer
                            if (!(deserialize_flags[df] & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                                // Don't free serialized_data here - it will be freed later
                            }
                            
                            // Test error message after deserialize
                            error_msg = sqlite3_errmsg(db2);
                        }
                        
                        if (db2) sqlite3_close(db2);
                        db2 = NULL;
                    }
                }
                
                // Free the serialized data if we need to
                if (!(flags[f] & SQLITE_SERIALIZE_NOCOPY)) {
                    free_buffer(serialized_data);
                }
                serialized_data = NULL;
            }
        }
    }
    
    // Test sqlite3_open_v2 with various flags
    int open_flags[] = {
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE,
        SQLITE_OPEN_READONLY,
        SQLITE_OPEN_READWRITE,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI
    };
    
    for (size_t i = 0; i < sizeof(open_flags)/sizeof(open_flags[0]); i++) {
        rc = sqlite3_open_v2("./dummy_file", &db2, open_flags[i], NULL);
        if (rc == SQLITE_OK && db2) {
            // Test a simple operation
            db_name = sqlite3_db_name(db2, 0);
            error_msg = sqlite3_errmsg(db2);
            sqlite3_close(db2);
            db2 = NULL;
        }
    }
    
    // Test with URI
    rc = sqlite3_open_v2("file:./dummy_file?mode=ro", &db2, 
                        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_URI, 
                        NULL);
    if (rc == SQLITE_OK && db2) {
        sqlite3_close(db2);
        db2 = NULL;
    }
    
    // Clean up
    if (db) sqlite3_close(db);
    remove_dummy_file();
    
    return 0;
}