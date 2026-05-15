// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static void cleanup_snapshot(sqlite3_snapshot** ppSnapshot) {
    if (*ppSnapshot) {
        sqlite3_snapshot_free(*ppSnapshot);
        *ppSnapshot = NULL;
    }
}

int LLVMFuzzerTestOneInput_75(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *zDb = "main";
    
    // Remove any existing dummy file
    remove("./dummy_file");
    
    // Open database
    rc = sqlite3_open("./dummy_file", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable WAL mode (required for snapshot functions)
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
    
    // Create a table and insert some data to generate WAL entries
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(3, 4);", NULL, NULL, NULL);
    
    // Begin a transaction (required for sqlite3_snapshot_get)
    sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
    
    // Use fuzz data to influence behavior
    int mode_selector = 0;
    int param1 = 0, param2 = 0;
    if (Size >= 4) {
        mode_selector = Data[0] % 6;
        param1 = Data[1];
        param2 = Data[2];
    }
    
    // Test sqlite3_snapshot_get and sqlite3_snapshot_free
    sqlite3_snapshot *pSnapshot1 = NULL;
    sqlite3_snapshot *pSnapshot2 = NULL;
    
    if (mode_selector == 0 || mode_selector == 1) {
        rc = sqlite3_snapshot_get(db, zDb, &pSnapshot1);
        // Don't check rc - we want to test error paths too
        
        if (rc == SQLITE_OK && pSnapshot1 && (mode_selector == 1)) {
            // Try to get a second snapshot for comparison
            sqlite3_exec(db, "INSERT INTO t1 VALUES(5, 6);", NULL, NULL, NULL);
            sqlite3_snapshot_get(db, zDb, &pSnapshot2);
        }
    }
    
    // Test sqlite3_snapshot_cmp if we have two snapshots
    if (pSnapshot1 && pSnapshot2) {
        int cmp = sqlite3_snapshot_cmp(pSnapshot1, pSnapshot2);
        (void)cmp; // Use result to avoid unused variable warning
    }
    
    // Test sqlite3_snapshot_open if we have a snapshot
    if (pSnapshot1 && (mode_selector == 2 || mode_selector == 3)) {
        // Ensure we're not in autocommit mode
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        
        rc = sqlite3_snapshot_open(db, zDb, pSnapshot1);
        // Don't check rc - test both success and error paths
    }
    
    // Test sqlite3_wal_checkpoint_v2 with different modes
    if (mode_selector == 4 || mode_selector == 5) {
        int pnLog = 0, pnCkpt = 0;
        int modes[] = {
            SQLITE_CHECKPOINT_PASSIVE,
            SQLITE_CHECKPOINT_FULL,
            SQLITE_CHECKPOINT_RESTART,
            SQLITE_CHECKPOINT_TRUNCATE
        };
        int mode_idx = param1 % 4;
        
        rc = sqlite3_wal_checkpoint_v2(db, zDb, modes[mode_idx], &pnLog, &pnCkpt);
        (void)rc; // Use result to avoid unused variable warning
    }
    
    // Clean up before recovery test
    cleanup_snapshot(&pSnapshot1);
    cleanup_snapshot(&pSnapshot2);
    
    // End any open transaction
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    
    // Close and reopen database to test sqlite3_snapshot_recover
    sqlite3_close(db);
    
    // Reopen database
    rc = sqlite3_open("./dummy_file", &db);
    if (rc == SQLITE_OK && db) {
        // Test sqlite3_snapshot_recover
        rc = sqlite3_snapshot_recover(db, zDb);
        (void)rc; // Use result to avoid unused variable warning
        
        // Try to get a snapshot after recovery
        sqlite3_exec(db, "BEGIN;", NULL, NULL, NULL);
        sqlite3_snapshot *pSnapshot3 = NULL;
        sqlite3_snapshot_get(db, zDb, &pSnapshot3);
        cleanup_snapshot(&pSnapshot3);
        sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    }
    
    // Final cleanup
    if (db) {
        sqlite3_close(db);
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}