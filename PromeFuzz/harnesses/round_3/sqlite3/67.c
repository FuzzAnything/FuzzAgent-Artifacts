// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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

static int setup_database(sqlite3 **db, const char *filename) {
    int rc = sqlite3_open(filename, db);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "CREATE TABLE IF NOT EXISTS t1(x INTEGER)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "INSERT INTO t1 VALUES(1)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    return SQLITE_OK;
}

static void cleanup_database(sqlite3 *db, const char *filename) {
    if (db) {
        sqlite3_close(db);
    }
    remove(filename);
}

static int get_snapshot(sqlite3 *db, sqlite3_snapshot **snap) {
    sqlite3_snapshot *temp = NULL;
    int rc = sqlite3_snapshot_get(db, "main", &temp);
    if (rc == SQLITE_OK && snap) {
        *snap = temp;
    } else if (temp) {
        sqlite3_snapshot_free(temp);
    }
    return rc;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    static const char *dummy_file = "./dummy_file";
    
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap1 = NULL;
    sqlite3_snapshot *snap2 = NULL;
    int rc;
    
    if (Size < 1) return 0;
    
    rc = setup_database(&db, dummy_file);
    if (rc != SQLITE_OK) goto cleanup;
    
    uint8_t op = Data[0] % 6;
    
    switch (op) {
        case 0: {
            if (sqlite3_exec(db, "BEGIN", NULL, NULL, NULL) != SQLITE_OK) break;
            
            rc = get_snapshot(db, &snap1);
            if (rc != SQLITE_OK) {
                sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
                break;
            }
            
            rc = sqlite3_exec(db, "INSERT INTO t1 VALUES(2)", NULL, NULL, NULL);
            if (rc != SQLITE_OK) {
                if (snap1) {
                    sqlite3_snapshot_free(snap1);
                    snap1 = NULL;
                }
                sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
                break;
            }
            
            rc = get_snapshot(db, &snap2);
            if (rc == SQLITE_OK && snap1 && snap2) {
                sqlite3_snapshot_cmp(snap1, snap2);
            }
            
            if (snap1) {
                sqlite3_snapshot_free(snap1);
                snap1 = NULL;
            }
            if (snap2) {
                sqlite3_snapshot_free(snap2);
                snap2 = NULL;
            }
            sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
            break;
        }
        
        case 1: {
            if (sqlite3_exec(db, "BEGIN", NULL, NULL, NULL) != SQLITE_OK) break;
            
            rc = get_snapshot(db, &snap1);
            if (rc == SQLITE_OK && snap1) {
                sqlite3_snapshot_open(db, "main", snap1);
                sqlite3_snapshot_free(snap1);
                snap1 = NULL;
            }
            
            sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
            break;
        }
        
        case 2: {
            int pnLog, pnCkpt;
            int mode = (Data[0] % 5) + 1;
            sqlite3_wal_checkpoint_v2(db, "main", mode, &pnLog, &pnCkpt);
            break;
        }
        
        case 3: {
            sqlite3_snapshot_recover(db, "main");
            break;
        }
        
        case 4: {
            if (sqlite3_exec(db, "BEGIN", NULL, NULL, NULL) != SQLITE_OK) break;
            
            rc = get_snapshot(db, &snap1);
            if (rc == SQLITE_OK && snap1) {
                sqlite3_snapshot_free(snap1);
                snap1 = NULL;
            }
            
            sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
            break;
        }
        
        case 5: {
            if (sqlite3_exec(db, "BEGIN", NULL, NULL, NULL) != SQLITE_OK) break;
            
            rc = get_snapshot(db, &snap1);
            if (rc == SQLITE_OK && snap1) {
                rc = sqlite3_exec(db, "INSERT INTO t1 VALUES(3)", NULL, NULL, NULL);
                if (rc == SQLITE_OK) {
                    rc = get_snapshot(db, &snap2);
                    if (rc == SQLITE_OK && snap1 && snap2) {
                        int cmp = sqlite3_snapshot_cmp(snap1, snap2);
                        if (cmp < 0) {
                            sqlite3_snapshot_open(db, "main", snap1);
                        }
                    }
                }
            }
            
            if (snap1) {
                sqlite3_snapshot_free(snap1);
                snap1 = NULL;
            }
            if (snap2) {
                sqlite3_snapshot_free(snap2);
                snap2 = NULL;
            }
            sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
            break;
        }
    }
    
cleanup:
    if (snap1) {
        sqlite3_snapshot_free(snap1);
        snap1 = NULL;
    }
    if (snap2) {
        sqlite3_snapshot_free(snap2);
        snap2 = NULL;
    }
    cleanup_database(db, dummy_file);
    
    return 0;
}