// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    // This is a dummy implementation that allows calling sqlite3_vtab_in
    // and sqlite3_vtab_rhs_value in a valid context
    if (pIdxInfo->nConstraint > 0) {
        // Try to call sqlite3_vtab_in with different bHandle values
        for (int i = 0; i < pIdxInfo->nConstraint && i < 3; i++) {
            sqlite3_vtab_in(pIdxInfo, i, -1);  // Check feasibility
            sqlite3_vtab_in(pIdxInfo, i, 0);   // Set preference (no batch)
            sqlite3_vtab_in(pIdxInfo, i, 1);   // Set preference (batch)
        }
        
        // Try to call sqlite3_vtab_rhs_value
        for (int i = 0; i < pIdxInfo->nConstraint && i < 3; i++) {
            sqlite3_value *ppVal = NULL;
            sqlite3_vtab_rhs_value(pIdxInfo, i, &ppVal);
        }
    }
    
    pIdxInfo->estimatedCost = 1000000.0;
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
    // This is a dummy implementation that allows calling sqlite3_vtab_in_first
    // and sqlite3_vtab_in_next in a valid context
    if (argc > 0) {
        sqlite3_value *pVal = argv[0];
        sqlite3_value *ppOut = NULL;
        
        // Try to iterate through IN values if available
        int rc = sqlite3_vtab_in_first(pVal, &ppOut);
        while (rc == SQLITE_OK && ppOut != NULL) {
            rc = sqlite3_vtab_in_next(pVal, &ppOut);
        }
    }
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    *ppVtab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = (sqlite3_vtab_cursor *)sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    sqlite3_free(pCursor);
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    0,                          /* iVersion */
    dummy_xConnect,             /* xCreate */
    dummy_xConnect,             /* xConnect */
    dummy_xBestIndex,           /* xBestIndex */
    dummy_xDisconnect,          /* xDisconnect */
    dummy_xDisconnect,          /* xDestroy */
    dummy_xOpen,                /* xOpen */
    dummy_xClose,               /* xClose */
    dummy_xFilter,              /* xFilter */
    dummy_xNext,                /* xNext */
    dummy_xEof,                 /* xEof */
    dummy_xColumn,              /* xColumn */
    dummy_xRowid,               /* xRowid */
    NULL,                       /* xUpdate */
    NULL,                       /* xBegin */
    NULL,                       /* xSync */
    NULL,                       /* xCommit */
    NULL,                       /* xRollback */
    NULL,                       /* xFindFunction */
    NULL,                       /* xRename */
    NULL,                       /* xSavepoint */
    NULL,                       /* xRelease */
    NULL,                       /* xRollbackTo */
    NULL,                       /* xShadowName */
};

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Create a dummy virtual table to provide valid context for the functions
    rc = sqlite3_create_module(db, "dummy_vtab", &dummy_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    // Execute a dummy CREATE VIRTUAL TABLE statement to initialize the module
    char *errmsg = NULL;
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE temp.dummy USING dummy_vtab", 
                      NULL, NULL, &errmsg);
    if (rc != SQLITE_OK && errmsg) {
        sqlite3_free(errmsg);
    }
    
    size_t offset = 0;
    
    for (int i = 0; i < 6 && offset < Size; i++) {
        switch (Data[offset++ % Size]) {
            case 0: {
                if (offset + 2 < Size) {
                    int op = Data[offset % Size] % 10;  // Limit to valid op values
                    offset++;
                    int arg = Data[offset % Size];
                    offset++;
                    sqlite3_vtab_config(db, op, arg);
                }
                break;
            }
            
            case 1: {
                if (offset + 1 < Size) {
                    const char *func_name = "fuzz_func";
                    int nArg = Data[offset % Size] % 10;  // Limit to reasonable arg count
                    offset++;
                    sqlite3_overload_function(db, func_name, nArg);
                }
                break;
            }
            
            default:
                offset++;
                break;
        }
        
        if (offset >= Size) offset = 0;
    }
    
    // Clean up
    sqlite3_exec(db, "DROP TABLE IF EXISTS temp.dummy", NULL, NULL, NULL);
    sqlite3_close(db);
    return 0;
}