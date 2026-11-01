// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
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
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to extract an integer from the fuzz input.
 * Returns a value based on the input data or a default if data is insufficient.
 */
static int get_int_from_input(const uint8_t *Data, size_t Size, size_t *offset, int default_val) {
    if (*offset + sizeof(int) <= Size) {
        int val;
        memcpy(&val, Data + *offset, sizeof(int));
        *offset += sizeof(int);
        return val;
    }
    return default_val;
}

/*
 * Static helper function to extract a 64-bit integer from the fuzz input.
 */
static sqlite3_int64 get_int64_from_input(const uint8_t *Data, size_t Size, size_t *offset, sqlite3_int64 default_val) {
    if (*offset + sizeof(sqlite3_int64) <= Size) {
        sqlite3_int64 val;
        memcpy(&val, Data + *offset, sizeof(sqlite3_int64));
        *offset += sizeof(sqlite3_int64);
        return val;
    }
    return default_val;
}

/*
 * Progress callback function for sqlite3_progress_handler.
 * Returns 0 to continue, 1 to abort (simulated based on input).
 */
static int progress_callback(void *arg) {
    int *abort_flag = (int *)arg;
    if (abort_flag && *abort_flag) {
        return 1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    /* Step 1: Prepare the environment */
    sqlite3 *db = NULL;
    int rc;
    size_t offset = 0;

    /* Initialize SQLite library */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Open a database file (using dummy file as per instructions) */
    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if (Size > offset) {
        /* Use input to potentially modify flags, but ensure valid bits are set */
        flags = (flags & ~0x000000FF) | (Data[offset] & 0x000000FF);
        offset++;
    }
    
    rc = sqlite3_open_v2("./dummy_file", &db, flags, NULL);
    
    /* 
     * FIX: Ensure sqlite3_close is called if db is not NULL, even if rc != SQLITE_OK.
     * sqlite3_open_v2 may return an error code but still allocate a database handle.
     * Calling sqlite3_shutdown() before closing the handle causes memory leaks.
     */
    if (rc != SQLITE_OK && db != NULL) {
        sqlite3_close(db);
        db = NULL;
    }
    
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* Step 2: Invoke target functions in the specified order */

    /* 1. sqlite3_progress_handler */
    int n_ops = get_int_from_input(Data, Size, &offset, 100);
    int abort_flag = 0;
    if (Size > offset) {
        abort_flag = (int)Data[offset];
        offset++;
    }
    
    if (n_ops < 0) n_ops = 1; 
    
    sqlite3_progress_handler(db, n_ops, progress_callback, &abort_flag);

    /* 2. sqlite3_limit (First call) */
    int limit_id_1 = get_int_from_input(Data, Size, &offset, SQLITE_LIMIT_SQL_LENGTH);
    int limit_val_1 = get_int_from_input(Data, Size, &offset, 1000);
    sqlite3_limit(db, limit_id_1, limit_val_1);

    /* 3. sqlite3_limit (Second call) */
    int limit_id_2 = get_int_from_input(Data, Size, &offset, SQLITE_LIMIT_COLUMN);
    int limit_val_2 = get_int_from_input(Data, Size, &offset, 500);
    sqlite3_limit(db, limit_id_2, limit_val_2);

    /* 4. sqlite3_hard_heap_limit64 */
    sqlite3_int64 heap_limit = get_int64_from_input(Data, Size, &offset, 1024 * 1024);
    if (heap_limit < 0) heap_limit = -1;
    sqlite3_hard_heap_limit64(heap_limit);

    /* 5. sqlite3_limit (Third call) */
    int limit_id_3 = get_int_from_input(Data, Size, &offset, SQLITE_LIMIT_VARIABLE_NUMBER);
    int limit_val_3 = get_int_from_input(Data, Size, &offset, 2500);
    sqlite3_limit(db, limit_id_3, limit_val_3);

    /* 6. sqlite3_db_config */
    int db_config_op = get_int_from_input(Data, Size, &offset, SQLITE_DBCONFIG_LOOKASIDE);
    
    if (db_config_op == SQLITE_DBCONFIG_LOOKASIDE) {
        void *pArg = NULL;
        int nSize = get_int_from_input(Data, Size, &offset, 128);
        int nReqSize = get_int_from_input(Data, Size, &offset, 32);
        if (nSize < 0) nSize = 128;
        if (nReqSize < 0) nReqSize = 32;
        sqlite3_db_config(db, db_config_op, pArg, nSize, nReqSize);
    } else if (db_config_op == SQLITE_DBCONFIG_ENABLE_FKEY) {
        int enable = get_int_from_input(Data, Size, &offset, 1);
        sqlite3_db_config(db, db_config_op, enable);
    } else if (db_config_op == SQLITE_DBCONFIG_ENABLE_TRIGGER) {
        int enable = get_int_from_input(Data, Size, &offset, 1);
        sqlite3_db_config(db, db_config_op, enable);
    } else if (db_config_op == SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER) {
        int enable = get_int_from_input(Data, Size, &offset, 1);
        sqlite3_db_config(db, db_config_op, enable);
    } else {
        int dummy = get_int_from_input(Data, Size, &offset, 0);
        sqlite3_db_config(db, db_config_op, dummy);
    }

    /* Step 3: Cleanup */
    if (db) {
        sqlite3_close(db);
    }
    
    /* Shutdown SQLite library */
    sqlite3_shutdown();

    return 0;
}