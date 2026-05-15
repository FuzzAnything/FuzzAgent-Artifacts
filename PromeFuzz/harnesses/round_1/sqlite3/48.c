// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_depth at sqlite3.c:80573:16 in sqlite3.h
// sqlite3_preupdate_count at sqlite3.c:80550:16 in sqlite3.h
// sqlite3_preupdate_blobwrite at sqlite3.c:80589:16 in sqlite3.h
// sqlite3_preupdate_old at sqlite3.c:80445:16 in sqlite3.h
// sqlite3_preupdate_new at sqlite3.c:80605:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_preupdate_hook at sqlite3.c:173294:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:80061:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:79985:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

    if (db == NULL) return;

    int depth = sqlite3_preupdate_depth(db);
    int count = sqlite3_preupdate_count(db);
    int blobwrite = sqlite3_preupdate_blobwrite(db);

    if (count > 0) {
        for (int i = 0; i < count && i < 10; i++) {
            sqlite3_value *old_val = NULL;
            sqlite3_value *new_val = NULL;
            
            if (op == SQLITE_UPDATE || op == SQLITE_DELETE) {
                sqlite3_preupdate_old(db, i, &old_val);
            }
            if (op == SQLITE_INSERT || op == SQLITE_UPDATE) {
                sqlite3_preupdate_new(db, i, &new_val);
            }
        }
    }
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }

    void *old_hook = sqlite3_preupdate_hook(db, preupdate_callback, NULL);

    rc = sqlite3_exec(db, "CREATE TABLE t1(id INTEGER PRIMARY KEY, a TEXT, b BLOB);", NULL, NULL, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_stmt *stmt = NULL;
        
        const uint8_t *input = Data;
        size_t remaining = Size;
        
        while (remaining > 0) {
            int op_type = 0;
            if (remaining >= 1) {
                op_type = *input % 3;
                input++;
                remaining--;
            } else {
                break;
            }
            
            if (op_type == 0) {
                sqlite3_prepare_v2(db, "INSERT INTO t1(a, b) VALUES(?, ?);", -1, &stmt, NULL);
                if (stmt) {
                    size_t len1 = 0;
                    if (remaining >= 1) {
                        len1 = *input % 64;
                        input++;
                        remaining--;
                    }
                    
                    len1 = len1 < remaining ? len1 : remaining;
                    const char *text_data = len1 > 0 ? (const char*)input : "";
                    sqlite3_bind_text(stmt, 1, text_data, len1, SQLITE_TRANSIENT);
                    input += len1;
                    remaining -= len1;
                    
                    size_t len2 = 0;
                    if (remaining >= 1) {
                        len2 = *input % 64;
                        input++;
                        remaining--;
                    }
                    
                    len2 = len2 < remaining ? len2 : remaining;
                    const void *blob_data = len2 > 0 ? input : "";
                    sqlite3_bind_blob(stmt, 2, blob_data, len2, SQLITE_TRANSIENT);
                    input += len2;
                    remaining -= len2;
                    
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            } else if (op_type == 1) {
                sqlite3_prepare_v2(db, "UPDATE t1 SET a = 'updated' WHERE id = 1;", -1, &stmt, NULL);
                if (stmt) {
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            } else if (op_type == 2) {
                sqlite3_prepare_v2(db, "DELETE FROM t1 WHERE id = 1;", -1, &stmt, NULL);
                if (stmt) {
                    sqlite3_step(stmt);
                    sqlite3_finalize(stmt);
                }
            }
        }
        
        sqlite3_exec(db, "DROP TABLE t1;", NULL, NULL, NULL);
    }

    sqlite3_preupdate_hook(db, old_hook, NULL);
    sqlite3_close(db);
    return 0;
}