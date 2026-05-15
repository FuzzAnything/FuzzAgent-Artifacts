// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_autocheckpoint at sqlite3.c:173377:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_snapshot_free at sqlite3.c:176008:17 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_wal_hook at sqlite3.c:173398:18 in sqlite3.h
// sqlite3_wal_checkpoint at sqlite3.c:173498:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_snapshot_get at sqlite3.c:175881:16 in sqlite3.h
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

static int wal_hook_callback(void *data, sqlite3 *db, const char *db_name, int pages) {
    return SQLITE_OK;
}

static void cleanup_snapshot(sqlite3_snapshot **ppSnapshot) {
    if (*ppSnapshot) {
        sqlite3_snapshot_free(*ppSnapshot);
        *ppSnapshot = NULL;
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *dummy_file = "./dummy_file";
    FILE *fp = NULL;
    
    if (Size < 1) return 0;
    
    fp = fopen(dummy_file, "wb");
    if (!fp) return 0;
    
    fwrite(Data, 1, Size, fp);
    fclose(fp);
    
    rc = sqlite3_open(dummy_file, &db);
    if (rc != SQLITE_OK || !db) {
        remove(dummy_file);
        return 0;
    }
    
    sqlite3_exec(db, "PRAGMA journal_mode=WAL", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a,b,c)", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1,2,3)", NULL, NULL, NULL);
    
    uint8_t selector = Data[0] % 6;
    
    switch (selector) {
        case 0: {
            sqlite3_db_cacheflush(db);
            break;
        }
        case 1: {
            void *user_data = (void *)(uintptr_t)Data[1 % Size];
            sqlite3_wal_hook(db, wal_hook_callback, user_data);
            break;
        }
        case 2: {
            const char *zDb = NULL;
            if (Size > 1 && (Data[1] % 3) == 0) {
                zDb = "main";
            } else if (Size > 1 && (Data[1] % 3) == 1) {
                zDb = "temp";
            }
            sqlite3_wal_checkpoint(db, zDb);
            break;
        }
        case 3: {
            int mode = SQLITE_CHECKPOINT_PASSIVE;
            int nLog = 0, nCkpt = 0;
            const char *zDb = NULL;
            
            if (Size > 1) {
                uint8_t mode_selector = Data[1] % 5;
                switch (mode_selector) {
                    case 0: mode = SQLITE_CHECKPOINT_PASSIVE; break;
                    case 1: mode = SQLITE_CHECKPOINT_FULL; break;
                    case 2: mode = SQLITE_CHECKPOINT_RESTART; break;
                    case 3: mode = SQLITE_CHECKPOINT_TRUNCATE; break;
                    case 4: mode = SQLITE_CHECKPOINT_NOOP; break;
                }
                
                if (Size > 2 && (Data[2] % 3) == 0) {
                    zDb = "main";
                } else if (Size > 2 && (Data[2] % 3) == 1) {
                    zDb = "temp";
                }
            }
            
            sqlite3_wal_checkpoint_v2(db, zDb, mode, &nLog, &nCkpt);
            break;
        }
        case 4: {
            sqlite3_snapshot *pSnapshot = NULL;
            sqlite3_exec(db, "BEGIN", NULL, NULL, NULL);
            rc = sqlite3_snapshot_get(db, "main", &pSnapshot);
            if (rc == SQLITE_OK && pSnapshot) {
                cleanup_snapshot(&pSnapshot);
            }
            sqlite3_exec(db, "COMMIT", NULL, NULL, NULL);
            break;
        }
        case 5: {
            int N = 1000;
            if (Size > 1) {
                N = (int)Data[1] * 10;
            }
            sqlite3_wal_autocheckpoint(db, N);
            break;
        }
    }
    
    sqlite3_close(db);
    remove(dummy_file);
    return 0;
}