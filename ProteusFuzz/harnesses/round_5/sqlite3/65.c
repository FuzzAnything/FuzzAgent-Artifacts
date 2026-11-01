// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper functions to avoid name conflicts
 */

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdx) {
    int i;
    /* Call the target API: sqlite3_vtab_collation */
    for (i = 0; i < pIdx->nConstraint; i++) {
        const char *coll = sqlite3_vtab_collation(pIdx, i);
        (void)coll;
    }
    if (pIdx->nConstraint > 0) {
        pIdx->aConstraintUsage[0].argvIndex = 1;
        pIdx->aConstraintUsage[0].omit = 0;
    }
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pVtabCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pVtabCursor;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pVtabCursor) {
    (void)pVtabCursor;
    return SQLITE_DONE;
}

static int dummy_xEof(sqlite3_vtab_cursor *pVtabCursor) {
    (void)pVtabCursor;
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pVtabCursor, sqlite3_context *pCtx, int i) {
    (void)pVtabCursor;
    (void)pCtx;
    (void)i;
    sqlite3_result_text(pCtx, "dummy", -1, SQLITE_STATIC);
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pVtabCursor, sqlite_int64 *pRowid) {
    (void)pVtabCursor;
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *p, int argc, sqlite3_value **argv, sqlite_int64 *pRowid) {
    (void)p;
    (void)argc;
    (void)argv;
    (void)pRowid;
    return SQLITE_OK;
}

static int dummy_xBegin(sqlite3_vtab *p) {
    (void)p;
    return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *p) {
    (void)p;
    return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *p) {
    (void)p;
    return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *p) {
    (void)p;
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *p) {
    (void)p;
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *p) {
    (void)p;
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *p, sqlite3_vtab_cursor **ppCursor) {
    (void)p;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor) {
        memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    }
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *p) {
    (void)p;
    free(p);
    return SQLITE_OK;
}

static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    /* Call target API: sqlite3_vtab_config */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);

    /* Call target API: sqlite3_declare_vtab */
    sqlite3_declare_vtab(db, "CREATE TABLE x(a TEXT)");

    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (*ppVtab) {
        memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    }
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    /* Call target API: sqlite3_vtab_config */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 0);

    /* Call target API: sqlite3_declare_vtab */
    sqlite3_declare_vtab(db, "CREATE TABLE x(a TEXT)");

    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (*ppVtab) {
        memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    }
    return SQLITE_OK;
}

/* 
 * Fixed signature for xFindFunction to match sqlite3.h:
 * int (*xFindFunction)(sqlite3_vtab *pVtab, int nArg, const char *zName,
 *                      void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
 *                      void (**pxxInfo)(void*))
 * The last parameter is void (**)(void*), not void (**)(void*).
 * Wait, the error says: 
 * Expected: void (**)(sqlite3_context *, int, sqlite3_value **)
 * Got: void (**)(void *)
 * 
 * Actually, looking at the error message again:
 * Expected: void (**)(sqlite3_context *, int, sqlite3_value **)
 * Got: void (**)(void *)
 * 
 * The last parameter of xFindFunction in sqlite3.h is:
 * void (**pxxInfo)(void *)
 * 
 * But the error message says the expected type for the last parameter is:
 * void (**)(sqlite3_context *, int, sqlite3_value **)
 * 
 * This suggests that the xFindFunction signature in the header file being used might be different.
 * Let me check the standard signature again.
 * 
 * Standard signature:
 * int (*xFindFunction)(sqlite3_vtab *pVtab, int nArg, const char *zName,
 *                      void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
 *                      void (**pxxInfo)(void*))
 * 
 * The error message says:
 * Expected: int (*)(struct sqlite3_vtab *, int, const char *, void (**)(struct sqlite3_context *, int, struct sqlite3_value **), void **)
 * Got: int (struct sqlite3_vtab *, int, const char *, void (**)(struct sqlite3_context *, int, struct sqlite3_value **), void (**)(void *))
 * 
 * The difference is in the last parameter:
 * Expected: void **
 * Got: void (**)(void *)
 * 
 * This is strange. Let me re-read the error message carefully.
 * 
 * "incompatible function pointer types assigning to 'int (*)(sqlite3_vtab *, int, const char *, void (**)(sqlite3_context *, int, sqlite3_value **), void **)' ... from 'int (sqlite3_vtab *, int, const char *, void (**)(sqlite3_context *, int, sqlite3_value **), void (**)(void *))'"
 * 
 * The expected type for the last parameter is void **, but my function has void (**)(void *).
 * 
 * This suggests that the xFindFunction signature in the header file being used is:
 * int (*xFindFunction)(sqlite3_vtab *pVtab, int nArg, const char *zName,
 *                      void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
 *                      void **pxxInfo)
 * 
 * But that doesn't match the standard signature. Let me check the sqlite3.h file.
 * 
 * Actually, I think the issue is that the last parameter is a pointer to a function pointer, and the type of the function pointer is void (*)(void *).
 * 
 * But the error message says the expected type is void **, which is a pointer to a pointer to void.
 * 
 * This is confusing. Let me try to match the expected type exactly.
 * 
 * The expected type for the last parameter is void **, so I will change my function to match that.
 */
static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                               void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                               void **pxxInfo) {
    (void)pVtab;
    (void)nArg;
    (void)zName;
    (void)pxFunc;
    (void)pxxInfo;
    return 0;
}

static void dummy_xDestroyClientData(void *pClientData) {
    (void)pClientData;
    free(pClientData);
}

/*
 * Main Fuzz Driver
 */
int LLVMFuzzerTestOneInput_65(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_module module = {0};
    int rc;

    if (Size == 0) {
        return 0;
    }

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Initialize module structure with dummy methods */
    module.iVersion = 2;
    module.xCreate = dummy_xCreate;
    module.xConnect = dummy_xConnect;
    module.xBestIndex = dummy_xBestIndex;
    module.xDisconnect = dummy_xDisconnect;
    module.xDestroy = dummy_xDestroy;
    module.xOpen = dummy_xOpen;
    module.xClose = dummy_xClose;
    module.xFilter = dummy_xFilter;
    module.xNext = dummy_xNext;
    module.xEof = dummy_xEof;
    module.xColumn = dummy_xColumn;
    module.xRowid = dummy_xRowid;
    module.xUpdate = dummy_xUpdate;
    module.xBegin = dummy_xBegin;
    module.xSync = dummy_xSync;
    module.xCommit = dummy_xCommit;
    module.xRollback = dummy_xRollback;
    module.xFindFunction = dummy_xFindFunction;

    /* Allocate client data for testing destructor */
    void *pClientData = malloc(Size);
    if (pClientData && Size > 0) {
        memcpy(pClientData, Data, Size);
    }

    /* 2. Invoke target functions diversely */

    /* Fuzz sqlite3_create_module */
    rc = sqlite3_create_module(db, "fuzz_module", &module, pClientData);
    (void)rc;

    /* Fuzz sqlite3_create_module_v2 with destructor */
    rc = sqlite3_create_module_v2(db, "fuzz_module_v2", &module, pClientData, dummy_xDestroyClientData);
    (void)rc;

    /* Fuzz sqlite3_overload_function */
    const char *funcName = "fuzz_func";
    if (Size > 0 && Size < 100) {
        char *safeName = malloc(Size + 1);
        if (safeName) {
            memcpy(safeName, Data, Size);
            safeName[Size] = '\0';
            funcName = safeName;
            rc = sqlite3_overload_function(db, funcName, (int)(Size % 10));
            free(safeName);
            (void)rc;
        }
    } else {
        rc = sqlite3_overload_function(db, funcName, 0);
        (void)rc;
    }

    /*
     * To test sqlite3_vtab_collation, we must be inside xBestIndex.
     * We trigger this by preparing a query that uses the virtual table.
     */
    char sql[512];
    int sqlLen = sizeof(sql) - 1;
    
    if (Size > 0) {
        int op = Data[0] % 3;
        if (op == 0) {
            snprintf(sql, sqlLen, "SELECT * FROM fuzz_module WHERE a COLLATE BINARY = 'test'");
        } else if (op == 1) {
            snprintf(sql, sqlLen, "SELECT * FROM fuzz_module ORDER BY a");
        } else {
            snprintf(sql, sqlLen, "SELECT * FROM fuzz_module");
        }
    } else {
        snprintf(sql, sqlLen, "SELECT * FROM fuzz_module");
    }

    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK && stmt) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Consume rows */
        }
        sqlite3_finalize(stmt);
    }

    /* 3. Cleanup */
    sqlite3_close(db);
    
    /* Note: pClientData is managed by SQLite if the module was registered with a destructor.
     * If the module was dropped or connection closed, the destructor should have freed it.
     * We do not free it here to avoid double-free if SQLite already did.
     */

    return 0;
}