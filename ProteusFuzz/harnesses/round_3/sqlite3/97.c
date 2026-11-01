// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_set_clientdata at sqlite3.c:189495:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_collation_v2 at sqlite3.c:189382:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_create_function_v2 at sqlite3.c:187755:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to create a minimal dummy module for fuzzing */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
    return SQLITE_OK;
}
static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
    return SQLITE_OK;
}
static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    (void)pVTab; (void)pIdxInfo;
    return SQLITE_OK;
}
static int dummy_xDisconnect(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xDestroy(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) { (void)pVTab; (void)ppCursor; return SQLITE_OK; }
static int dummy_xClose(sqlite3_vtab_cursor *pCursor) { (void)pCursor; return SQLITE_OK; }
static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}
static int dummy_xNext(sqlite3_vtab_cursor *pCursor) { (void)pCursor; return SQLITE_OK; }
static int dummy_xEof(sqlite3_vtab_cursor *pCursor) { (void)pCursor; return 1; }
static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int i) { (void)pCursor; (void)pCtx; (void)i; return SQLITE_OK; }
static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) { (void)pCursor; (void)pRowid; return SQLITE_OK; }
static int dummy_xUpdate(sqlite3_vtab *pVTab, int nArg, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVTab; (void)nArg; (void)argv; (void)pRowid; return SQLITE_OK;
}
static int dummy_xBegin(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xSync(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xCommit(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xRollback(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName, void (**pxFunc)(sqlite3_context *, int, sqlite3_value **), void **ppArg) {
    (void)pVtab; (void)nArg; (void)zName; (void)pxFunc; (void)ppArg;
    return SQLITE_OK;
}
static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) { (void)pVtab; (void)zNew; return SQLITE_OK; }
static int dummy_xSavepoint(sqlite3_vtab *pVTab, int iSavepoint) { (void)pVTab; (void)iSavepoint; return SQLITE_OK; }
static int dummy_xRelease(sqlite3_vtab *pVTab, int iSavepoint) { (void)pVTab; (void)iSavepoint; return SQLITE_OK; }
static int dummy_xRollbackTo(sqlite3_vtab *pVTab, int iSavepoint) { (void)pVTab; (void)iSavepoint; return SQLITE_OK; }

static sqlite3_module g_dummy_module = {
    3, /* iVersion */
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
    NULL /* xShadowName */
};

/* Static destructor for client data - Does NOT free memory to avoid double-free */
static void dummy_destructor(void *p) {
    (void)p;
    /* Memory management is handled by the fuzz driver, not the destructor,
       to prevent double-free when the same buffer is passed to multiple APIs
       or when the API takes ownership but the driver attempts to free it later. */
}

/* Static collation comparator */
static int dummy_collate(void *pArg, int nLeft, const void *zLeft, int nRight, const void *zRight) {
    (void)pArg;
    if (nLeft < nRight) return -1;
    if (nLeft > nRight) return 1;
    return memcmp(zLeft, zRight, nLeft);
}

/* Static function implementation */
static void dummy_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx; (void)argc; (void)argv;
    sqlite3_result_int(ctx, 0);
}

static void dummy_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx; (void)argc; (void)argv;
}

static void dummy_final(sqlite3_context *ctx) {
    (void)ctx;
    sqlite3_result_int(ctx, 0);
}

int LLVMFuzzerTestOneInput_97(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zName = NULL;
    void *pClientData = NULL;
    int nArg = 0;
    int eTextRep = SQLITE_UTF8;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Prepare data from fuzz input */
    if (Size > 0) {
        /* Create a safe name string from input, ensuring null termination */
        size_t nameLen = (Size > 100) ? 100 : Size;
        zName = (char *)malloc(nameLen + 1);
        if (zName) {
            memcpy(zName, Data, nameLen);
            zName[nameLen] = '\0';
        }
    }

    /* Allocate client data if input is large enough */
    if (Size > 5) {
        pClientData = malloc(Size - 5);
        if (pClientData && Size > 5) {
            memcpy(pClientData, Data + 5, Size - 5);
        }
    }

    /* Parse nArg safely */
    if (Size > 0) {
        nArg = Data[0] % 10; /* Limit arguments to 0-9 */
    }

    /* Fuzz sqlite3_set_clientdata */
    if (zName) {
        sqlite3_set_clientdata(db, zName, pClientData, dummy_destructor);
        /* Try removing it */
        sqlite3_set_clientdata(db, zName, NULL, NULL);
    }

    /* Fuzz sqlite3_create_module_v2 */
    if (zName) {
        /* Pass pClientData again. Since dummy_destructor does not free, 
           this is safe even if the previous call triggered the destructor. */
        sqlite3_create_module_v2(db, zName, &g_dummy_module, pClientData, dummy_destructor);
        /* Try dropping the module */
        sqlite3_create_module_v2(db, zName, NULL, NULL, NULL);
    }

    /* Fuzz sqlite3_create_module */
    if (zName) {
        sqlite3_create_module(db, zName, &g_dummy_module, pClientData);
        /* Try dropping the module */
        sqlite3_create_module(db, zName, NULL, NULL);
    }

    /* Fuzz sqlite3_create_collation_v2 */
    if (zName) {
        sqlite3_create_collation_v2(db, zName, eTextRep, pClientData, dummy_collate, dummy_destructor);
        /* Try with NULL destructor */
        sqlite3_create_collation_v2(db, zName, eTextRep, pClientData, dummy_collate, NULL);
    }

    /* Fuzz sqlite3_create_function_v2 */
    if (zName) {
        sqlite3_create_function_v2(db, zName, nArg, eTextRep, pClientData, dummy_func, dummy_step, dummy_final, dummy_destructor);
        /* Try scalar only */
        sqlite3_create_function_v2(db, zName, nArg, eTextRep, pClientData, dummy_func, NULL, NULL, dummy_destructor);
        /* Try aggregate only */
        sqlite3_create_function_v2(db, zName, nArg, eTextRep, pClientData, NULL, dummy_step, dummy_final, dummy_destructor);
        /* Try with NULL destructor */
        sqlite3_create_function_v2(db, zName, nArg, eTextRep, pClientData, dummy_func, NULL, NULL, NULL);
    }

    /* Fuzz sqlite3_overload_function */
    if (zName) {
        sqlite3_overload_function(db, zName, nArg);
    }

    /* Cleanup */
    if (zName) free(zName);
    
    /* Free pClientData manually since dummy_destructor does not free it anymore.
       This prevents double-free and ensures no memory leak in the fuzz driver. */
    if (pClientData) {
        free(pClientData);
    }

    sqlite3_close(db);
    return 0;
}