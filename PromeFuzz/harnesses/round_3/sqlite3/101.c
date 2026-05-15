// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

static int dummy_xCreate(sqlite3 *db, void *pAux,
                         int argc, const char *const *argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux,
                          int argc, const char *const *argv,
                          sqlite3_vtab **ppVtab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *info) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum,
                         const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor,
                         sqlite3_context *ctx, int i) {
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVtab, int argc,
                         sqlite3_value **argv, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg,
                               const char *zName,
                               void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                               void **ppArg) {
    return 0;
}

static int dummy_xBegin(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static int dummy_xSavepoint(sqlite3_vtab *pVtab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xRelease(sqlite3_vtab *pVtab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xRollbackTo(sqlite3_vtab *pVtab, int iSavepoint) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
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

int LLVMFuzzerTestOneInput_101(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    char **pazResult = NULL;
    int nRow = 0, nColumn = 0;
    
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    size_t name_len = Size / 4;
    if (name_len > 100) name_len = 100;
    if (name_len < 1) name_len = 1;
    
    char *module_name = (char *)malloc(name_len + 1);
    if (!module_name) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(module_name, Data, name_len);
    module_name[name_len] = '\0';
    
    size_t sql_len = Size - name_len;
    if (sql_len > 200) sql_len = 200;
    char *sql_stmt = (char *)malloc(sql_len + 1);
    if (!sql_stmt) {
        free(module_name);
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_stmt, Data + name_len, sql_len);
    sql_stmt[sql_len] = '\0';
    
    size_t func_len = Size / 3;
    if (func_len > 50) func_len = 50;
    if (func_len < 1) func_len = 1;
    char *func_name = (char *)malloc(func_len + 1);
    if (!func_name) {
        free(module_name);
        free(sql_stmt);
        sqlite3_close(db);
        return 0;
    }
    memcpy(func_name, Data, func_len);
    func_name[func_len] = '\0';
    
    int op = Data[0] % 6;
    
    switch (op) {
        case 0:
            rc = sqlite3_create_module(db, module_name, &dummy_module, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_create_module(db, module_name, NULL, NULL);
            }
            break;
            
        case 1:
            rc = sqlite3_declare_vtab(db, sql_stmt);
            break;
            
        case 2: {
            const char *keep_list[] = {module_name, NULL};
            rc = sqlite3_drop_modules(db, keep_list);
            rc = sqlite3_drop_modules(db, NULL);
            break;
        }
            
        case 3:
            rc = sqlite3_get_table(db, sql_stmt, &pazResult, &nRow, &nColumn, &zErrMsg);
            if (pazResult) {
                sqlite3_free_table(pazResult);
                pazResult = NULL;
            }
            if (zErrMsg) {
                sqlite3_free(zErrMsg);
                zErrMsg = NULL;
            }
            break;
            
        case 4:
            rc = sqlite3_overload_function(db, func_name, (int)(Data[1] % 10));
            break;
            
        case 5:
            rc = sqlite3_exec(db, "CREATE TABLE t1(a, b);", NULL, NULL, &zErrMsg);
            if (rc == SQLITE_OK) {
                rc = sqlite3_get_table(db, "SELECT * FROM t1;", &pazResult, &nRow, &nColumn, &zErrMsg);
                if (pazResult) {
                    sqlite3_free_table(pazResult);
                    pazResult = NULL;
                }
            }
            if (zErrMsg) {
                sqlite3_free(zErrMsg);
                zErrMsg = NULL;
            }
            break;
    }
    
    free(module_name);
    free(sql_stmt);
    free(func_name);
    
    if (pazResult) {
        sqlite3_free_table(pazResult);
    }
    if (zErrMsg) {
        sqlite3_free(zErrMsg);
    }
    
    sqlite3_close(db);
    
    return 0;
}