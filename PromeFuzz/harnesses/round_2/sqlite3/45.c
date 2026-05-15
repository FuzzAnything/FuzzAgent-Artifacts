// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_db_filename at sqlite3.c:175848:24 in sqlite3.h
// sqlite3_filename_wal at sqlite3.c:175807:24 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_db_readonly at sqlite3.c:175864:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static void initialize_database(sqlite3 **db) {
    int rc;
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        *db = NULL;
        return;
    }
    
    // Enable WAL mode for snapshot functionality
    sqlite3_exec(*db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    
    // Create a table and insert some data to have a valid database
    sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT)", NULL, NULL, NULL);
    sqlite3_exec(*db, "INSERT OR IGNORE INTO test VALUES(1, 'test')", NULL, NULL, NULL);
    
    // Ensure we're not in autocommit mode for snapshot operations
    sqlite3_exec(*db, "BEGIN TRANSACTION", NULL, NULL, NULL);
}

static void cleanup_database(sqlite3 *db) {
    if (db) {
        sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
        sqlite3_close(db);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    // Initialize database
    initialize_database(&db);
    if (!db) {
        return 0;
    }
    
    // Use the fuzzer input to influence behavior
    // We'll use the first few bytes to determine which functions to call and with what parameters
    
    if (Size == 0) {
        cleanup_database(db);
        return 0;
    }
    
    // Extract some parameters from the fuzzer input
    uint8_t func_selector = Data[0] % 6;  // Which function to test
    uint8_t mode_selector = (Size > 1) ? Data[1] : 0;
    uint8_t db_selector = (Size > 2) ? Data[2] : 0;
    
    const char *db_name = "main";
    if (db_selector % 3 == 1) {
        db_name = "temp";
    } else if (db_selector % 3 == 2) {
        // Use fuzzer data as database name (limited to safe length)
        char custom_db[32];
        size_t len = (Size > 35) ? 31 : (Size - 4);
        if (len > 0) {
            memcpy(custom_db, Data + 4, len);
            custom_db[len] = '\0';
            db_name = custom_db;
        }
    }
    
    switch (func_selector) {
        case 0: {
            // Test sqlite3_snapshot_open
            // First get a snapshot if possible
            sqlite3_snapshot *snap = NULL;
            rc = sqlite3_snapshot_get(db, "main", &snap);
            if (rc == SQLITE_OK && snap) {
                // Try to open the snapshot
                rc = sqlite3_snapshot_open(db, "main", snap);
                // Clean up
                sqlite3_snapshot_free(snap);
            }
            break;
        }
        
        case 1: {
            // Test sqlite3_db_readonly
            rc = sqlite3_db_readonly(db, db_name);
            break;
        }
        
        case 2: {
            // Test sqlite3_wal_checkpoint
            rc = sqlite3_wal_checkpoint(db, db_name);
            break;
        }
        
        case 3: {
            // Test sqlite3_snapshot_recover
            rc = sqlite3_snapshot_recover(db, db_name);
            break;
        }
        
        case 4: {
            // Test sqlite3_filename_wal
            // First get the database filename
            const char *filename = sqlite3_db_filename(db, db_name);
            if (filename) {
                const char *wal_name = sqlite3_filename_wal(filename);
                (void)wal_name;  // Use result to avoid unused variable warning
            }
            rc = SQLITE_OK;
            break;
        }
        
        case 5: {
            // Test sqlite3_wal_checkpoint_v2
            int pnLog = 0, pnCkpt = 0;
            int modes[] = {
                SQLITE_CHECKPOINT_PASSIVE,
                SQLITE_CHECKPOINT_FULL,
                SQLITE_CHECKPOINT_RESTART,
                SQLITE_CHECKPOINT_TRUNCATE,
                SQLITE_CHECKPOINT_NOOP
            };
            int mode = modes[mode_selector % 5];
            
            rc = sqlite3_wal_checkpoint_v2(db, db_name, mode, &pnLog, &pnCkpt);
            break;
        }
    }
    
    // Also test some combinations
    if (Size > 10) {
        // Try checkpoint then snapshot recover
        if ((Data[3] & 0x1) && func_selector != 2 && func_selector != 3) {
            sqlite3_wal_checkpoint(db, "main");
            sqlite3_snapshot_recover(db, "main");
        }
        
        // Try readonly check on multiple databases
        if ((Data[3] & 0x2) && func_selector != 1) {
            sqlite3_db_readonly(db, "main");
            sqlite3_db_readonly(db, "temp");
        }
    }
    
    cleanup_database(db);
    return 0;
}