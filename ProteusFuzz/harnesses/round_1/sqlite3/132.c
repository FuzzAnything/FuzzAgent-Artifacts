// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * WAL Hook Callback
 * Receives: user_data, db, db_name, wal_page_count
 * Returns: SQLITE_OK
 */
static int wal_hook_callback(void *pArg, sqlite3 *db, const char *zDb, int nPage) {
    (void)pArg;
    (void)db;
    (void)zDb;
    (void)nPage;
    return SQLITE_OK;
}

/*
 * Fuzz Driver Entry Point
 */
int LLVMFuzzerTestOneInput_132(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    int op;
    int val;
    void *prev_hook;
    int err_code;

    if (Size == 0) {
        return 0;
    }

    /* Initialize database connection using a dummy file */
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Ensure we have enough data to read an operation code */
    if (Size >= 1) {
        op = Data[0];
    } else {
        op = 0;
    }

    /*
     * Test sqlite3_wal_hook
     * Register a callback, then unregister it.
     */
    if (Size >= 2) {
        /* Use second byte to decide if we register or unregister */
        if (Data[1] & 0x01) {
            prev_hook = sqlite3_wal_hook(db, wal_hook_callback, (void *)(uintptr_t)(Data[1] & 0xFE));
            (void)prev_hook;
        } else {
            prev_hook = sqlite3_wal_hook(db, NULL, NULL);
            (void)prev_hook;
        }
    } else {
        /* Default: try to register */
        prev_hook = sqlite3_wal_hook(db, wal_hook_callback, (void *)0x1234);
        (void)prev_hook;
    }

    /*
     * Test sqlite3_busy_timeout
     * Set a timeout based on input data.
     */
    if (Size >= 3) {
        int timeout = (int)(Data[2] * 1000) % 5000; /* 0 to 4999 ms */
        sqlite3_busy_timeout(db, timeout);
    } else {
        sqlite3_busy_timeout(db, 100);
    }

    /*
     * Test sqlite3_db_config
     * Use the op variable to select a configuration operation.
     * We focus on SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE as per instructions.
     */
    if (Size >= 4) {
        int enable = (Data[3] & 0x01) ? 1 : 0;
        int prev_state = 0;
        /* SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE requires 3rd and 4th args */
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE, enable, &prev_state);
        (void)rc;
        (void)prev_state;
    }

    /*
     * Test sqlite3_errcode
     * Trigger a potential error by preparing invalid SQL, then check code.
     */
    if (Size >= 5) {
        /* Create a potentially malformed SQL string from input */
        char *sql = (char *)malloc(Size + 1);
        if (sql) {
            memcpy(sql, Data, Size);
            sql[Size] = '\0';
            
            /* Try to prepare, likely to fail if data is random */
            rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
            if (rc != SQLITE_OK) {
                err_code = sqlite3_errcode(db);
                (void)err_code;
            }
            
            /* If we got a statement, try to use it for sqlite3_db_handle */
            if (stmt != NULL) {
                sqlite3 *handle = sqlite3_db_handle(stmt);
                if (handle != NULL) {
                    /* Verify handle matches db */
                    if (handle != db) {
                        /* This is an unexpected state, but safe to ignore in fuzzing */
                    }
                    sqlite3_finalize(stmt);
                    stmt = NULL;
                }
            }
            free(sql);
        }
    }

    /*
     * Test sqlite3_close_v2
     * Safely close the database.
     */
    rc = sqlite3_close_v2(db);
    (void)rc;

    return 0;
}