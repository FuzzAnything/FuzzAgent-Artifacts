// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:174663:16 in sqlite3.h
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

/* Dummy comparison function for collations */
static int dummy_compare(void *pArg, int len1, const void *str1, 
                         int len2, const void *str2) {
    /* Simple byte-wise comparison */
    int min_len = len1 < len2 ? len1 : len2;
    int cmp = memcmp(str1, str2, min_len);
    if (cmp == 0) {
        return len1 - len2;
    }
    return cmp;
}

/* Dummy destroy function for collations */
static void dummy_destroy(void *pArg) {
    /* Nothing to do */
}

/* Callback for sqlite3_collation_needed */
static void collation_needed_callback(void *pArg, sqlite3 *db, 
                                      int eTextRep, const char *zName) {
    /* Register a dummy collation when needed */
    sqlite3_create_collation_v2(db, zName, eTextRep, NULL, 
                               dummy_compare, dummy_destroy);
}

/* Callback for sqlite3_collation_needed16 */
static void collation_needed16_callback(void *pArg, sqlite3 *db, 
                                        int eTextRep, const void *zName) {
    /* For UTF-16, convert to UTF-8 if possible, then register */
    if (zName) {
        /* Simple approach: treat as ASCII by taking every other byte */
        const unsigned char *name16 = (const unsigned char *)zName;
        char name8[256];
        int i, j;
        for (i = 0, j = 0; i < 255 && name16[i] != 0 && name16[i+1] != 0; i += 2, j++) {
            name8[j] = name16[i]; /* Take low byte (little-endian assumption) */
        }
        name8[j] = '\0';
        sqlite3_create_collation_v2(db, name8, eTextRep, NULL, 
                                   dummy_compare, dummy_destroy);
    }
}

/* Dummy sqlite3_index_info structure for testing */
static struct sqlite3_index_info dummy_index_info;

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *err_msg = NULL;
    
    /* Initialize dummy index info structure */
    memset(&dummy_index_info, 0, sizeof(dummy_index_info));
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use fuzzer input to generate collation name and parameters */
    if (Size > 0) {
        /* Extract parameters from fuzzer input */
        int eTextRep = Data[0] % 6; /* 0-5 for different encodings */
        const char *zName = (Size > 1) ? (const char *)(Data + 1) : "dummy";
        
        /* Ensure null-terminated name for UTF-8 functions */
        char name_buf[256];
        size_t name_len = (Size - 1) < 255 ? (Size - 1) : 255;
        if (name_len > 0) {
            memcpy(name_buf, Data + 1, name_len);
            name_buf[name_len] = '\0';
            zName = name_buf;
        }
        
        /* Test sqlite3_create_collation */
        sqlite3_create_collation(db, zName, eTextRep, NULL, dummy_compare);
        
        /* Test sqlite3_create_collation_v2 */
        sqlite3_create_collation_v2(db, zName, eTextRep, NULL, 
                                   dummy_compare, dummy_destroy);
        
        /* Test sqlite3_create_collation16 */
        if (Size > 2) {
            /* Use the input data as UTF-16 (simplified) */
            sqlite3_create_collation16(db, Data + 1, eTextRep, NULL, dummy_compare);
        }
        
        /* Test sqlite3_collation_needed */
        sqlite3_collation_needed(db, NULL, collation_needed_callback);
        
        /* Test sqlite3_collation_needed16 */
        sqlite3_collation_needed16(db, NULL, collation_needed16_callback);
        
        /* Test sqlite3_vtab_collation */
        if (dummy_index_info.nConstraint > 0) {
            const char *coll_name = sqlite3_vtab_collation(&dummy_index_info, 0);
            (void)coll_name; /* Ignore result */
        }
        
        /* Try to trigger collation needed callback by executing a query */
        if (Size > 10) {
            char query[256];
            snprintf(query, sizeof(query), 
                    "CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT COLLATE %s)", 
                    zName);
            sqlite3_exec(db, query, NULL, NULL, &err_msg);
            if (err_msg) {
                sqlite3_free(err_msg);
                err_msg = NULL;
            }
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}