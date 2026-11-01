// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:187411:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:188121:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:187488:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a dummy database file if needed.
 */
static int create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (!f) return -1;
    const char *dummy = "SQLite Fuzz Test Dummy Data";
    fwrite(dummy, 1, strlen(dummy), f);
    fclose(f);
    return 0;
}

/*
 * Static helper busy handler for sqlite3_busy_handler.
 */
static int busy_callback(void *arg, int count) {
    if (arg) {
        int *val = (int *)arg;
        return (*val % 2 != 0) ? 1 : 0;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    int param_n = 0;
    int param_ms = 0;
    int param_flags = 0;

    /* Ensure we have enough data to extract parameters safely */
    if (Size < 3) {
        return 0;
    }

    /* Extract fuzzing parameters safely */
    param_n = (int)(Data[0]); 
    param_ms = (int)(Data[1]);
    param_flags = (int)(Data[2]);
    
    /*
     * Step 1: Prepare the environment.
     */
    if (create_dummy_file() != 0) {
        rc = sqlite3_open(":memory:", &db);
    } else {
        rc = sqlite3_open("./dummy_file", &db);
    }

    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close_v2(db);
        return 0;
    }

    /*
     * Step 2 & 3: Invoke target functions diversely.
     */

    /* 1. sqlite3_busy_timeout */
    sqlite3_busy_timeout(db, param_ms);
    sqlite3_busy_timeout(db, 0);
    sqlite3_busy_timeout(db, -1);

    /* 2. sqlite3_busy_handler */
    sqlite3_busy_handler(db, busy_callback, (void *)(intptr_t)param_n);
    sqlite3_busy_handler(db, NULL, NULL);
    sqlite3_busy_handler(db, busy_callback, (void *)(intptr_t)(param_n + 1));

    /* 3. sqlite3_wal_autocheckpoint */
    sqlite3_wal_autocheckpoint(db, param_n);
    sqlite3_wal_autocheckpoint(db, 0);
    sqlite3_wal_autocheckpoint(db, -1);
    sqlite3_wal_autocheckpoint(db, 1000);

    /* 4. sqlite3_setlk_timeout */
    sqlite3_setlk_timeout(db, param_ms, param_flags);
    sqlite3_setlk_timeout(db, 0, 0);
    sqlite3_setlk_timeout(db, -1, 0);
    sqlite3_setlk_timeout(db, -2, 0);

    /* 5. sqlite3_errcode */
    int err = sqlite3_errcode(db);
    (void)err;

    /* Additional exploration */
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 1";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    sqlite3_errcode(db);
    sqlite3_wal_autocheckpoint(db, param_n % 100);
    
    /*
     * Step 2: Cleanup.
     */
    if (db) {
        sqlite3_close_v2(db);
    }

    return 0;
}