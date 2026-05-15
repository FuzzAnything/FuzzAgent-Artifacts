// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
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

static int progress_callback(void *arg) {
    /* Simple progress handler that does nothing */
    (void)arg;
    return 0;
}

int LLVMFuzzerTestOneInput_1(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_MEMORY;
    
    /* Initialize SQLite library */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    /* Open database connection */
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }
    
    /* Set progress handler */
    int n_ops = 1000;
    if (Size > 0) {
        n_ops = Data[0] % 10000 + 1;
    }
    sqlite3_progress_handler(db, n_ops, progress_callback, NULL);
    
    /* Call sqlite3_limit multiple times with different limit IDs */
    int limit_ids[] = {
        SQLITE_LIMIT_LENGTH,
        SQLITE_LIMIT_SQL_LENGTH,
        SQLITE_LIMIT_COLUMN,
        SQLITE_LIMIT_EXPR_DEPTH,
        SQLITE_LIMIT_COMPOUND_SELECT,
        SQLITE_LIMIT_VDBE_OP,
        SQLITE_LIMIT_FUNCTION_ARG,
        SQLITE_LIMIT_ATTACHED,
        SQLITE_LIMIT_LIKE_PATTERN_LENGTH,
        SQLITE_LIMIT_VARIABLE_NUMBER,
        SQLITE_LIMIT_TRIGGER_DEPTH,
        SQLITE_LIMIT_WORKER_THREADS
    };
    int num_limits = sizeof(limit_ids) / sizeof(limit_ids[0]);
    
    /* First sqlite3_limit call - set some limits */
    if (num_limits > 0 && Size > 1) {
        int new_val = Data[1] % 1000;
        sqlite3_limit(db, limit_ids[0], new_val);
    }
    
    /* Second sqlite3_limit call - query current limit */
    if (num_limits > 1) {
        sqlite3_limit(db, limit_ids[1], -1);
    }
    
    /* Set hard heap limit */
    sqlite3_int64 heap_limit = 1024 * 1024; /* 1MB default */
    if (Size > 2) {
        heap_limit = (sqlite3_int64)(Data[2]) * 1024;
        if (heap_limit < 0) heap_limit = -heap_limit;
    }
    sqlite3_hard_heap_limit64(heap_limit);
    
    /* Third sqlite3_limit call - set another limit */
    if (num_limits > 2 && Size > 3) {
        int new_val = Data[3] % 500;
        sqlite3_limit(db, limit_ids[2], new_val);
    }
    
    /* Call sqlite3_db_config with various operations */
    if (Size > 4) {
        /* SQLITE_DBCONFIG_ENABLE_FKEY */
        int enable_fkey = (Data[4] & 1) ? 1 : 0;
        sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, enable_fkey, &rc);
    }
    
    if (Size > 5) {
        /* SQLITE_DBCONFIG_ENABLE_TRIGGER */
        int enable_trigger = (Data[5] & 1) ? 1 : 0;
        sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, enable_trigger, &rc);
    }
    
    if (Size > 6) {
        /* SQLITE_DBCONFIG_ENABLE_VIEW */
        int enable_view = (Data[6] & 1) ? 1 : 0;
        sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW, enable_view, &rc);
    }
    
    if (Size > 7) {
        /* SQLITE_DBCONFIG_LOOKASIDE configuration */
        int lookaside_size = 128;
        int lookaside_count = 8;
        if (Data[7] > 0) {
            lookaside_size = Data[7] * 8;
            lookaside_count = Data[7] % 100 + 1;
        }
        sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, NULL, lookaside_size, lookaside_count);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    sqlite3_shutdown();
    
    return 0;
}