// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_cmp at sqlite3.c:57145:16 in sqlite3.h
// sqlite3_snapshot_open at sqlite3.c:175921:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_snapshot_recover at sqlite3.c:175977:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int setup_database(sqlite3 **db) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "CREATE TABLE t1(a INTEGER, b TEXT)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "INSERT INTO t1 VALUES(1, 'test')", NULL, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(*db, "INSERT INTO t1 VALUES(2, 'test2')", NULL, NULL, NULL);
    return rc;
}

static void cleanup_snapshot(sqlite3_snapshot **snap) {
    if (*snap) {
        sqlite3_snapshot_free(*snap);
        *snap = NULL;
    }
}

static int get_snapshot(sqlite3 *db, sqlite3_snapshot **snap) {
    int rc;
    sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);
    rc = sqlite3_snapshot_get(db, "main", snap);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
    }
    return rc;
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_snapshot *snap1 = NULL;
    sqlite3_snapshot *snap2 = NULL;
    int rc;
    
    if (Size < 1) return 0;
    
    rc = setup_database(&db);
    if (rc != SQLITE_OK) goto cleanup;
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: { /* sqlite3_snapshot_cmp */
            rc = get_snapshot(db, &snap1);
            if (rc != SQLITE_OK) break;
            
            rc = get_snapshot(db, &snap2);
            if (rc != SQLITE_OK) break;
            
            if (snap1 && snap2) {
                sqlite3_snapshot_cmp(snap1, snap2);
            }
            break;
        }
        case 1: { /* sqlite3_snapshot_open */
            rc = get_snapshot(db, &snap1);
            if (rc != SQLITE_OK) break;
            
            if (snap1) {
                rc = sqlite3_snapshot_open(db, "main", snap1);
                if (rc == SQLITE_OK) {
                    sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
                }
            }
            break;
        }
        case 2: { /* sqlite3_wal_checkpoint_v2 */
            int pnLog, pnCkpt;
            int mode = Data[1] % 5;
            const char *zDb = (Data[1] & 0x80) ? NULL : "main";
            
            rc = sqlite3_wal_checkpoint_v2(db, zDb, mode, &pnLog, &pnCkpt);
            break;
        }
        case 3: { /* sqlite3_snapshot_recover */
            rc = sqlite3_snapshot_recover(db, "main");
            break;
        }
        case 4: { /* sqlite3_snapshot_get */
            rc = get_snapshot(db, &snap1);
            break;
        }
        case 5: { /* sqlite3_wal_checkpoint */
            rc = sqlite3_wal_checkpoint(db, "main");
            break;
        }
    }
    
cleanup:
    cleanup_snapshot(&snap1);
    cleanup_snapshot(&snap2);
    if (db) {
        sqlite3_exec(db, "ROLLBACK", NULL, NULL, NULL);
        sqlite3_close(db);
    }
    return 0;
}