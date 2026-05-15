// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void dummy_busy_handler(void* arg, int count) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap1 = NULL, *snap2 = NULL;
    int rc;
    char *errmsg = NULL;
    
    // Use first byte to determine mode of operation
    uint8_t mode = Size > 0 ? Data[0] : 0;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Set a busy handler to avoid SQLITE_BUSY errors
    sqlite3_busy_handler(db, (int(*)(void*,int))dummy_busy_handler, NULL);
    
    // Enable WAL mode for snapshot functionality
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL", NULL, NULL, &errmsg);
    if (rc != SQLITE_OK || errmsg) {
        sqlite3_free(errmsg);
        sqlite3_close(db);
        return 0;
    }
    
    // Create a table and insert some data to ensure WAL has content
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a INTEGER, b TEXT)", NULL, NULL, &errmsg);
    if (rc == SQLITE_OK) {
        // Insert some dummy data
        sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test')", NULL, NULL, NULL);
        sqlite3_exec(db, "INSERT INTO t1 VALUES(2, 'data')", NULL, NULL, NULL);
    }
    
    // Begin a transaction (required for snapshot_get)
    sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
    
    // Try to get a snapshot
    rc = sqlite3_snapshot_get(db, "main", &snap1);
    
    // Based on mode, try different operations
    switch (mode % 8) {
        case 0:
            // Try snapshot_recover
            sqlite3_snapshot_recover(db, "main");
            break;
        case 1:
            // Try snapshot_open if we have a snapshot
            if (rc == SQLITE_OK && snap1) {
                sqlite3_snapshot_open(db, "main", snap1);
            }
            break;
        case 2:
            // Try wal_checkpoint
            sqlite3_wal_checkpoint(db, "main");
            break;
        case 3:
            // Try snapshot_cmp (need two snapshots)
            if (rc == SQLITE_OK && snap1) {
                // Get another snapshot for comparison
                sqlite3_snapshot *snap2_local = NULL;
                if (sqlite3_snapshot_get(db, "main", &snap2_local) == SQLITE_OK) {
                    sqlite3_snapshot_cmp(snap1, snap2_local);
                    sqlite3_snapshot_free(snap2_local);
                }
            }
            break;
        case 4:
            // Try wal_checkpoint_v2 with different modes
            {
                int pnLog = 0, pnCkpt = 0;
                int checkpoint_mode = (mode >> 1) % 5; // 0-4 for different checkpoint modes
                sqlite3_wal_checkpoint_v2(db, "main", checkpoint_mode, &pnLog, &pnCkpt);
            }
            break;
        case 5:
            // Try to get snapshot (already done above, but do it again)
            {
                sqlite3_snapshot *snap_temp = NULL;
                sqlite3_snapshot_get(db, "main", &snap_temp);
                if (snap_temp) sqlite3_snapshot_free(snap_temp);
            }
            break;
        case 6:
            // Combination: checkpoint then recover
            sqlite3_wal_checkpoint(db, "main");
            sqlite3_snapshot_recover(db, "main");
            break;
        case 7:
            // Try with different database name
            sqlite3_wal_checkpoint_v2(db, NULL, SQLITE_CHECKPOINT_PASSIVE, NULL, NULL);
            break;
    }
    
    // Commit transaction
    sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
    
    // Clean up
    if (snap1) sqlite3_snapshot_free(snap1);
    
    // Try one more operation after commit
    if (Size > 1 && (Data[1] % 2 == 0)) {
        sqlite3_wal_checkpoint(db, "main");
    }
    
    // Close database
    sqlite3_close(db);
    
    return 0;
}