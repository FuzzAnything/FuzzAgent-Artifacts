// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
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
#include <unistd.h>

static int progress_handler(void *arg) {
    (void)arg;
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        flags = Data[0] % 256;
    }
    
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    void *progress_arg = NULL;
    if (Size > 1) {
        progress_arg = (void *)(intptr_t)Data[1];
    }
    sqlite3_progress_handler(db, 1000, progress_handler, progress_arg);
    
    int limit_id = SQLITE_LIMIT_LENGTH;
    if (Size > 2) {
        limit_id = Data[2] % 20;
    }
    int new_limit = -1;
    if (Size > 3) {
        new_limit = Data[3];
    }
    int prev_limit = sqlite3_limit(db, limit_id, new_limit);
    (void)prev_limit;
    
    limit_id = SQLITE_LIMIT_SQL_LENGTH;
    if (Size > 4) {
        limit_id = Data[4] % 20;
    }
    new_limit = -1;
    if (Size > 5) {
        new_limit = Data[5];
    }
    prev_limit = sqlite3_limit(db, limit_id, new_limit);
    (void)prev_limit;
    
    sqlite3_int64 heap_limit = -1;
    if (Size > 8) {
        memcpy(&heap_limit, Data + 6, sizeof(sqlite3_int64));
    }
    sqlite3_int64 prev_heap_limit = sqlite3_hard_heap_limit64(heap_limit);
    (void)prev_heap_limit;
    
    limit_id = SQLITE_LIMIT_COLUMN;
    if (Size > 9) {
        limit_id = Data[9] % 20;
    }
    new_limit = -1;
    if (Size > 10) {
        new_limit = Data[10];
    }
    prev_limit = sqlite3_limit(db, limit_id, new_limit);
    (void)prev_limit;
    
    int op = SQLITE_DBCONFIG_LOOKASIDE;
    if (Size > 11) {
        op = Data[11] % 20;
    }
    
    int arg1 = 0, arg2 = 0;
    void *arg3 = NULL;
    if (Size > 15) {
        arg1 = *(int *)(Data + 12);
        arg2 = *(int *)(Data + 16);
    }
    
    rc = sqlite3_db_config(db, op, arg1, arg2, arg3);
    (void)rc;
    
    sqlite3_close(db);
    unlink(filename);
    
    return 0;
}