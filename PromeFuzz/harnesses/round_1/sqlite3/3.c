// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int progress_callback(void *arg) {
    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    sqlite3_int64 prev_heap_limit;
    int prev_limit_val;
    
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    const char *filename = "./dummy_file";
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        remove(filename);
        return 0;
    }
    
    sqlite3_progress_handler(db, 1000, progress_callback, NULL);
    
    if (Size >= sizeof(int)) {
        int limit_id = *(const int*)Data % 12;
        int new_limit = *(const int*)(Data + sizeof(int) % Size) % 1000;
        prev_limit_val = sqlite3_limit(db, limit_id, new_limit);
    } else {
        prev_limit_val = sqlite3_limit(db, SQLITE_LIMIT_LENGTH, 1000);
    }
    
    if (Size >= 2 * sizeof(int)) {
        int limit_id2 = *(const int*)(Data + 2 * sizeof(int) % Size) % 12;
        int new_limit2 = *(const int*)(Data + 3 * sizeof(int) % Size) % 1000;
        prev_limit_val = sqlite3_limit(db, limit_id2, new_limit2);
    } else {
        prev_limit_val = sqlite3_limit(db, SQLITE_LIMIT_SQL_LENGTH, 1000);
    }
    
    sqlite3_int64 heap_limit = 1024 * 1024;
    if (Size >= sizeof(sqlite3_int64)) {
        heap_limit = *(const sqlite3_int64*)Data;
    }
    prev_heap_limit = sqlite3_hard_heap_limit64(heap_limit);
    
    if (Size >= 4 * sizeof(int)) {
        int limit_id3 = *(const int*)(Data + 4 * sizeof(int) % Size) % 12;
        int new_limit3 = *(const int*)(Data + 5 * sizeof(int) % Size) % 1000;
        prev_limit_val = sqlite3_limit(db, limit_id3, new_limit3);
    } else {
        prev_limit_val = sqlite3_limit(db, SQLITE_LIMIT_COLUMN, 100);
    }
    
    if (Size >= 6 * sizeof(int)) {
        int op = *(const int*)(Data + 6 * sizeof(int) % Size);
        int val = *(const int*)(Data + 7 * sizeof(int) % Size);
        rc = sqlite3_db_config(db, op, val);
    } else {
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1);
    }
    
    sqlite3_close(db);
    sqlite3_shutdown();
    remove(filename);
    
    return 0;
}