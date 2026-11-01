// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper function to create a dummy file if needed.
 */
static void ensure_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fclose(f);
    }
}

/*
 * Progress callback for sqlite3_progress_handler.
 * Returns 0 to continue execution.
 */
static int progress_callback(void *arg) {
    (void)arg;
    return 0;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Step 1: Initialize the SQLite library */
    rc = sqlite3_initialize();
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Ensure the dummy file exists or can be created */
    ensure_dummy_file();

    /* Step 2: Open the database with fine-grained control */
    rc = sqlite3_open_v2("./dummy_file", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (rc != SQLITE_OK || db == NULL) {
        sqlite3_shutdown();
        return 0;
    }

    /* Step 3: Set a progress handler */
    int n_ops = 100;
    if (Size > 0) {
        n_ops = (int)Data[0];
        if (n_ops <= 0) n_ops = 100;
    }
    sqlite3_progress_handler(db, n_ops, progress_callback, NULL);

    /* Step 4: First sqlite3_limit call */
    int limit_id_1 = SQLITE_LIMIT_SQL_LENGTH;
    int new_val_1 = 1000;
    if (Size > 2) {
        limit_id_1 = Data[1] % 13; 
        new_val_1 = (int)Data[2] * 100;
    }
    sqlite3_limit(db, limit_id_1, new_val_1);

    /* Step 5: Second sqlite3_limit call */
    int limit_id_2 = SQLITE_LIMIT_COLUMN;
    int new_val_2 = 500;
    if (Size > 4) {
        limit_id_2 = Data[3] % 13;
        new_val_2 = (int)Data[4] * 100;
    }
    sqlite3_limit(db, limit_id_2, new_val_2);

    /* Step 6: Set hard heap limit */
    sqlite3_int64 heap_limit = 1048576; /* 1MB default */
    if (Size > 5) {
        if (Size >= 13) {
            heap_limit = 0;
            for (int i = 0; i < 8; i++) {
                heap_limit = (heap_limit << 8) | Data[5 + i];
            }
        } else {
            heap_limit = (sqlite3_int64)Data[5] * 100000;
        }
    }
    sqlite3_hard_heap_limit64(heap_limit);

    /* Step 7: Third sqlite3_limit call */
    int limit_id_3 = SQLITE_LIMIT_VARIABLE_NUMBER;
    int new_val_3 = 2500;
    if (Size > 7) {
        limit_id_3 = Data[6] % 13;
        new_val_3 = (int)Data[7] * 100;
    }
    sqlite3_limit(db, limit_id_3, new_val_3);

    /* Step 8: Configure the database connection */
    int op = SQLITE_DBCONFIG_LOOKASIDE;
    void *pArg = NULL;
    int szArg = 0;
    int nArg = 0;

    if (Size > 8) {
        op = Data[8] % 10;
        if (Size > 12) {
            szArg = (int)Data[9] * 10;
            nArg = (int)Data[10];
            pArg = NULL; 
        }
    }

    rc = sqlite3_db_config(db, op, pArg, szArg, nArg);

    /* Cleanup */
    if (db) {
        sqlite3_close(db);
    }

    sqlite3_shutdown();

    return 0;
}