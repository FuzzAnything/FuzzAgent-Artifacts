// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:173377:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
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

static int wal_hook_callback(void *user_data, sqlite3 *db, const char *db_name, int pages_in_wal) {
    // Simple callback that does nothing
    (void)user_data;
    (void)db;
    (void)db_name;
    (void)pages_in_wal;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *err_msg = NULL;
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    // Enable WAL mode for functions that require it
    rc = sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 0;
    }
    
    // Create a simple table and insert some data to have some database state
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a INTEGER, b TEXT);", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    
    rc = sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test');", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    
    // Use the input data to decide which functions to call and with what parameters
    if (Size > 0) {
        uint8_t selector = Data[0] % 6;
        const uint8_t *params = Data + 1;
        size_t params_size = Size - 1;
        
        switch (selector) {
            case 0: {
                // Test sqlite3_db_cacheflush
                rc = sqlite3_db_cacheflush(db);
                (void)rc; // Result is ignored for fuzzing
                break;
            }
            
            case 1: {
                // Test sqlite3_wal_hook
                void *user_data = NULL;
                if (params_size > 0) {
                    user_data = (void *)(uintptr_t)params[0];
                }
                void *prev = sqlite3_wal_hook(db, wal_hook_callback, user_data);
                (void)prev;
                break;
            }
            
            case 2: {
                // Test sqlite3_wal_checkpoint
                const char *db_name = "main";
                if (params_size > 0 && (params[0] % 2 == 0)) {
                    db_name = NULL; // Test with NULL
                }
                rc = sqlite3_wal_checkpoint(db, db_name);
                (void)rc;
                break;
            }
            
            case 3: {
                // Test sqlite3_wal_checkpoint_v2
                const char *db_name = "main";
                int mode = SQLITE_CHECKPOINT_PASSIVE;
                int pnLog = 0, pnCkpt = 0;
                
                if (params_size > 0) {
                    uint8_t mode_selector = params[0] % 5;
                    switch (mode_selector) {
                        case 0: mode = SQLITE_CHECKPOINT_PASSIVE; break;
                        case 1: mode = SQLITE_CHECKPOINT_FULL; break;
                        case 2: mode = SQLITE_CHECKPOINT_RESTART; break;
                        case 3: mode = SQLITE_CHECKPOINT_TRUNCATE; break;
                        case 4: mode = SQLITE_CHECKPOINT_NOOP; break;
                    }
                    
                    if (params_size > 1 && (params[1] % 2 == 0)) {
                        db_name = NULL;
                    }
                }
                
                rc = sqlite3_wal_checkpoint_v2(db, db_name, mode, &pnLog, &pnCkpt);
                (void)rc;
                (void)pnLog;
                (void)pnCkpt;
                break;
            }
            
            case 4: {
                // Test sqlite3_snapshot_get (if compiled with SQLITE_ENABLE_SNAPSHOT)
                #ifdef SQLITE_ENABLE_SNAPSHOT
                sqlite3_snapshot *snapshot = NULL;
                const char *schema = "main";
                
                // Begin a transaction (required for snapshot_get)
                rc = sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, &err_msg);
                if (rc == SQLITE_OK) {
                    rc = sqlite3_snapshot_get(db, schema, &snapshot);
                    if (rc == SQLITE_OK && snapshot != NULL) {
                        sqlite3_snapshot_free(snapshot);
                    }
                    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
                }
                if (err_msg) sqlite3_free(err_msg);
                #endif
                break;
            }
            
            case 5: {
                // Test sqlite3_wal_autocheckpoint
                int N = 1000; // Default
                if (params_size > 0) {
                    // Use first byte as N value (cast to signed, but limit range)
                    N = (int)((int8_t)params[0]);
                }
                rc = sqlite3_wal_autocheckpoint(db, N);
                (void)rc;
                break;
            }
        }
    }
    
    // Cleanup
    sqlite3_close(db);
    return 0;
}