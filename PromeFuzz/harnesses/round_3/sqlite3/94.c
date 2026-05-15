// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:173377:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_wal_hook(void *user_data, sqlite3 *db, const char *db_name, int pages) {
    // Simple callback that returns SQLITE_OK
    (void)user_data;
    (void)db;
    (void)db_name;
    (void)pages;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_94(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *main_db = "./dummy_file";
    
    // Remove any existing dummy file
    remove(main_db);
    
    // Open database
    rc = sqlite3_open(main_db, &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable WAL mode for testing WAL-related functions
    sqlite3_exec(db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    
    // Create a table and insert some data to ensure WAL has content
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT)", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO test(data) VALUES('test data')", NULL, NULL, NULL);
    
    // Use first byte to determine which function(s) to test
    if (Size > 0) {
        uint8_t selector = Data[0];
        
        // Test sqlite3_db_cacheflush
        if (selector & 0x01) {
            sqlite3_db_cacheflush(db);
        }
        
        // Test sqlite3_wal_checkpoint_v2
        if (selector & 0x02) {
            int pnLog = 0, pnCkpt = 0;
            int modes[] = {
                SQLITE_CHECKPOINT_PASSIVE,
                SQLITE_CHECKPOINT_FULL,
                SQLITE_CHECKPOINT_RESTART,
                SQLITE_CHECKPOINT_TRUNCATE,
                SQLITE_CHECKPOINT_NOOP
            };
            int mode_idx = (Size > 1) ? (Data[1] % 5) : 0;
            const char *db_names[] = {NULL, "main", "temp"};
            int name_idx = (Size > 2) ? (Data[2] % 3) : 0;
            
            sqlite3_wal_checkpoint_v2(db, db_names[name_idx], modes[mode_idx], &pnLog, &pnCkpt);
        }
        
        // Test sqlite3_snapshot_get (if supported)
        if ((selector & 0x04) && Size > 3) {
            #ifdef SQLITE_ENABLE_SNAPSHOT
            sqlite3_snapshot *snapshot = NULL;
            const char *schemas[] = {"main", "temp"};
            int schema_idx = (Data[3] % 2);
            
            // Begin a transaction to satisfy snapshot requirements
            sqlite3_exec(db, "BEGIN TRANSACTION", NULL, NULL, NULL);
            rc = sqlite3_snapshot_get(db, schemas[schema_idx], &snapshot);
            if (rc == SQLITE_OK && snapshot != NULL) {
                sqlite3_snapshot_free(snapshot);
            }
            sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
            #endif
        }
        
        // Test sqlite3_wal_autocheckpoint
        if (selector & 0x08) {
            int N = (Size > 4) ? ((int)Data[4] - 128) : 1000; // Range from -128 to 127
            sqlite3_wal_autocheckpoint(db, N);
        }
        
        // Test sqlite3_wal_hook
        if (selector & 0x10) {
            void *user_data = (void*)(uintptr_t)(Size > 5 ? Data[5] : 0);
            sqlite3_wal_hook(db, dummy_wal_hook, user_data);
        }
        
        // Test sqlite3_wal_checkpoint
        if (selector & 0x20) {
            const char *db_names[] = {NULL, "main", "temp"};
            int name_idx = (Size > 6) ? (Data[6] % 3) : 0;
            sqlite3_wal_checkpoint(db, db_names[name_idx]);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    remove(main_db);
    
    return 0;
}