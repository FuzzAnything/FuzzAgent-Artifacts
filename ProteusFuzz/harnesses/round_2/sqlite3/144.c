// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:188142:18 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:187468:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:186583:16 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_db_handle at sqlite3.c:94258:21 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

static int dummy_wal_callback(void *pArg, sqlite3 *db, const char *zDb, int nPage) {
    (void)pArg;
    (void)db;
    (void)zDb;
    (void)nPage;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    void *lookaside_buf = NULL;
    size_t lookaside_size = 0;
    int lookaside_cnt = 0;
    uint8_t op_code = 0;
    uint8_t timeout_val = 0;
    int enable = 0;

    if (Size < 1) {
        return 0;
    }

    // Initialize database connection
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Prepare a simple statement to test sqlite3_db_handle
    const char *sql = "SELECT 1;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close_v2(db);
        return 0;
    }

    // Parse input data safely
    if (Size >= 1) {
        op_code = Data[0] % 10;
    }
    if (Size >= 2) {
        timeout_val = Data[1] % 1000;
    }
    if (Size >= 3) {
        enable = (Data[2] % 2) ? 1 : 0;
    }

    // Test sqlite3_busy_timeout
    sqlite3_busy_timeout(db, timeout_val);

    // Test sqlite3_db_config
    // Only use SQLITE_DBCONFIG_LOOKASIDE if we have enough data to construct valid args
    if (Size >= 4 && op_code == 1) {
        // Construct valid lookaside arguments from input
        lookaside_size = (Data[2] % 100) + 1; // Ensure > 0
        lookaside_cnt = (Data[3] % 100);      // Non-negative

        // Ensure size is a multiple of sizeof(void*)
        lookaside_size = (lookaside_size / sizeof(void*)) * sizeof(void*);
        if (lookaside_size == 0) lookaside_size = sizeof(void*);

        lookaside_buf = malloc(lookaside_size);
        if (lookaside_buf) {
            // SQLITE_DBCONFIG_LOOKASIDE: (db, op, void*, int, int)
            rc = sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, lookaside_buf, (int)lookaside_size, lookaside_cnt);
            free(lookaside_buf);
            lookaside_buf = NULL;
        }
    } else if (op_code == 2) {
        // Test SQLITE_DBCONFIG_ENABLE_FKEY
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, enable, 0);
    } else if (op_code == 3) {
        // Test SQLITE_DBCONFIG_ENABLE_TRIGGER
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, enable, 0);
    } else {
        // Default safe operation or no-op
        rc = sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, 0);
    }

    // Test sqlite3_errcode
    int err = sqlite3_errcode(db);
    (void)err; // Suppress unused variable warning

    // Test sqlite3_db_handle
    sqlite3 *handle = sqlite3_db_handle(stmt);
    if (handle != db) {
        // This should ideally not happen, but we check for state
    }
    (void)handle;

    // Test sqlite3_wal_hook
    // Register a dummy callback
    void *prev_arg = sqlite3_wal_hook(db, dummy_wal_callback, (void *)(size_t)(Data[0] + (Size > 1 ? Data[1] : 0)));
    (void)prev_arg;

    // Try to unregister or change it
    sqlite3_wal_hook(db, NULL, NULL);

    // Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);

    return 0;
}