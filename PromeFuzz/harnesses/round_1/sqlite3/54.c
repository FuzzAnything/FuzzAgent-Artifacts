// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_db_config at sqlite3.c:171839:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_busy_handler(void *arg, int count) {
    (void)arg;
    (void)count;
    return 0;
}

int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const char *dummy_file = "./dummy_file";
    FILE *fp = NULL;
    
    if (Size == 0) {
        return 0;
    }
    
    fp = fopen(dummy_file, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
    }
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        remove(dummy_file);
        return 0;
    }
    
    if (Size > 0) {
        void *busy_arg = (void*)(intptr_t)Data[0];
        sqlite3_busy_handler(db, dummy_busy_handler, busy_arg);
    }
    
    if (Size > 1) {
        sqlite3_db_cacheflush(db);
    }
    
    if (Size > 4) {
        int op = Data[1] % 10;
        
        switch (op) {
            case 0:
                sqlite3_db_config(db, SQLITE_DBCONFIG_LOOKASIDE, NULL, 0, 0);
                break;
            case 1:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_FKEY, 1, 0);
                break;
            case 2:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_TRIGGER, 1, 0);
                break;
            case 3:
                sqlite3_db_config(db, SQLITE_DBCONFIG_ENABLE_VIEW, 1, 0);
                break;
            case 4:
                sqlite3_db_config(db, SQLITE_DBCONFIG_NO_CKPT_ON_CLOSE, 1, 0);
                break;
            default:
                sqlite3_db_config(db, op, Data[2], Data[3], Data[4]);
                break;
        }
    }
    
    if (Size > 5) {
        int timeout = (int)Data[5];
        if (Size > 6) {
            timeout = (timeout << 8) | Data[6];
        }
        sqlite3_busy_timeout(db, timeout);
    }
    
    if (Size > 8) {
        int pnLog = 0, pnCkpt = 0;
        const char *zDb = NULL;
        
        if ((Data[7] & 0x01) == 0) {
            zDb = "main";
        } else if ((Data[7] & 0x02) == 0) {
            zDb = "temp";
        }
        
        int eMode = Data[8] % 5;
        sqlite3_wal_checkpoint_v2(db, zDb, eMode, &pnLog, &pnCkpt);
    }
    
    if (Size > 10) {
        int timeout = (int)Data[9];
        int flags = Data[10] & 0x01;
        sqlite3_setlk_timeout(db, timeout, flags);
    }
    
    sqlite3_close(db);
    remove(dummy_file);
    
    return 0;
}