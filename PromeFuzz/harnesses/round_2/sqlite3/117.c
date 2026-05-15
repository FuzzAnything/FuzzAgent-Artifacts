// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:80007:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
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

static void preupdate_callback(
    void *pCtx,
    sqlite3 *db,
    int op,
    char const *zDb,
    char const *zName,
    sqlite3_int64 iKey1,
    sqlite3_int64 iKey2
) {
    (void)pCtx;
    (void)zDb;
    (void)zName;
    (void)iKey1;
    (void)iKey2;
    
    int depth = sqlite3_preupdate_depth(db);
    int count = sqlite3_preupdate_count(db);
    int blobwrite = sqlite3_preupdate_blobwrite(db);
    
    (void)depth;
    (void)count;
    (void)blobwrite;
    
    if (count > 0) {
        sqlite3_value *val = NULL;
        for (int i = 0; i < count; i++) {
            if (op == SQLITE_UPDATE || op == SQLITE_DELETE) {
                sqlite3_preupdate_old(db, i, &val);
            }
            if (op == SQLITE_UPDATE || op == SQLITE_INSERT) {
                sqlite3_preupdate_new(db, i, &val);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    static sqlite3_stmt *stmt = NULL;
    
    if (!initialized) {
        if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
            return 0;
        }
        
        const char *create_table = 
            "CREATE TABLE IF NOT EXISTS test ("
            "id INTEGER PRIMARY KEY, "
            "name TEXT, "
            "value BLOB, "
            "num REAL);";
        
        if (sqlite3_exec(db, create_table, NULL, NULL, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        const char *insert_sql = "INSERT INTO test (name, value, num) VALUES (?, ?, ?);";
        if (sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL) != SQLITE_OK) {
            sqlite3_close(db);
            return 0;
        }
        
        initialized = 1;
    }
    
    if (db == NULL) {
        return 0;
    }
    
    void *old_hook = sqlite3_preupdate_hook(db, preupdate_callback, NULL);
    
    int depth = sqlite3_preupdate_depth(db);
    int count = sqlite3_preupdate_count(db);
    int blobwrite = sqlite3_preupdate_blobwrite(db);
    
    (void)depth;
    (void)count;
    (void)blobwrite;
    
    sqlite3_value *val = NULL;
    sqlite3_preupdate_old(db, 0, &val);
    sqlite3_preupdate_new(db, 0, &val);
    
    if (Size > 0) {
        const char *name = "test";
        double num = 3.14;
        
        sqlite3_bind_text(stmt, 1, name, -1, SQLITE_STATIC);
        sqlite3_bind_blob(stmt, 2, Data, (int)(Size > 1024 ? 1024 : Size), SQLITE_STATIC);
        sqlite3_bind_double(stmt, 3, num);
        
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
        
        if (Size > 10) {
            const char *update_sql = "UPDATE test SET name = ?, value = ? WHERE id = 1;";
            sqlite3_stmt *update_stmt = NULL;
            if (sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, NULL) == SQLITE_OK) {
                sqlite3_bind_text(update_stmt, 1, "updated", -1, SQLITE_STATIC);
                sqlite3_bind_blob(update_stmt, 2, Data, (int)(Size > 512 ? 512 : Size), SQLITE_STATIC);
                sqlite3_step(update_stmt);
                sqlite3_finalize(update_stmt);
            }
            
            const char *delete_sql = "DELETE FROM test WHERE id = 1;";
            sqlite3_exec(db, delete_sql, NULL, NULL, NULL);
        }
    }
    
    sqlite3_preupdate_hook(db, (void(*)(void*, sqlite3*, int, const char*, const char*, sqlite3_int64, sqlite3_int64))old_hook, NULL);
    
    return 0;
}