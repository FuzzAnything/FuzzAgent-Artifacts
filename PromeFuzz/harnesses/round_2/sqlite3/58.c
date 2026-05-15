// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_autocheckpoint at sqlite3.c:173377:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int wal_hook_callback(void *pArg, sqlite3 *db, const char *zDb, int nFrame) {
    // Simple callback that returns SQLITE_OK
    (void)pArg;
    (void)db;
    (void)zDb;
    (void)nFrame;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_58(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Enable WAL mode for testing WAL-related functions
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK && zErrMsg) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    // Create a table and insert some data to have WAL content
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a,b);", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK && zErrMsg) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    // Insert some data to generate WAL frames
    for (int i = 0; i < 10 && Size > 0; i++) {
        char sql[256];
        snprintf(sql, sizeof(sql), "INSERT INTO t1 VALUES(%d,%d);", i, i * 2);
        rc = sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
        if (rc != SQLITE_OK && zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }
    }
    
    // Begin a transaction to satisfy sqlite3_snapshot_get requirements
    rc = sqlite3_exec(db, "BEGIN;", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK && zErrMsg) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    // Use fuzzer data to determine which functions to call and with what parameters
    size_t offset = 0;
    
    // 1. Test sqlite3_wal_hook
    if (Size - offset >= 1) {
        void *pArg = (void*)(Data + offset);
        offset += 1;
        void *prev = sqlite3_wal_hook(db, wal_hook_callback, pArg);
        (void)prev;
    }
    
    // 2. Test sqlite3_snapshot_get
    if (Size - offset >= 1) {
        sqlite3_snapshot *pSnapshot = NULL;
        const char *zSchema = "main";
        rc = sqlite3_snapshot_get(db, zSchema, &pSnapshot);
        if (rc == SQLITE_OK && pSnapshot) {
            sqlite3_snapshot_free(pSnapshot);
        }
        offset += 1;
    }
    
    // 3. Test sqlite3_wal_checkpoint
    if (Size - offset >= 1) {
        const char *zDb = "main";
        rc = sqlite3_wal_checkpoint(db, zDb);
        (void)rc;
        offset += 1;
    }
    
    // 4. Test sqlite3_db_cacheflush
    if (Size - offset >= 1) {
        rc = sqlite3_db_cacheflush(db);
        (void)rc;
        offset += 1;
    }
    
    // 5. Test sqlite3_wal_autocheckpoint
    if (Size - offset >= 2) {
        int N = (int)Data[offset];
        offset += 1;
        rc = sqlite3_wal_autocheckpoint(db, N);
        (void)rc;
    }
    
    // 6. Test sqlite3_wal_checkpoint_v2
    if (Size - offset >= 4) {
        const char *zDb = "main";
        int eMode = (int)Data[offset] % 5; // 0-4 for different checkpoint modes
        offset += 1;
        int pnLog = 0;
        int pnCkpt = 0;
        rc = sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
        (void)rc;
        (void)pnLog;
        (void)pnCkpt;
    }
    
    // Commit the transaction
    rc = sqlite3_exec(db, "COMMIT;", NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK && zErrMsg) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    // Cleanup
    sqlite3_close(db);
    
    return 0;
}