// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
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
    int rc;
    char *err_msg = NULL;
    
    rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    rc = sqlite3_exec(*db, "PRAGMA journal_mode=WAL;", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        return rc;
    }
    
    rc = sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS t1 (id INTEGER PRIMARY KEY, data TEXT);", 
                     NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        return rc;
    }
    
    rc = sqlite3_exec(*db, "INSERT INTO t1 (data) VALUES ('test');", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        return rc;
    }
    
    rc = sqlite3_exec(*db, "BEGIN TRANSACTION;", NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        sqlite3_close(*db);
        return rc;
    }
    
    return SQLITE_OK;
}

static void cleanup_database(sqlite3 *db) {
    char *err_msg = NULL;
    sqlite3_exec(db, "COMMIT;", NULL, NULL, &err_msg);
    if (err_msg) sqlite3_free(err_msg);
    sqlite3_close(db);
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snapshot1 = NULL;
    sqlite3_snapshot *snapshot2 = NULL;
    sqlite3_snapshot **ppSnapshot = NULL;
    int rc;
    char *err_msg = NULL;
    
    if (Size < 1) return 0;
    
    rc = setup_database(&db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    uint8_t op = Data[0] % 6;
    size_t offset = 1;
    
    switch (op) {
        case 0: {
            if (Size - offset < 1) break;
            uint8_t schema_choice = Data[offset] % 3;
            const char *zSchema = "main";
            if (schema_choice == 1) zSchema = "temp";
            else if (schema_choice == 2) zSchema = NULL;
            
            rc = sqlite3_snapshot_get(db, zSchema, &snapshot1);
            if (rc == SQLITE_OK && snapshot1) {
                sqlite3_snapshot_free(snapshot1);
            }
            break;
        }
        
        case 1: {
            if (Size - offset < 2) break;
            uint8_t schema_choice = Data[offset] % 3;
            const char *zSchema = "main";
            if (schema_choice == 1) zSchema = "temp";
            else if (schema_choice == 2) zSchema = NULL;
            
            rc = sqlite3_snapshot_get(db, zSchema, &snapshot1);
            if (rc == SQLITE_OK && snapshot1) {
                rc = sqlite3_snapshot_open(db, zSchema, snapshot1);
                sqlite3_snapshot_free(snapshot1);
            }
            break;
        }
        
        case 2: {
            if (Size - offset < 1) break;
            uint8_t db_choice = Data[offset] % 3;
            const char *zDb = "main";
            if (db_choice == 1) zDb = "temp";
            else if (db_choice == 2) zDb = NULL;
            
            rc = sqlite3_snapshot_recover(db, zDb);
            break;
        }
        
        case 3: {
            if (Size - offset < 1) break;
            uint8_t db_choice = Data[offset] % 3;
            const char *zDb = "main";
            if (db_choice == 1) zDb = "temp";
            else if (db_choice == 2) zDb = NULL;
            
            rc = sqlite3_wal_checkpoint(db, zDb);
            break;
        }
        
        case 4: {
            if (Size - offset < 2) break;
            uint8_t schema_choice1 = Data[offset] % 3;
            uint8_t schema_choice2 = Data[offset + 1] % 3;
            const char *zSchema1 = "main";
            const char *zSchema2 = "main";
            if (schema_choice1 == 1) zSchema1 = "temp";
            else if (schema_choice1 == 2) zSchema1 = NULL;
            if (schema_choice2 == 1) zSchema2 = "temp";
            else if (schema_choice2 == 2) zSchema2 = NULL;
            
            rc = sqlite3_snapshot_get(db, zSchema1, &snapshot1);
            if (rc == SQLITE_OK && snapshot1) {
                rc = sqlite3_snapshot_get(db, zSchema2, &snapshot2);
                if (rc == SQLITE_OK && snapshot2) {
                    int cmp = sqlite3_snapshot_cmp(snapshot1, snapshot2);
                    (void)cmp;
                    sqlite3_snapshot_free(snapshot2);
                }
                sqlite3_snapshot_free(snapshot1);
            }
            break;
        }
        
        case 5: {
            if (Size - offset < 3) break;
            uint8_t db_choice = Data[offset] % 3;
            uint8_t mode = Data[offset + 1] % 5;
            const char *zDb = "main";
            if (db_choice == 1) zDb = "temp";
            else if (db_choice == 2) zDb = NULL;
            
            int pnLog, pnCkpt;
            rc = sqlite3_wal_checkpoint_v2(db, zDb, mode, &pnLog, &pnCkpt);
            break;
        }
    }
    
    cleanup_database(db);
    return 0;
}