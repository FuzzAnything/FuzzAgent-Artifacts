// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy virtual table module implementation */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    *ppVTab = (sqlite3_vtab *)malloc(sizeof(sqlite3_vtab));
    if (!*ppVTab) return SQLITE_NOMEM;
    memset(*ppVTab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    return dummy_xCreate(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    free(pVTab);
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    return dummy_xDisconnect(pVTab);
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = (sqlite3_vtab_cursor *)malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    free(pCursor);
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv,
                         sqlite3_int64 *pRowid) {
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

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                               void (**pxFunc)(sqlite3_context *, int, sqlite3_value **),
                               void **ppArg) {
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

static int dummy_xIntegrity(sqlite3_vtab *pVTab, const char *zSchema,
                            const char *zTabName, int mFlags, char **pzErr) {
    return SQLITE_OK;
}

static void dummy_destructor(void *p) {
    /* Nothing to free */
}

static sqlite3_module dummy_module = {
    4, /* iVersion */
    dummy_xCreate,
    dummy_xConnect,
    dummy_xBestIndex,
    dummy_xDisconnect,
    dummy_xDestroy,
    dummy_xOpen,
    dummy_xClose,
    dummy_xFilter,
    dummy_xNext,
    dummy_xEof,
    dummy_xColumn,
    dummy_xRowid,
    dummy_xUpdate,
    dummy_xBegin,
    dummy_xSync,
    dummy_xCommit,
    dummy_xRollback,
    dummy_xFindFunction,
    dummy_xRename,
    dummy_xSavepoint,
    dummy_xRelease,
    dummy_xRollbackTo,
    dummy_xShadowName,
    dummy_xIntegrity
};

/* Helper function to extract strings from fuzzing input */
static const char *extract_string(const uint8_t **data, size_t *size) {
    if (*size == 0) return NULL;
    
    size_t len = (*data)[0] % 256;
    if (len == 0) len = 1;
    if (len > *size - 1) len = *size - 1;
    
    static char buf[256];
    if (len > 255) len = 255;
    
    memcpy(buf, *data + 1, len);
    buf[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return buf;
}

/* Main fuzzing entry point */
int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Extract parameters from fuzzing input */
    const char *module_name = extract_string(&ptr, &remaining);
    const char *func_name = extract_string(&ptr, &remaining);
    const char *sql = extract_string(&ptr, &remaining);
    const char *keep_module = extract_string(&ptr, &remaining);
    
    /* Use defaults if extraction failed */
    if (!module_name) module_name = "dummy";
    if (!func_name) func_name = "func";
    if (!sql) sql = "CREATE TABLE t(x)";
    if (!keep_module) keep_module = "dummy";
    
    /* Test sqlite3_create_module */
    sqlite3_create_module(db, module_name, &dummy_module, NULL);
    
    /* Test sqlite3_create_module_v2 */
    sqlite3_create_module_v2(db, module_name, &dummy_module, NULL, dummy_destructor);
    
    /* Test sqlite3_declare_vtab - need to be in xCreate/xConnect context */
    {
        sqlite3_vtab *vtab = NULL;
        char *errmsg = NULL;
        dummy_xCreate(db, NULL, 0, NULL, &vtab, &errmsg);
        if (vtab) {
            sqlite3_declare_vtab(db, sql);
            dummy_xDestroy(vtab);
        }
        if (errmsg) sqlite3_free(errmsg);
    }
    
    /* Test sqlite3_drop_modules */
    {
        const char *keep_list[] = {keep_module, NULL};
        sqlite3_drop_modules(db, keep_list);
    }
    
    /* Test sqlite3_overload_function */
    {
        int nArg = (ptr < Data + Size) ? *ptr % 10 : 1;
        sqlite3_overload_function(db, func_name, nArg);
    }
    
    /* Test sqlite3_vtab_config - need to be in xCreate/xConnect context */
    {
        sqlite3_vtab *vtab = NULL;
        char *errmsg = NULL;
        dummy_xCreate(db, NULL, 0, NULL, &vtab, &errmsg);
        if (vtab) {
            int op = (ptr + 1 < Data + Size) ? *(ptr + 1) % 10 : 0;
            sqlite3_vtab_config(db, op);
            dummy_xDestroy(vtab);
        }
        if (errmsg) sqlite3_free(errmsg);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}