// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
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

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    
    FILE *fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_close(db);
        sqlite3_shutdown();
        return 0;
    }
    
    sqlite3_progress_handler(db, 1000, progress_callback, NULL);
    
    int limit_id1 = SQLITE_LIMIT_LENGTH;
    int new_limit1 = (Size > 0) ? (Data[0] % 1000) : -1;
    int prev_limit1 = sqlite3_limit(db, limit_id1, new_limit1);
    
    int limit_id2 = SQLITE_LIMIT_SQL_LENGTH;
    int new_limit2 = (Size > 1) ? (Data[1] % 1000) : -1;
    int prev_limit2 = sqlite3_limit(db, limit_id2, new_limit2);
    
    sqlite3_int64 heap_limit = (Size > 2) ? ((sqlite3_int64)Data[2] * 1000) : -1;
    sqlite3_int64 prev_heap_limit = sqlite3_hard_heap_limit64(heap_limit);
    
    int limit_id3 = SQLITE_LIMIT_COLUMN;
    int new_limit3 = (Size > 3) ? (Data[3] % 1000) : -1;
    int prev_limit3 = sqlite3_limit(db, limit_id3, new_limit3);
    
    int db_config_op = SQLITE_DBCONFIG_LOOKASIDE;
    int p1 = (Size > 4) ? (Data[4] * 100) : 0;
    int p2 = (Size > 5) ? (Data[5] * 100) : 0;
    void *p3 = NULL;
    rc = sqlite3_db_config(db, db_config_op, p1, p2, p3);
    
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}