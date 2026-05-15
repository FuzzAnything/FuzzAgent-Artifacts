// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_collation_v2 at sqlite3.c:174638:16 in sqlite3.h
// sqlite3_create_collation16 at sqlite3.c:174663:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:156966:24 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_collation_needed at sqlite3.c:174693:16 in sqlite3.h
// sqlite3_collation_needed16 at sqlite3.c:174714:16 in sqlite3.h
// sqlite3_create_collation at sqlite3.c:174625:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

/* Dummy collation comparison function */
static int dummy_collation(void *pArg, int n1, const void *p1, int n2, const void *p2) {
    (void)pArg;
    (void)n1;
    (void)p1;
    (void)n2;
    (void)p2;
    return 0;
}

/* Dummy destroy function for collation_v2 */
static void dummy_destroy(void *pArg) {
    (void)pArg;
}

/* Dummy callback for collation_needed */
static void dummy_collation_needed(void *pArg, sqlite3 *db, int eTextRep, const char *zName) {
    (void)pArg;
    (void)db;
    (void)eTextRep;
    (void)zName;
}

/* Dummy callback for collation_needed16 */
static void dummy_collation_needed16(void *pArg, sqlite3 *db, int eTextRep, const void *zName) {
    (void)pArg;
    (void)db;
    (void)eTextRep;
    (void)zName;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    void *dummy_arg = (void *)0x12345678;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Use fuzzer input to determine which operations to perform */
    if (Size > 0) {
        uint8_t op = Data[0] % 8;
        const uint8_t *input_data = Data + 1;
        size_t input_size = Size - 1;
        
        /* Extract a string from input data for collation names */
        char collation_name[256] = {0};
        const char *name_ptr = "dummy_collation";
        if (input_size > 0) {
            size_t name_len = input_size < 255 ? input_size : 255;
            memcpy(collation_name, input_data, name_len);
            collation_name[name_len] = '\0';
            name_ptr = collation_name;
        }
        
        /* Extract encoding type from input */
        int eTextRep = SQLITE_UTF8;
        if (input_size > 1) {
            eTextRep = input_data[1] % 5;
            switch (eTextRep) {
                case 0: eTextRep = SQLITE_UTF8; break;
                case 1: eTextRep = SQLITE_UTF16LE; break;
                case 2: eTextRep = SQLITE_UTF16BE; break;
                case 3: eTextRep = SQLITE_UTF16; break;
                case 4: eTextRep = SQLITE_UTF16_ALIGNED; break;
                default: eTextRep = SQLITE_UTF8; break;
            }
        }
        
        switch (op) {
            case 0:
                /* Test sqlite3_collation_needed */
                sqlite3_collation_needed(db, dummy_arg, dummy_collation_needed);
                break;
                
            case 1:
                /* Test sqlite3_collation_needed16 */
                sqlite3_collation_needed16(db, dummy_arg, dummy_collation_needed16);
                break;
                
            case 2:
                /* Test sqlite3_create_collation */
                sqlite3_create_collation(db, name_ptr, eTextRep, dummy_arg, dummy_collation);
                break;
                
            case 3:
                /* Test sqlite3_create_collation_v2 */
                sqlite3_create_collation_v2(db, name_ptr, eTextRep, dummy_arg, 
                                           dummy_collation, dummy_destroy);
                break;
                
            case 4:
                /* Test sqlite3_create_collation16 */
                sqlite3_create_collation16(db, (const void *)name_ptr, eTextRep, 
                                          dummy_arg, dummy_collation);
                break;
                
            case 5:
                /* Test sqlite3_vtab_collation with dummy sqlite3_index_info */
                {
                    sqlite3_index_info dummy_index_info = {0};
                    const char *coll_name = sqlite3_vtab_collation(&dummy_index_info, 0);
                    (void)coll_name; /* Suppress unused variable warning */
                }
                break;
                
            case 6:
                /* Test multiple operations in sequence */
                sqlite3_collation_needed(db, dummy_arg, dummy_collation_needed);
                sqlite3_create_collation(db, name_ptr, eTextRep, dummy_arg, dummy_collation);
                break;
                
            case 7:
                /* Test removing collation by passing NULL xCompare */
                sqlite3_create_collation(db, name_ptr, eTextRep, dummy_arg, NULL);
                break;
        }
        
        /* Try to trigger collation needed callback by using the collation in a query */
        if (input_size > 0 && collation_name[0] != '\0') {
            sqlite3_stmt *stmt = NULL;
            char query[512];
            snprintf(query, sizeof(query), "SELECT 1 COLLATE %s", collation_name);
            sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
            if (stmt) {
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}