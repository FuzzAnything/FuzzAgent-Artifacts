// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_serialize at sqlite3.c:41183:27 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_db_name at sqlite3.c:175830:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:173592:24 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_144(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    sqlite3 *db = NULL;
    const char *result = NULL;
    int rc;
    
    /* Test sqlite3_errmsg with NULL db */
    result = sqlite3_errmsg(NULL);
    
    /* Test sqlite3_open with various inputs */
    unsigned char *filename = malloc(size + 1);
    if (!filename) return 0;
    
    memcpy(filename, data, size);
    filename[size] = '\0';
    
    /* Try opening with regular sqlite3_open */
    rc = sqlite3_open((const char*)filename, &db);
    if (rc == SQLITE_OK) {
        /* Test sqlite3_errmsg with valid db */
        result = sqlite3_errmsg(db);
        
        /* Test sqlite3_db_name with various N values */
        for (int n = -2; n < 5; n++) {
            result = sqlite3_db_name(db, n);
        }
        
        /* Test sqlite3_serialize */
        sqlite3_int64 serial_size = 0;
        unsigned char *serialized = sqlite3_serialize(db, "main", &serial_size, 0);
        if (serialized) {
            /* Test sqlite3_deserialize with the serialized data */
            rc = sqlite3_deserialize(db, "main", serialized, serial_size, serial_size, 0);
            /* Don't free if SQLITE_DESERIALIZE_FREEONCLOSE wasn't used */
            if (!(rc == SQLITE_OK && serialized)) {
                sqlite3_free(serialized);
            }
        }
        
        /* Test sqlite3_serialize with SQLITE_SERIALIZE_NOCOPY */
        serialized = sqlite3_serialize(db, "main", &serial_size, 1); /* SQLITE_SERIALIZE_NOCOPY */
        
        /* Close database */
        sqlite3_close(db);
        db = NULL;
    }
    
    /* Test sqlite3_open_v2 with various flags */
    int flags = data[0] % 8;
    flags |= SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    
    rc = sqlite3_open_v2((const char*)filename, &db, flags, NULL);
    if (rc == SQLITE_OK) {
        /* Test sqlite3_db_name with this db */
        result = sqlite3_db_name(db, 0);
        result = sqlite3_db_name(db, 1);
        
        sqlite3_close(db);
        db = NULL;
    }
    
    /* Test with dummy file */
    write_dummy_file(data, size > 1024 ? 1024 : size);
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK) {
        result = sqlite3_errmsg(db);
        sqlite3_close(db);
    }
    cleanup_dummy_file();
    
    /* Test sqlite3_deserialize with raw data */
    db = NULL;
    rc = sqlite3_open(":memory:", &db);
    if (rc == SQLITE_OK) {
        /* Try to deserialize the fuzzer input as a database */
        rc = sqlite3_deserialize(db, "main", (unsigned char*)data, 
                                 size > 100 ? 100 : size, 
                                 size, 
                                 0);
        
        /* Test with SQLITE_DESERIALIZE_FREEONCLOSE flag */
        unsigned char *copy = sqlite3_malloc(size);
        if (copy) {
            memcpy(copy, data, size);
            rc = sqlite3_deserialize(db, "main", copy, 
                                     size > 100 ? 100 : size, 
                                     size, 
                                     SQLITE_DESERIALIZE_FREEONCLOSE);
            /* If deserialize fails, sqlite3_free is called on copy */
        }
        
        sqlite3_close(db);
    }
    
    free(filename);
    return 0;
}