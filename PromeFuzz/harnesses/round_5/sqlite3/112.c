// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_stmt_scanstatus_v2 at sqlite3.c:80693:16 in sqlite3.h
// sqlite3_expired at sqlite3.c:78293:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_reset at sqlite3.c:78392:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_stmt_busy at sqlite3.c:80310:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_explain at sqlite3.c:80274:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int create_dummy_db(sqlite3 **db, sqlite3_stmt **stmt) {
    int rc = sqlite3_open(":memory:", db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    const char *create_table_sql = "CREATE TABLE t1 (id INTEGER PRIMARY KEY, val TEXT);";
    rc = sqlite3_exec(*db, create_table_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    const char *insert_sql = "INSERT INTO t1 (val) VALUES ('test');";
    rc = sqlite3_exec(*db, insert_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(*db);
        return rc;
    }
    
    const char *select_sql = "SELECT * FROM t1;";
    rc = sqlite3_prepare_v2(*db, select_sql, -1, stmt, NULL);
    return rc;
}

int LLVMFuzzerTestOneInput_112(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    
    if (Size < 1) {
        return 0;
    }
    
    int rc = create_dummy_db(&db, &stmt);
    if (rc != SQLITE_OK || db == NULL || stmt == NULL) {
        if (db) sqlite3_close(db);
        return 0;
    }
    
    uint8_t selector = Data[0] % 6;
    size_t data_offset = 1;
    
    switch (selector) {
        case 0: {
            sqlite3_stmt_busy(stmt);
            break;
        }
        case 1: {
            if (Size - data_offset < 2 * sizeof(int)) {
                break;
            }
            int idx = *((int*)(Data + data_offset));
            data_offset += sizeof(int);
            int iScanStatusOp = *((int*)(Data + data_offset));
            data_offset += sizeof(int);
            
            void *pOut = malloc(sizeof(sqlite3_int64));
            if (pOut) {
                sqlite3_stmt_scanstatus(stmt, idx, iScanStatusOp, pOut);
                free(pOut);
            }
            break;
        }
        case 2: {
            if (Size - data_offset < 2 * sizeof(int)) {
                break;
            }
            int op = *((int*)(Data + data_offset));
            data_offset += sizeof(int);
            int resetFlg = *((int*)(Data + data_offset));
            data_offset += sizeof(int);
            
            sqlite3_stmt_status(stmt, op, resetFlg);
            break;
        }
        case 3: {
            if (Size - data_offset < sizeof(int)) {
                break;
            }
            int eMode = *((int*)(Data + data_offset)) % 3;
            sqlite3_stmt_explain(stmt, eMode);
            break;
        }
        case 4: {
            if (Size - data_offset < 3 * sizeof(int)) {
                break;
            }
            int idx = *((int*)(Data + data_offset));
            data_offset += sizeof(int);
            int iScanStatusOp = *((int*)(Data + data_offset));
            data_offset += sizeof(int);
            int flags = *((int*)(Data + data_offset));
            data_offset += sizeof(int);
            
            void *pOut = malloc(sizeof(sqlite3_int64));
            if (pOut) {
                sqlite3_stmt_scanstatus_v2(stmt, idx, iScanStatusOp, flags, pOut);
                free(pOut);
            }
            break;
        }
        case 5: {
            sqlite3_expired(stmt);
            break;
        }
    }
    
    sqlite3_step(stmt);
    sqlite3_reset(stmt);
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    
    return 0;
}