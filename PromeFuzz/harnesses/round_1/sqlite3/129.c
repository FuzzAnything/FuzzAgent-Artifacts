// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:173377:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:173377:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int dummy_wal_hook(void *pArg, sqlite3 *db, const char *zDb, int nFrame) {
    (void)pArg;
    (void)db;
    (void)zDb;
    (void)nFrame;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *zDb = "main";
    int nLog, nCkpt;
    sqlite3_snapshot *pSnapshot = NULL;
    void *pWalArg = NULL;
    
    // Use first byte to determine which functions to call
    uint8_t selector = Size > 0 ? Data[0] : 0;
    size_t data_offset = Size > 0 ? 1 : 0;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Enable WAL mode
    sqlite3_exec(db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    
    // Create a table and insert some data to have WAL content
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b)", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2)", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(3, 4)", NULL, NULL, NULL);
    
    // Test sqlite3_wal_hook
    if ((selector & 0x01) && (Size - data_offset) >= sizeof(void*)) {
        pWalArg = (void*)(Data + data_offset);
        data_offset += sizeof(void*);
        sqlite3_wal_hook(db, dummy_wal_hook, pWalArg);
    } else {
        sqlite3_wal_hook(db, NULL, NULL);
    }
    
    // Test sqlite3_wal_autocheckpoint
    if ((selector & 0x02) && (Size - data_offset) >= sizeof(int)) {
        int N = *(int*)(Data + data_offset);
        data_offset += sizeof(int);
        sqlite3_wal_autocheckpoint(db, N);
    } else {
        sqlite3_wal_autocheckpoint(db, 1000);
    }
    
    // Test sqlite3_wal_checkpoint
    if (selector & 0x04) {
        sqlite3_wal_checkpoint(db, zDb);
    }
    
    // Test sqlite3_db_cacheflush
    if (selector & 0x08) {
        sqlite3_db_cacheflush(db);
    }
    
    // Test sqlite3_wal_checkpoint_v2
    if ((selector & 0x10) && (Size - data_offset) >= sizeof(int)) {
        int eMode = *(int*)(Data + data_offset) % 5; // 0-4 for checkpoint modes
        data_offset += sizeof(int);
        sqlite3_wal_checkpoint_v2(db, zDb, eMode, &nLog, &nCkpt);
    }
    
    // Test sqlite3_snapshot_get
    if ((selector & 0x20) && (Size - data_offset) >= 1) {
        // Start a transaction to satisfy snapshot requirements
        sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);
        
        // Try to get a snapshot
        rc = sqlite3_snapshot_get(db, zDb, &pSnapshot);
        if (rc == SQLITE_OK && pSnapshot != NULL) {
            // Free the snapshot if we got one
            sqlite3_snapshot_free(pSnapshot);
        }
        
        sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}