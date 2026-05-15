// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
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

static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int n, sqlite3_value **val, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xBegin(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName, void (**pxFunc)(sqlite3_context *, int, sqlite3_value **), void **ppArg) {
    return 0;
}

static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static int dummy_xSavepoint(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xRelease(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xRollbackTo(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xShadowName(const char *zName) {
    return 0;
}

static int dummy_xIntegrity(sqlite3_vtab *pVTab, const char *zSchema, const char *zTabName, int mFlags, char **pzErr) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    .iVersion = 4,
    .xCreate = dummy_xCreate,
    .xConnect = dummy_xConnect,
    .xBestIndex = dummy_xBestIndex,
    .xDisconnect = dummy_xDisconnect,
    .xDestroy = dummy_xDestroy,
    .xOpen = dummy_xOpen,
    .xClose = dummy_xClose,
    .xFilter = dummy_xFilter,
    .xNext = dummy_xNext,
    .xEof = dummy_xEof,
    .xColumn = dummy_xColumn,
    .xRowid = dummy_xRowid,
    .xUpdate = dummy_xUpdate,
    .xBegin = dummy_xBegin,
    .xSync = dummy_xSync,
    .xCommit = dummy_xCommit,
    .xRollback = dummy_xRollback,
    .xFindFunction = dummy_xFindFunction,
    .xRename = dummy_xRename,
    .xSavepoint = dummy_xSavepoint,
    .xRelease = dummy_xRelease,
    .xRollbackTo = dummy_xRollbackTo,
    .xShadowName = dummy_xShadowName,
    .xIntegrity = dummy_xIntegrity
};

static char *extract_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    size_t start = *offset;
    while (*offset < size && data[*offset] != 0) {
        (*offset)++;
    }
    
    size_t len = *offset - start;
    if (len == 0 && data[start] == 0) {
        (*offset)++;
        char *str = malloc(1);
        if (str) str[0] = '\0';
        return str;
    }
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, data + start, len);
    str[len] = '\0';
    (*offset)++;
    
    return str;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;
    
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    size_t offset = 0;
    
    char *module_name = extract_string(Data, Size, &offset);
    char *sql_statement = extract_string(Data, Size, &offset);
    char *function_name = extract_string(Data, Size, &offset);
    
    if (!module_name) module_name = strdup("test_module");
    if (!sql_statement) sql_statement = strdup("CREATE TABLE test(a, b, c)");
    if (!function_name) function_name = strdup("test_func");
    
    int nArg = 0;
    if (offset < Size) {
        nArg = Data[offset] % 10;
        offset++;
    }
    
    const char *keep_modules[] = {NULL};
    const char *keep_modules_with_name[] = {module_name, NULL};
    
    int action = 0;
    if (offset < Size) {
        action = Data[offset] % 6;
        offset++;
    }
    
    switch (action) {
        case 0: {
            rc = sqlite3_create_module(db, module_name, &dummy_module, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_create_module(db, "another_module", NULL, NULL);
            }
            break;
        }
        case 1: {
            rc = sqlite3_create_module(db, module_name, &dummy_module, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_drop_modules(db, NULL);
                sqlite3_drop_modules(db, keep_modules);
                sqlite3_drop_modules(db, keep_modules_with_name);
            }
            break;
        }
        case 2: {
            rc = sqlite3_create_module(db, module_name, &dummy_module, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_overload_function(db, function_name, nArg);
                sqlite3_overload_function(db, "another_func", 2);
            }
            break;
        }
        case 3: {
            char **result = NULL;
            int nrow = 0, ncol = 0;
            char *errmsg = NULL;
            
            rc = sqlite3_get_table(db, sql_statement, &result, &nrow, &ncol, &errmsg);
            if (result) {
                sqlite3_free_table(result);
            }
            if (errmsg) {
                sqlite3_free(errmsg);
            }
            break;
        }
        case 4: {
            char *dummy_result[] = {"col1", "col2", "data1", "data2", NULL};
            sqlite3_free_table(dummy_result);
            
            char *another_dummy[] = {NULL};
            sqlite3_free_table(another_dummy);
            break;
        }
        case 5: {
            rc = sqlite3_create_module(db, module_name, &dummy_module, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_exec(db, "CREATE VIRTUAL TABLE test USING test_module", NULL, NULL, NULL);
                sqlite3_declare_vtab(db, sql_statement);
            }
            break;
        }
    }
    
    free(module_name);
    free(sql_statement);
    free(function_name);
    
    sqlite3_close(db);
    
    return 0;
}