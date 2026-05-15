// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_file_control at sqlite3.c:175027:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_test_control at sqlite3.c:175080:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_deserialize at sqlite3.c:41272:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Extract parameters from fuzz input */
    size_t offset = 0;
    
    /* Parameters for sqlite3_limit calls */
    int limit_ids[4];
    int limit_vals[4];
    for (int i = 0; i < 4; i++) {
        if (offset + 8 <= Size) {
            limit_ids[i] = *(int*)(Data + offset);
            limit_vals[i] = *(int*)(Data + offset + 4);
            offset += 8;
        } else {
            limit_ids[i] = i;
            limit_vals[i] = -1;
        }
    }
    
    /* Call sqlite3_limit four times */
    for (int i = 0; i < 4; i++) {
        sqlite3_limit(db, limit_ids[i], limit_vals[i]);
    }
    
    /* Call sqlite3_hard_heap_limit64 */
    sqlite3_int64 heap_limit = -1;
    if (offset + 8 <= Size) {
        heap_limit = *(sqlite3_int64*)(Data + offset);
        offset += 8;
    }
    sqlite3_hard_heap_limit64(heap_limit);
    
    /* Call sqlite3_test_control */
    int test_op = 0;
    int test_arg = 0;
    if (offset + 8 <= Size) {
        test_op = *(int*)(Data + offset);
        test_arg = *(int*)(Data + offset + 4);
        offset += 8;
    }
    sqlite3_test_control(test_op, test_arg);
    
    /* Prepare for sqlite3_deserialize */
    unsigned char *pData = NULL;
    sqlite3_int64 szDb = 0;
    sqlite3_int64 szBuf = 0;
    unsigned mFlags = 0;
    
    if (Size > offset) {
        size_t remaining = Size - offset;
        
        /* Allocate buffer using sqlite3_malloc for SQLITE_DESERIALIZE_FREEONCLOSE */
        pData = (unsigned char*)sqlite3_malloc(remaining);
        if (pData) {
            memcpy(pData, Data + offset, remaining);
            
            /* Set sizes - ensure szDb is multiple of page size (512) */
            szBuf = remaining;
            szDb = (remaining / 512) * 512;
            if (szDb == 0 && remaining > 0) {
                szDb = 512;
            }
            
            /* Use some flags based on input */
            mFlags = 0;
            if (remaining > 0) {
                mFlags = Data[offset] & 0x03; /* Use first 2 bits for flags */
            }
            
            /* Call sqlite3_deserialize */
            sqlite3_deserialize(db, "main", pData, szDb, szBuf, mFlags);
            
            /* Free the buffer if not using FREEONCLOSE flag */
            if (!(mFlags & SQLITE_DESERIALIZE_FREEONCLOSE)) {
                sqlite3_free(pData);
            }
        }
    }
    
    /* Call sqlite3_file_control */
    const char *zDbName = "main";
    int file_op = 0;
    void *file_arg = NULL;
    
    /* Use last few bytes for file control parameters */
    if (Size >= 4) {
        file_op = *(int*)(Data + (Size % 4)); /* Use modulo to avoid overflow */
    }
    
    sqlite3_file_control(db, zDbName, file_op, file_arg);
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}