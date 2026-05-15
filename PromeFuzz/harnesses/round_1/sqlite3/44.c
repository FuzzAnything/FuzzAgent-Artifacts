// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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

static int setup_database(sqlite3 **db) {
    int rc = sqlite3_open("./dummy_file", db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    // Enable WAL mode for snapshot functions
    rc = sqlite3_exec(*db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    // Create a table and insert some data to ensure WAL has content
    rc = sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS t1 (a INTEGER, b TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    rc = sqlite3_exec(*db, "INSERT INTO t1 VALUES (1, 'test');", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    return SQLITE_OK;
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
    remove("./dummy_file");
}

static const char* get_db_name(const uint8_t *data, size_t size) {
    if (size == 0) return NULL;
    // Use first byte to decide database name
    switch (data[0] % 4) {
        case 0: return NULL;
        case 1: return "main";
        case 2: return "temp";
        default: return "test";
    }
}

static int get_checkpoint_mode(const uint8_t *data, size_t size) {
    if (size < 2) return SQLITE_CHECKPOINT_PASSIVE;
    switch (data[1] % 5) {
        case 0: return SQLITE_CHECKPOINT_PASSIVE;
        case 1: return SQLITE_CHECKPOINT_FULL;
        case 2: return SQLITE_CHECKPOINT_RESTART;
        case 3: return SQLITE_CHECKPOINT_TRUNCATE;
        default: return SQLITE_CHECKPOINT_NOOP;
    }
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    
    // Setup database
    if (setup_database(&db) != SQLITE_OK) {
        return 0;
    }
    
    // Use fuzz input to determine which functions to call
    if (Size > 0) {
        // Always try checkpoint first
        int pnLog, pnCkpt;
        const char *zDb = get_db_name(Data, Size);
        int eMode = get_checkpoint_mode(Data, Size);
        
        sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
        
        // Try snapshot recover if we have enough data
        if (Size > 1) {
            sqlite3_snapshot_recover(db, zDb);
        }
        
        // Try to get and use snapshots if enabled
        #ifdef SQLITE_ENABLE_SNAPSHOT
        if (Size > 2) {
            // Begin a transaction for snapshot operations
            sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
            
            sqlite3_snapshot *pSnapshot1 = NULL;
            sqlite3_snapshot *pSnapshot2 = NULL;
            
            // Try to get a snapshot
            if (sqlite3_snapshot_get(db, "main", &pSnapshot1) == SQLITE_OK && pSnapshot1) {
                // Try to open the snapshot
                sqlite3_snapshot_open(db, "main", pSnapshot1);
                
                // Try to get another snapshot for comparison
                if (Size > 3 && sqlite3_snapshot_get(db, "main", &pSnapshot2) == SQLITE_OK && pSnapshot2) {
                    // Compare snapshots
                    sqlite3_snapshot_cmp(pSnapshot1, pSnapshot2);
                    
                    // Try to open second snapshot
                    sqlite3_snapshot_open(db, "main", pSnapshot2);
                    
                    // Free second snapshot
                    sqlite3_snapshot_free(pSnapshot2);
                }
                
                // Free first snapshot
                sqlite3_snapshot_free(pSnapshot1);
            }
            
            sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
        }
        #endif
    }
    
    // Cleanup
    cleanup_database(db);
    
    return 0;
}