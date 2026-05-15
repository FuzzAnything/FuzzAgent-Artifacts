// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:174573:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:171261:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:172693:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:173877:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:17195:26 in sqlite3.h
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
    /* Simple progress handler that returns 0 to continue */
    return 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *filename = "./dummy_file";
    int flags;
    int limit_id1, limit_id2, limit_id3;
    int new_limit1, new_limit2, new_limit3;
    int old_limit;
    sqlite3_int64 heap_limit;
    sqlite3_int64 old_heap_limit;
    int db_config_op;
    int db_config_arg;
    FILE *fp;
    
    /* Initialize SQLite library */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    /* Create dummy file for database */
    fp = fopen(filename, "wb");
    if (fp) {
        if (Size > 0) {
            fwrite(Data, 1, Size, fp);
        }
        fclose(fp);
    }
    
    /* Open database connection */
    flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }
    
    /* Set progress handler */
    if (Size >= 4) {
        int progress_interval = *(int*)Data % 10000;
        if (progress_interval < 0) progress_interval = -progress_interval;
        sqlite3_progress_handler(db, progress_interval, progress_callback, NULL);
    } else {
        sqlite3_progress_handler(db, 1000, progress_callback, NULL);
    }
    
    /* First sqlite3_limit call */
    if (Size >= 8) {
        limit_id1 = *(int*)Data % 12; /* SQLITE_N_LIMIT is typically 12 */
        new_limit1 = *((int*)Data + 1) % 1000;
    } else {
        limit_id1 = SQLITE_LIMIT_SQL_LENGTH;
        new_limit1 = 1000;
    }
    old_limit = sqlite3_limit(db, limit_id1, new_limit1);
    
    /* Second sqlite3_limit call */
    if (Size >= 16) {
        limit_id2 = *((int*)Data + 2) % 12; /* SQLITE_N_LIMIT is typically 12 */
        new_limit2 = *((int*)Data + 3) % 1000;
    } else {
        limit_id2 = SQLITE_LIMIT_COLUMN;
        new_limit2 = 200;
    }
    old_limit = sqlite3_limit(db, limit_id2, new_limit2);
    
    /* Set hard heap limit */
    if (Size >= 24) {
        heap_limit = *((sqlite3_int64*)(Data + 16)) % (1024 * 1024 * 100);
        if (heap_limit < 0) heap_limit = -heap_limit;
    } else {
        heap_limit = 1024 * 1024 * 10; /* 10 MB */
    }
    old_heap_limit = sqlite3_hard_heap_limit64(heap_limit);
    
    /* Third sqlite3_limit call */
    if (Size >= 28) {
        limit_id3 = *((int*)Data + 6) % 12; /* SQLITE_N_LIMIT is typically 12 */
        new_limit3 = *((int*)Data + 7) % 1000;
    } else {
        limit_id3 = SQLITE_LIMIT_EXPR_DEPTH;
        new_limit3 = 100;
    }
    old_limit = sqlite3_limit(db, limit_id3, new_limit3);
    
    /* Configure database connection */
    if (Size >= 32) {
        db_config_op = *((int*)Data + 8) % 20; /* There are about 20 db config options */
        db_config_arg = *((int*)Data + 9) % 1000;
    } else {
        db_config_op = SQLITE_DBCONFIG_ENABLE_FKEY;
        db_config_arg = 1;
    }
    
    /* Try different db config operations based on the op value */
    switch (db_config_op) {
        case SQLITE_DBCONFIG_ENABLE_FKEY:
        case SQLITE_DBCONFIG_ENABLE_TRIGGER:
        case SQLITE_DBCONFIG_ENABLE_VIEW:
        case SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER:
        case SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION:
            rc = sqlite3_db_config(db, db_config_op, db_config_arg & 1, &old_limit);
            break;
        case SQLITE_DBCONFIG_LOOKASIDE:
            /* Lookaside configuration requires buffer and size */
            if (Size >= 40) {
                void *buffer = malloc(1024);
                int buf_size = 1024;
                int slot_size = 128;
                rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, buffer, slot_size, buf_size / slot_size);
                free(buffer);
            }
            break;
        default:
            /* For other configs, just try with a single argument */
            rc = sqlite3_db_config(db, db_config_op, db_config_arg);
            break;
    }
    
    /* Clean up */
    sqlite3_close(db);
    sqlite3_shutdown();
    
    /* Remove dummy file */
    remove(filename);
    
    return 0;
}