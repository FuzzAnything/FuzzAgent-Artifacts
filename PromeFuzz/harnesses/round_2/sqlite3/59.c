// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_stmt_status at sqlite3.c:80342:16 in sqlite3.h
// sqlite3_stmt_scanstatus at sqlite3.c:80847:16 in sqlite3.h
// sqlite3_stmt_scanstatus_reset at sqlite3.c:80859:17 in sqlite3.h
// sqlite3_normalized_sql at sqlite3.c:80408:24 in sqlite3.h
// sqlite3_prepare_v3 at sqlite3.c:132490:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_prepare16_v2 at sqlite3.c:132600:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int fuzz_prepare_statement(sqlite3 *db, sqlite3_stmt **ppStmt, const uint8_t *Data, size_t Size) {
    const char *zSql = (const char *)Data;
    int nByte = Size > 0 ? (int)Size : 0;
    const char *pzTail = NULL;
    
    if (Size == 0) {
        zSql = "SELECT 1";
        nByte = -1;
    }
    
    return sqlite3_prepare_v2(db, zSql, nByte, ppStmt, &pzTail);
}

static void fuzz_target_functions(sqlite3 *db, sqlite3_stmt *pStmt, const uint8_t *Data, size_t Size) {
    int i;
    
    if (pStmt == NULL) return;
    
    for (i = 0; i < 10 && i < (int)Size; i++) {
        int op = Data[i] % 10;
        int resetFlg = Data[i] % 2;
        sqlite3_stmt_status(pStmt, op, resetFlg);
    }
    
#ifdef SQLITE_ENABLE_STMT_SCANSTATUS
    for (i = 0; i < 5 && i < (int)Size; i++) {
        int idx = (int)Data[i] % 5 - 1;
        int iScanStatusOp = Data[i] % 4;
        int outVal = 0;
        sqlite3_stmt_scanstatus(pStmt, idx, iScanStatusOp, &outVal);
    }
    
    sqlite3_stmt_scanstatus_reset(pStmt);
#endif
    
    const char *normSql = sqlite3_normalized_sql(pStmt);
    if (normSql) {
        volatile const char *dummy = normSql;
        (void)dummy;
    }
}

static void fuzz_prepare_functions(sqlite3 *db, const uint8_t *Data, size_t Size) {
    sqlite3_stmt *pStmt1 = NULL;
    sqlite3_stmt *pStmt2 = NULL;
    const char *pzTail1 = NULL;
    const void *pzTail2 = NULL;
    
    const char *zSql = (const char *)Data;
    int nByte = Size > 0 ? (int)Size : 0;
    
    if (Size == 0) {
        zSql = "SELECT 1";
        nByte = -1;
    }
    
    sqlite3_prepare_v3(db, zSql, nByte, 0, &pStmt1, &pzTail1);
    if (pStmt1) {
        sqlite3_finalize(pStmt1);
    }
    
    if (Size > 0) {
        size_t alloc_size = Size * 2 + 2;
        void *zSql16 = malloc(alloc_size);
        if (zSql16) {
            memset(zSql16, 0, alloc_size);
            memcpy(zSql16, Data, Size);
            sqlite3_prepare16_v2(db, zSql16, -1, &pStmt2, &pzTail2);
            free(zSql16);
        }
    } else {
        static const unsigned short static_sql[] = {'S','E','L','E','C','T',' ','1',0};
        sqlite3_prepare16_v2(db, static_sql, -1, &pStmt2, &pzTail2);
    }
    
    if (pStmt2) {
        sqlite3_finalize(pStmt2);
    }
}

int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    static sqlite3 *db = NULL;
    sqlite3_stmt *pStmt = NULL;
    
    if (!initialized) {
        if (sqlite3_initialize() != SQLITE_OK) {
            return 0;
        }
        initialized = 1;
    }
    
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a,b,c);", 0, 0, 0);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1,2,3);", 0, 0, 0);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(4,5,6);", 0, 0, 0);
    
    if (fuzz_prepare_statement(db, &pStmt, Data, Size) == SQLITE_OK) {
        fuzz_target_functions(db, pStmt, Data, Size);
        sqlite3_finalize(pStmt);
    }
    
    fuzz_prepare_functions(db, Data, Size);
    
    sqlite3_close(db);
    
    return 0;
}