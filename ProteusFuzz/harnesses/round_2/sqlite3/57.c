// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Static helper to create a minimal virtual table module for testing.
 */
static int dummy_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
    /* Attempt to configure the vtab context if inside xCreate/xConnect */
    if (db) {
        sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
        sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 1);
    }
    
    /* Declare a simple schema */
    const char *zSQL = "CREATE TABLE dummy(id INTEGER PRIMARY KEY, val TEXT)";
    if (db) {
        return sqlite3_declare_vtab(db, zSQL);
    }
    return SQLITE_ERROR;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return dummy_vtab_create(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_vtab_bestindex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    (void)pVTab;
    if (pIdxInfo && pIdxInfo->nConstraint > 0 && pIdxInfo->aConstraint) {
        const char *coll = sqlite3_vtab_collation(pIdxInfo, 0);
        (void)coll; 
    }
    return SQLITE_OK;
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_destroy(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) { (void)pVTab; *ppCursor = NULL; return SQLITE_OK; }
static int dummy_vtab_close(sqlite3_vtab_cursor *pCur) { (void)pCur; return SQLITE_OK; }
static int dummy_vtab_filter(sqlite3_vtab_cursor *pCur, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) { (void)pCur; (void)idxNum; (void)idxStr; (void)argc; (void)argv; return SQLITE_OK; }
static int dummy_vtab_next(sqlite3_vtab_cursor *pCur) { (void)pCur; return SQLITE_OK; }
static int dummy_vtab_eof(sqlite3_vtab_cursor *pCur) { (void)pCur; return 1; }
static int dummy_vtab_column(sqlite3_vtab_cursor *pCur, sqlite3_context *ctx, int i) { (void)pCur; (void)i; sqlite3_result_null(ctx); return SQLITE_OK; }
static int dummy_vtab_rowid(sqlite3_vtab_cursor *pCur, sqlite3_int64 *pRowid) { (void)pCur; *pRowid = 0; return SQLITE_OK; }
static int dummy_vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) { (void)pVTab; (void)argc; (void)argv; (void)pRowid; return SQLITE_OK; }
static int dummy_vtab_begin(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_sync(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_commit(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_rollback(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_findfunction(sqlite3_vtab *pVtab, int nArg, const char *zName, void (**pxFunc)(sqlite3_context *, int, sqlite3_value **), void **ppArg) { (void)pVtab; (void)nArg; (void)zName; (void)pxFunc; (void)ppArg; return SQLITE_OK; }
static int dummy_vtab_rename(sqlite3_vtab *pVtab, const char *zNew) { (void)pVtab; (void)zNew; return SQLITE_OK; }
static int dummy_vtab_savepoint(sqlite3_vtab *pVTab, int iSavepoint) { (void)pVTab; (void)iSavepoint; return SQLITE_OK; }
static int dummy_vtab_release(sqlite3_vtab *pVTab, int iSavepoint) { (void)pVTab; (void)iSavepoint; return SQLITE_OK; }
static int dummy_vtab_rollbackto(sqlite3_vtab *pVTab, int iSavepoint) { (void)pVTab; (void)iSavepoint; return SQLITE_OK; }
static int dummy_vtab_shadowname_wrapper(const char *zName) { (void)zName; return 0; }
static int dummy_vtab_integrity(sqlite3_vtab *pVTab, const char *zSchema, const char *zTabName, int mFlags, char **pzErr) { (void)pVTab; (void)zSchema; (void)zTabName; (void)mFlags; (void)pzErr; return SQLITE_OK; }

static const sqlite3_module dummy_module = {
    3,
    dummy_vtab_create,
    dummy_vtab_connect,
    dummy_vtab_bestindex,
    dummy_vtab_disconnect,
    dummy_vtab_destroy,
    dummy_vtab_open,
    dummy_vtab_close,
    dummy_vtab_filter,
    dummy_vtab_next,
    dummy_vtab_eof,
    dummy_vtab_column,
    dummy_vtab_rowid,
    dummy_vtab_update,
    dummy_vtab_begin,
    dummy_vtab_sync,
    dummy_vtab_commit,
    dummy_vtab_rollback,
    dummy_vtab_findfunction,
    dummy_vtab_rename,
    dummy_vtab_savepoint,
    dummy_vtab_release,
    dummy_vtab_rollbackto,
    dummy_vtab_shadowname_wrapper,
    dummy_vtab_integrity
};

int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    size_t offset = 0;
    
    /* Fuzz sqlite3_create_module and sqlite3_create_module_v2 */
    if (Size > 0) {
        int op = Data[offset++];
        const char *zName = "fuzz_vtab";
        
        if (op & 1) {
            /* Test sqlite3_create_module */
            sqlite3_create_module(db, zName, &dummy_module, NULL);
            /* Try dropping module */
            sqlite3_create_module(db, zName, NULL, NULL);
        } else {
            /* Test sqlite3_create_module_v2 */
            void *pClientData = NULL;
            void (*xDestroy)(void *) = NULL;
            
            if (offset < Size) {
                /* Only use a safe pointer if the byte is 0, otherwise NULL to avoid invalid pointer dereference */
                if (Data[offset] == 0) {
                    pClientData = (void*)0x1000; 
                }
                offset++;
            }
            /* Do not cast random bytes to function pointer to avoid segfault on call */
            xDestroy = NULL;
            
            sqlite3_create_module_v2(db, zName, &dummy_module, pClientData, xDestroy);
        }
    }

    /* Fuzz sqlite3_overload_function */
    if (offset + 4 <= Size) {
        const char *zFuncName = "fuzz_func";
        int nArg = (int)Data[offset] | (Data[offset+1] << 8) | (Data[offset+2] << 16) | (Data[offset+3] << 24);
        offset += 4;
        
        /* Ensure nArg is reasonable to avoid extreme values */
        if (nArg > 100) nArg = 100;
        if (nArg < -1) nArg = -1;
        
        sqlite3_overload_function(db, zFuncName, nArg);
        
        /* 
         * Do NOT call with NULL name. 
         * The sanitizer error indicates that sqlite3_overload_function(db, NULL, nArg) 
         * triggers an internal assertion failure (assert( pKey!=0 )) when looking up the function.
         * We skip this specific invalid input to prevent the crash.
         */
    }

    /* Additional exploration: Create a virtual table to trigger xCreate/xConnect */
    if (offset + 10 <= Size) {
        char sql[256];
        snprintf(sql, sizeof(sql), "CREATE VIRTUAL TABLE test_vtab USING fuzz_vtab");
        sqlite3_exec(db, sql, NULL, NULL, NULL);
        
        /* Try with malformed SQL */
        snprintf(sql, sizeof(sql), "CREATE VIRTUAL TABLE test_vtab USING");
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}