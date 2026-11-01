// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:185823:16 in sqlite3.h
// sqlite3_open_v2 at sqlite3.c:189317:16 in sqlite3.h
// sqlite3_shutdown at sqlite3.c:186005:16 in sqlite3.h
// sqlite3_progress_handler at sqlite3.c:187437:17 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_hard_heap_limit64 at sqlite3.c:31202:26 in sqlite3.h
// sqlite3_limit at sqlite3.c:188621:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Dummy progress callback for fuzzing */
static int dummy_progress_handler(void *arg) {
    (void)arg;
    /* Always return 0 to allow execution to continue */
    return 0;
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* 1. Prepare the environment: Initialize SQLite */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* 2. Open a database connection using a dummy file */
    /* Using :memory: or a dummy file. The instructions say use "./dummy_file" if a file is required.
       sqlite3_open_v2 creates the file if it doesn't exist with appropriate flags. */
    rc = sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* 3. Invoke target functions in the specified order */

    /* sqlite3_progress_handler */
    /* Use data from fuzz input to determine N (number of ops) */
    int n_ops = (Size > 0) ? (int)Data[0] : 100;
    if (n_ops < 1) n_ops = 1; /* Ensure valid positive number for handler */
    sqlite3_progress_handler(db, n_ops, dummy_progress_handler, NULL);

    /* sqlite3_limit (1st call) */
    /* Use fuzz data to select limit ID and new value */
    int limit_id_1 = (Size > 1) ? (Data[1] % 10) : 0; /* SQLITE_LIMIT_SQL_LENGTH is 0, etc. */
    int limit_val_1 = (Size > 2) ? (int)Data[2] : 1000;
    sqlite3_limit(db, limit_id_1, limit_val_1);

    /* sqlite3_limit (2nd call) */
    int limit_id_2 = (Size > 3) ? (Data[3] % 10) : 0;
    int limit_val_2 = (Size > 4) ? (int)Data[4] : 500;
    sqlite3_limit(db, limit_id_2, limit_val_2);

    /* sqlite3_hard_heap_limit64 */
    /* Use fuzz data for heap limit. Ensure it's non-negative for setting, or -1 to query. */
    sqlite3_int64 heap_limit = -1;
    if (Size > 5) {
        /* Construct a 64-bit value from input if possible, otherwise use a small value */
        if (Size >= 13) {
            heap_limit = *(sqlite3_int64*)&Data[5];
        } else {
            heap_limit = (sqlite3_int64)Data[5];
        }
    }
    /* Ensure we don't set a negative limit unless we want to query (-1) */
    if (heap_limit < 0) heap_limit = -1; 
    sqlite3_hard_heap_limit64(heap_limit);

    /* sqlite3_limit (3rd call) */
    int limit_id_3 = (Size > 6) ? (Data[6] % 10) : 0;
    int limit_val_3 = (Size > 7) ? (int)Data[7] : 2000;
    sqlite3_limit(db, limit_id_3, limit_val_3);

    /* sqlite3_db_config */
    /* Configure the database connection. SQLITE_DBCONFIG_LOOKASIDE is a common op. */
    int db_config_op = SQLITE_DBCONFIG_ENABLE_FKEY;
    int enable_flag = 1;

    if (Size > 8) {
        /* Vary the operation based on input, though only a few are valid */
        int op_choice = Data[8] % 3;
        if (op_choice == 0) db_config_op = SQLITE_DBCONFIG_ENABLE_FKEY;
        else if (op_choice == 1) db_config_op = SQLITE_DBCONFIG_ENABLE_TRIGGER;
        else db_config_op = SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER;

        if (db_config_op == SQLITE_DBCONFIG_ENABLE_FKEY || 
            db_config_op == SQLITE_DBCONFIG_ENABLE_TRIGGER ||
            db_config_op == SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER) {
            enable_flag = (Size > 9) ? (int)Data[9] : 1;
        }
    }

    /* Only call db_config with valid arguments for the chosen operation */
    if (db_config_op == SQLITE_DBCONFIG_ENABLE_FKEY || 
        db_config_op == SQLITE_DBCONFIG_ENABLE_TRIGGER ||
        db_config_op == SQLITE_DBCONFIG_ENABLE_FTS3_TOKENIZER) {
        sqlite3_db_config(db, db_config_op, enable_flag);
    } else {
        /* Fallback to a safe operation if random choice picked something unsupported in this context */
        sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1);
    }

    /* 4. Cleanup */
    if (db) {
        sqlite3_close(db);
    }

    /* Shutdown SQLite library */
    sqlite3_shutdown();

    return 0;
}