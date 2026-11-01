// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Progress callback for sqlite3_progress_handler.
 * Returns 0 to continue, non-zero to abort.
 */
static int progress_callback(void *arg) {
    if (arg) {
        int *counter = (int *)arg;
        (*counter)++;
        /* Abort if counter exceeds a threshold to trigger different paths */
        if (*counter > 100) {
            return 1;
        }
    }
    return 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment */
    sqlite3 *db = NULL;
    int rc;
    int progress_counter = 0;
    void *progress_arg = &progress_counter;
    
    /* Use a dummy file for the database */
    const char *filename = "./dummy_file";
    
    /* Initialize SQLite library */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Open database with flags derived from input to explore different states */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > 0) {
        /* Use first byte to vary flags slightly, but keep it safe */
        if (Data[0] & 0x01) flags |= SQLITE_OPEN_NOFOLLOW;
        if (Data[0] & 0x02) flags |= SQLITE_OPEN_URI;
        if (Data[0] & 0x04) flags |= SQLITE_OPEN_EXCLUSIVE;
    }
    
    rc = sqlite3_open_v2(filename, &db, flags, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* 2. Invoke target functions in the specified order */
    
    /* sqlite3_progress_handler */
    int n_ops = 100;
    if (Size > 1) {
        n_ops = Data[1] % 200 + 1; /* Range 1-200 */
    }
    /* Randomly disable progress handler sometimes */
    if (Size > 2 && Data[2] % 3 == 0) {
        sqlite3_progress_handler(db, -1, NULL, NULL);
    } else {
        sqlite3_progress_handler(db, n_ops, progress_callback, progress_arg);
    }

    /* sqlite3_limit (1st call) */
    /* Use input to select limit ID and new value. 
       We use a safe range of known limit IDs (0-19) */
    int limit_id_1 = SQLITE_LIMIT_SQL_LENGTH;
    if (Size > 3) {
        limit_id_1 = Data[3] % 20; 
    }
    int new_val_1 = 1000;
    if (Size > 4) {
        new_val_1 = (Data[4] * 100) + 100;
    }
    sqlite3_limit(db, limit_id_1, new_val_1);

    /* sqlite3_limit (2nd call) */
    /* Fixed to a known valid limit ID to avoid compilation errors */
    int limit_id_2 = SQLITE_LIMIT_VARIABLE_NUMBER;
    if (Size > 5) {
        limit_id_2 = Data[5] % 20;
    }
    int new_val_2 = -1; /* Query current limit */
    if (Size > 6 && Data[6] % 2 == 0) {
        new_val_2 = (Data[6] * 50) + 50;
    }
    sqlite3_limit(db, limit_id_2, new_val_2);

    /* sqlite3_hard_heap_limit64 */
    sqlite3_int64 heap_limit = 1024 * 1024; /* 1MB default */
    if (Size > 7) {
        heap_limit = ((sqlite3_int64)Data[7]) * 1024 * 1024;
        if (heap_limit < 0) heap_limit = 0;
    }
    sqlite3_hard_heap_limit64(heap_limit);

    /* sqlite3_limit (3rd call) */
    int limit_id_3 = SQLITE_LIMIT_EXPR_DEPTH;
    if (Size > 8) {
        limit_id_3 = Data[8] % 20;
    }
    int new_val_3 = 500;
    if (Size > 9) {
        new_val_3 = (Data[9] * 10) + 10;
    }
    sqlite3_limit(db, limit_id_3, new_val_3);

    /* sqlite3_db_config */
    /* Use input to select operation code */
    int db_config_op = SQLITE_DBCONFIG_LOOKASIDE;
    if (Size > 10) {
        db_config_op = Data[10] % 10; /* Limit to first 10 ops to avoid undefined behavior */
    }
    
    /* Provide arguments based on the operation */
    int db_config_rc = SQLITE_OK;
    switch (db_config_op) {
        case SQLITE_DBCONFIG_LOOKASIDE:
            if (Size > 11) {
                int sz = Data[11] * 10 + 10;
                int n = Data[12] % 10 + 1;
                db_config_rc = sqlite3_db_config(db, db_config_op, sz, sz, n);
            }
            break;
        case SQLITE_DBCONFIG_ENABLE_FKEY:
            if (Size > 11) {
                int on = Data[11] % 2;
                db_config_rc = sqlite3_db_config(db, db_config_op, on);
            }
            break;
        case SQLITE_DBCONFIG_ENABLE_TRIGGER:
            if (Size > 11) {
                int on = Data[11] % 2;
                db_config_rc = sqlite3_db_config(db, db_config_op, on);
            }
            break;
        case SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER:
            if (Size > 11) {
                int on = Data[11] % 2;
                db_config_rc = sqlite3_db_config(db, db_config_op, on);
            }
            break;
        case SQLITE_DBCONFIG_ENABLE_LOAD_EXTENSION:
            if (Size > 11) {
                int on = Data[11] % 2;
                db_config_rc = sqlite3_db_config(db, db_config_op, on);
            }
            break;
        default:
            /* For other ops, try with minimal args if safe, or skip */
            break;
    }

    /* 3. Cleanup */
    if (db) {
        sqlite3_close(db);
    }
    
    /* Shutdown SQLite library */
    sqlite3_shutdown();

    return 0;
}