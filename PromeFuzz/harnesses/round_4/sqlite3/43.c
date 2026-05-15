// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_get_clientdata at sqlite3.c:174734:18 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void dummy_destructor(void *p) {
    (void)p;
}

static int dummy_xCreate(sqlite3 *db, void *pAux,
                         int argc, const char *const *argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVtab; (void)pzErr;
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux,
                          int argc, const char *const *argv,
                          sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVtab; (void)pzErr;
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab; (void)pIdxInfo;
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab; (void)ppCursor;
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum,
                         const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor,
                         sqlite3_context *ctx, int i) {
    (void)pCursor; (void)ctx; (void)i;
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor; (void)pRowid;
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVtab, int argc,
                         sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVtab; (void)argc; (void)argv; (void)pRowid;
    return SQLITE_OK;
}

static int dummy_xBegin(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg,
                               const char *zName,
                               void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                               void **ppArg) {
    (void)pVtab; (void)nArg; (void)zName; (void)pxFunc; (void)ppArg;
    return 0;
}

static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    (void)pVtab; (void)zNew;
    return SQLITE_OK;
}

static int dummy_xSavepoint(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab; (void)iSavepoint;
    return SQLITE_OK;
}

static int dummy_xRelease(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab; (void)iSavepoint;
    return SQLITE_OK;
}

static int dummy_xRollbackTo(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab; (void)iSavepoint;
    return SQLITE_OK;
}

static const sqlite3_module dummy_module = {
    3,                          /* iVersion */
    dummy_xCreate,              /* xCreate */
    dummy_xConnect,             /* xConnect */
    dummy_xBestIndex,           /* xBestIndex */
    dummy_xDisconnect,          /* xDisconnect */
    dummy_xDestroy,             /* xDestroy */
    dummy_xOpen,                /* xOpen */
    dummy_xClose,               /* xClose */
    dummy_xFilter,              /* xFilter */
    dummy_xNext,                /* xNext */
    dummy_xEof,                 /* xEof */
    dummy_xColumn,              /* xColumn */
    dummy_xRowid,               /* xRowid */
    dummy_xUpdate,              /* xUpdate */
    dummy_xBegin,               /* xBegin */
    dummy_xSync,                /* xSync */
    dummy_xCommit,              /* xCommit */
    dummy_xRollback,            /* xRollback */
    dummy_xFindFunction,        /* xFindFunction */
    dummy_xRename,              /* xRename */
    dummy_xSavepoint,           /* xSavepoint */
    dummy_xRelease,             /* xRelease */
    dummy_xRollbackTo           /* xRollbackTo */
};

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    if (Size < 1) return 0;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    char *module_name = NULL;
    char *sql_stmt = NULL;
    char *func_name = NULL;
    char *clientdata_name = NULL;
    
    size_t offset = 0;
    
    if (offset < Size) {
        size_t len = Size - offset;
        if (len > 100) len = 100;
        module_name = (char *)malloc(len + 1);
        if (module_name) {
            memcpy(module_name, Data + offset, len);
            module_name[len] = '\0';
            offset += len;
        }
    }
    
    if (offset < Size) {
        size_t len = Size - offset;
        if (len > 200) len = 200;
        sql_stmt = (char *)malloc(len + 1);
        if (sql_stmt) {
            memcpy(sql_stmt, Data + offset, len);
            sql_stmt[len] = '\0';
            offset += len;
        }
    }
    
    if (offset < Size) {
        size_t len = Size - offset;
        if (len > 50) len = 50;
        func_name = (char *)malloc(len + 1);
        if (func_name) {
            memcpy(func_name, Data + offset, len);
            func_name[len] = '\0';
            offset += len;
        }
    }
    
    if (offset < Size) {
        size_t len = Size - offset;
        if (len > 50) len = 50;
        clientdata_name = (char *)malloc(len + 1);
        if (clientdata_name) {
            memcpy(clientdata_name, Data + offset, len);
            clientdata_name[len] = '\0';
        }
    }
    
    int nArg = (Size > 0) ? (Data[0] % 10) : 1;
    
    if (module_name) {
        rc = sqlite3_create_module(db, module_name, &dummy_module, NULL);
        (void)rc;
        
        rc = sqlite3_create_module_v2(db, module_name, &dummy_module, NULL, dummy_destructor);
        (void)rc;
        
        const char *keep_modules[] = {module_name, NULL};
        rc = sqlite3_drop_modules(db, keep_modules);
        (void)rc;
        
        rc = sqlite3_drop_modules(db, NULL);
        (void)rc;
    }
    
    if (sql_stmt) {
        rc = sqlite3_declare_vtab(db, sql_stmt);
        (void)rc;
    }
    
    if (func_name) {
        rc = sqlite3_overload_function(db, func_name, nArg);
        (void)rc;
    }
    
    if (clientdata_name) {
        void *clientdata = sqlite3_get_clientdata(db, clientdata_name);
        (void)clientdata;
    }
    
    free(module_name);
    free(sql_stmt);
    free(func_name);
    free(clientdata_name);
    
    sqlite3_close(db);
    
    return 0;
}