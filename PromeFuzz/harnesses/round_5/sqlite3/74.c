// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:173377:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

static int wal_hook_callback(void *user_data, sqlite3 *db, const char *db_name, int pages) {
    (void)user_data;
    (void)db;
    (void)db_name;
    (void)pages;
    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *db_file = "./dummy_file";
    
    // Remove any existing database file
    remove(db_file);
    
    // Open a database connection
    rc = sqlite3_open(db_file, &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    // Enable WAL mode
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
    
    // Create a table and insert some data to ensure WAL has content
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 2);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(3, 4);", NULL, NULL, NULL);
    
    // Use fuzzer input to influence parameters
    // We'll use the first few bytes to determine which functions to call and with what parameters
    
    if (Size > 0) {
        uint8_t selector = Data[0];
        size_t data_index = 1;
        
        // sqlite3_wal_checkpoint
        if (selector & 0x01) {
            const char *zDb = "main";
            if (Size > data_index && (Data[data_index] & 0x01)) {
                zDb = NULL;  // Test with NULL
            }
            sqlite3_wal_checkpoint(db, zDb);
            data_index++;
        }
        
        // sqlite3_snapshot_get
        if ((selector & 0x02) && Size > data_index) {
            sqlite3_snapshot *snapshot = NULL;
            const char *zSchema = "main";
            if (Data[data_index] & 0x01) {
                zSchema = NULL;
            }
            
            // Begin a transaction to take database out of autocommit mode
            sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
            
            rc = sqlite3_snapshot_get(db, zSchema, &snapshot);
            if (rc == SQLITE_OK && snapshot != NULL) {
                // Free the snapshot if we got one
                sqlite3_snapshot_free(snapshot);
            }
            
            sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
            data_index++;
        }
        
        // sqlite3_wal_checkpoint_v2
        if (selector & 0x04) {
            const char *zDb = "main";
            int eMode = SQLITE_CHECKPOINT_PASSIVE;
            int pnLog = 0, pnCkpt = 0;
            
            if (Size > data_index) {
                uint8_t mode_selector = Data[data_index] % 5;
                switch (mode_selector) {
                    case 0: eMode = SQLITE_CHECKPOINT_PASSIVE; break;
                    case 1: eMode = SQLITE_CHECKPOINT_FULL; break;
                    case 2: eMode = SQLITE_CHECKPOINT_RESTART; break;
                    case 3: eMode = SQLITE_CHECKPOINT_TRUNCATE; break;
                    case 4: eMode = SQLITE_CHECKPOINT_NOOP; break;
                }
                
                if (Data[data_index] & 0x80) {
                    zDb = NULL;
                }
            }
            
            sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
            data_index++;
        }
        
        // sqlite3_wal_autocheckpoint
        if (selector & 0x08) {
            int N = 1000;  // Default
            if (Size > data_index) {
                // Use the byte value as N, but limit to reasonable range
                N = (int)Data[data_index] * 10;
            }
            sqlite3_wal_autocheckpoint(db, N);
            data_index++;
        }
        
        // sqlite3_db_cacheflush
        if (selector & 0x10) {
            sqlite3_db_cacheflush(db);
        }
        
        // sqlite3_wal_hook
        if (selector & 0x20) {
            void *user_data = NULL;
            if (Size > data_index && (Data[data_index] & 0x01)) {
                // Use a non-NULL user_data pointer
                user_data = (void*)0x1234;
            }
            
            void *prev = sqlite3_wal_hook(db, wal_hook_callback, user_data);
            (void)prev;  // Unused
            
            // Trigger the hook by doing another insert
            sqlite3_exec(db, "INSERT INTO t1 VALUES(5, 6);", NULL, NULL, NULL);
            
            // Restore default behavior
            sqlite3_wal_hook(db, NULL, NULL);
            data_index++;
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    // Remove the database file
    remove(db_file);
    remove("./dummy_file-wal");
    remove("./dummy_file-shm");
    
    return 0;
}