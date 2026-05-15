// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_memory_highwater at sqlite3.c:17266:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_soft_heap_limit64 at sqlite3.c:17153:26 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_status64 at sqlite3.c:10741:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
// sqlite3_db_status64 at sqlite3.c:10810:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_memory_used at sqlite3.c:17255:26 in sqlite3.h
// sqlite3_status at sqlite3.c:10766:16 in sqlite3.h
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

static void fuzz_soft_heap_limit64(const uint8_t *data, size_t size) {
    sqlite3_int64 limit = 0;
    
    if (size >= sizeof(sqlite3_int64)) {
        memcpy(&limit, data, sizeof(sqlite3_int64));
    } else if (size > 0) {
        limit = (sqlite3_int64)data[0];
    }
    
    sqlite3_soft_heap_limit64(limit);
    sqlite3_soft_heap_limit64(-1);
    sqlite3_soft_heap_limit64(0);
}

static void fuzz_status64(const uint8_t *data, size_t size) {
    sqlite3_int64 current = 0;
    sqlite3_int64 highwater = 0;
    int op = 0;
    int resetFlag = 0;
    
    if (size >= sizeof(int)) {
        memcpy(&op, data, sizeof(int));
        if (size >= sizeof(int) + 1) {
            resetFlag = data[sizeof(int)] & 1;
        }
    } else if (size > 0) {
        op = data[0];
        resetFlag = (size > 1) ? (data[1] & 1) : 0;
    }
    
    sqlite3_status64(op, &current, &highwater, resetFlag);
    
    for (int i = 0; i < 10; i++) {
        sqlite3_status64(i, &current, &highwater, resetFlag);
    }
}

static void fuzz_db_status64(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    sqlite3_int64 current = 0;
    sqlite3_int64 highwater = 0;
    int op = 0;
    int resetFlag = 0;
    
    if (sqlite3_open(":memory:", &db) == SQLITE_OK) {
        if (size >= sizeof(int)) {
            memcpy(&op, data, sizeof(int));
            if (size >= sizeof(int) + 1) {
                resetFlag = data[sizeof(int)] & 1;
            }
        } else if (size > 0) {
            op = data[0];
            resetFlag = (size > 1) ? (data[1] & 1) : 0;
        }
        
        sqlite3_db_status64(db, op, &current, &highwater, resetFlag);
        
        for (int i = 0; i < 10; i++) {
            sqlite3_db_status64(db, i, &current, &highwater, resetFlag);
        }
        
        sqlite3_close(db);
    }
}

static void fuzz_memory_used(void) {
    sqlite3_int64 used = sqlite3_memory_used();
    (void)used;
}

static void fuzz_status(const uint8_t *data, size_t size) {
    int current = 0;
    int highwater = 0;
    int op = 0;
    int resetFlag = 0;
    
    if (size >= sizeof(int)) {
        memcpy(&op, data, sizeof(int));
        if (size >= sizeof(int) + 1) {
            resetFlag = data[sizeof(int)] & 1;
        }
    } else if (size > 0) {
        op = data[0];
        resetFlag = (size > 1) ? (data[1] & 1) : 0;
    }
    
    sqlite3_status(op, &current, &highwater, resetFlag);
    
    for (int i = 0; i < 10; i++) {
        sqlite3_status(i, &current, &highwater, resetFlag);
    }
}

static void fuzz_memory_highwater(const uint8_t *data, size_t size) {
    int resetFlag = 0;
    
    if (size > 0) {
        resetFlag = data[0] & 1;
    }
    
    sqlite3_int64 highwater = sqlite3_memory_highwater(resetFlag);
    (void)highwater;
    
    sqlite3_memory_highwater(0);
    sqlite3_memory_highwater(1);
}

int LLVMFuzzerTestOneInput_117(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    fuzz_soft_heap_limit64(data, size);
    fuzz_status64(data, size);
    fuzz_db_status64(data, size);
    fuzz_memory_used();
    fuzz_status(data, size);
    fuzz_memory_highwater(data, size);
    
    return 0;
}