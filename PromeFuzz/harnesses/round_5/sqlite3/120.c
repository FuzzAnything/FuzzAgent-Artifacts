// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_busy_timeout at sqlite3.c:172724:16 in sqlite3.h
// sqlite3_busy_handler at sqlite3.c:172667:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_setlk_timeout at sqlite3.c:172744:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_db_cacheflush at sqlite3.c:171810:16 in sqlite3.h
// sqlite3_db_status at sqlite3.c:11033:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_wal_checkpoint_v2 at sqlite3.c:173428:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int busy_handler_callback(void *data, int count) {
    if (count < 5) {
        return 1;
    }
    return 0;
}

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    const char *db_file = "./dummy_file";
    remove(db_file);
    
    rc = sqlite3_open(db_file, &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", NULL, NULL, NULL);
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER PRIMARY KEY, data TEXT);", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO test(data) VALUES('test data');", NULL, NULL, NULL);
    
    if (Size >= 1) {
        int timeout = (int)Data[0];
        sqlite3_busy_timeout(db, timeout);
        sqlite3_busy_timeout(db, -1);
        sqlite3_busy_timeout(db, 0);
    }
    
    if (Size >= 2) {
        int checkpoint_mode = Data[1] % 5;
        int nLog = 0, nCkpt = 0;
        sqlite3_wal_checkpoint_v2(db, "main", checkpoint_mode, &nLog, &nCkpt);
        sqlite3_wal_checkpoint_v2(db, NULL, checkpoint_mode, &nLog, &nCkpt);
        sqlite3_wal_checkpoint_v2(db, "invalid_db", checkpoint_mode, &nLog, &nCkpt);
    }
    
    if (Size >= 3) {
        sqlite3_busy_handler(db, busy_handler_callback, NULL);
        sqlite3_busy_timeout(db, 100);
        sqlite3_busy_handler(db, busy_handler_callback, db);
    }
    
    if (Size >= 4) {
        sqlite3_db_cacheflush(db);
        sqlite3_db_cacheflush(db);
    }
    
    if (Size >= 8) {
        int current = 0, highwater = 0;
        for (int op = 0; op < 10; op++) {
            int resetFlag = (Data[4] + op) % 2;
            sqlite3_db_status(db, op, &current, &highwater, resetFlag);
        }
        sqlite3_db_status(db, SQLITE_DBSTATUS_LOOKASIDE_USED, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_SCHEMA_USED, &current, &highwater, 0);
        sqlite3_db_status(db, SQLITE_DBSTATUS_STMT_USED, &current, &highwater, 0);
    }
    
    if (Size >= 10) {
        int setlk_timeout = (int)Data[5];
        int flags = Data[6] % 2;
        sqlite3_setlk_timeout(db, setlk_timeout, flags);
        sqlite3_setlk_timeout(db, 0, flags);
        sqlite3_setlk_timeout(db, 100, flags);
        sqlite3_setlk_timeout(db, -1, flags);
    }
    
    sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO test(data) VALUES('more data');", NULL, NULL, NULL);
    sqlite3_db_cacheflush(db);
    int current = 0, highwater = 0;
    sqlite3_db_status(db, SQLITE_DBSTATUS_CACHE_USED, &current, &highwater, 0);
    sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    
    if (Size >= 12) {
        const char *attach_file = "./dummy_file2";
        remove(attach_file);
        char attach_sql[256];
        snprintf(attach_sql, sizeof(attach_sql), "ATTACH DATABASE '%s' AS attached;", attach_file);
        sqlite3_exec(db, attach_sql, NULL, NULL, NULL);
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS attached.test2(id INTEGER);", NULL, NULL, NULL);
        int nLog = 0, nCkpt = 0;
        sqlite3_wal_checkpoint_v2(db, "attached", SQLITE_CHECKPOINT_PASSIVE, &nLog, &nCkpt);
        remove(attach_file);
    }
    
    sqlite3_close(db);
    remove(db_file);
    
    return 0;
}

#ifdef __cplusplus
}
#endif