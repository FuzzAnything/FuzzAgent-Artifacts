// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzzing Driver for SQLite Virtual Table APIs
 *
 * This driver fuzzes the following APIs:
 * - sqlite3_create_module
 * - sqlite3_create_module_v2
 * - sqlite3_overload_function
 * - sqlite3_declare_vtab (triggered via SQL)
 * - sqlite3_vtab_config (called within module methods)
 * - sqlite3_vtab_collation (called within xBestIndex)
 */

/* Static helper to avoid name conflicts */
static void cleanup_module(void *pClientData) {
    /* 
     * We do NOT free pClientData here.
     * The driver manages the lifetime of this memory.
     * If we free it here, and then free it again in LLVMFuzzerTestOneInput,
     * we get a double-free.
     * We simply mark it as "cleaned up" by setting a flag or just ignoring.
     * Since we pass a pointer to an int array, we can't easily flag it without extra memory.
     * Instead, we will just not free it here, and free it in the driver after sqlite3_close.
     * However, sqlite3_close might call this destructor if the module is dropped.
     * To prevent double free, we set the pointer to NULL in the destructor if possible,
     * but the destructor takes void*. We can't modify the pointer in the caller.
     * 
     * Strategy: 
     * 1. Allocate a wrapper struct that contains the data and a 'freed' flag.
     * 2. Or, simply don't pass a destructor to sqlite3_create_module_v2 if we manage memory manually.
     * 3. Or, pass a destructor that does nothing, and free manually in the driver.
     * 
     * Let's use strategy 3: Pass NULL as destructor, or a no-op destructor, and free manually.
     * But the task requires testing sqlite3_create_module_v2 with a destructor.
     * So we must handle the double-free risk.
     * 
     * Correct approach for fuzzing:
     * The destructor is called by SQLite when the module is dropped or DB closed.
     * If we free in the destructor, we must NOT free in the driver.
     * But the driver needs to clean up if the DB close fails or if we want to ensure cleanup.
     * 
     * Let's change the approach:
     * We will allocate the client_data.
     * We will pass a destructor that does NOT free, but sets a flag? No, void*.
     * 
     * Better approach:
     * Use a global flag or a specific pattern in the data to indicate if it's already freed?
     * No, that's race-prone (though single threaded here).
     * 
     * Simplest fix:
     * Do NOT pass a destructor that frees. Pass a destructor that does nothing.
     * Then free manually in the driver.
     * BUT, the task says "Call it with ... optional cleanup".
     * To test the destructor path, we need a destructor that runs.
     * If the destructor frees, we must not free again.
     * 
     * Let's assume the destructor IS called. We will free inside the destructor.
     * Then in the driver, we will NOT free.
     * But what if the destructor is NOT called (e.g. error before registration)?
     * Then we leak.
     * 
     * Robust solution:
     * 1. Allocate client_data.
     * 2. Pass a destructor that frees.
     * 3. In the driver, do NOT free client_data after sqlite3_close.
     * 4. If sqlite3_create_module_v2 fails, the destructor is called immediately (per docs).
     *    So if it fails, the memory is freed. We must not free it then.
     * 5. If it succeeds, the memory is freed on close.
     * 
     * So: Never free client_data in the driver if we passed a destructor that frees.
     * But we need to know if we passed a destructor.
     * 
     * Let's just NOT free in the driver if we used the destructor.
     * We'll track if we allocated it.
     */
    if (pClientData) {
        /* 
         * We will free here.
         * To prevent double free, we will NOT free in the driver.
         * But we need to handle the case where the registration fails and destructor is called.
         * The docs say: "The destructor will also be invoked if the call to sqlite3_create_module_v2() fails."
         * So if it fails, it's freed.
         * If it succeeds, it's freed on close.
         * So we should never free in the driver.
         */
        free(pClientData);
    }
}

/*
 * Dummy xCreate implementation for the virtual table module.
 */
static int dummy_xCreate(sqlite3 *db, void *pAux,
                         int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    (void)argc;
    (void)argv;
    (void)pzErr;

    /* Fuzz sqlite3_vtab_config with random options */
    if (pAux) {
        int *config_opts = (int *)pAux;
        int op = config_opts[0] & 0xFF;
        int arg1 = config_opts[1];
        int arg2 = config_opts[2];
        sqlite3_vtab_config(db, op, arg1, arg2);
    }

    /* Attempt to declare a schema */
    const char *zSQL = "CREATE TABLE dummy(col1 TEXT, col2 INTEGER)";
    if (argc > 0 && argv[0] && strlen(argv[0]) > 5) {
        if (strncmp(argv[0], "CREATE", 6) == 0) {
            zSQL = argv[0];
        }
    }

    sqlite3_declare_vtab(db, zSQL);

    *ppVTab = malloc(sizeof(sqlite3_vtab));
    if (*ppVTab) {
        memset(*ppVTab, 0, sizeof(sqlite3_vtab));
    } else {
        return SQLITE_NOMEM;
    }
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux,
                          int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    return dummy_xCreate(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    (void)pVTab;
    if (!pIdxInfo) return SQLITE_ERROR;

    if (pIdxInfo->nConstraint > 0) {
        for (int i = 0; i < pIdxInfo->nConstraint; i++) {
            if (i < pIdxInfo->nConstraint) {
                const char *coll = sqlite3_vtab_collation(pIdxInfo, i);
                (void)coll;
            }
        }
    }

    pIdxInfo->idxNum = 0;
    pIdxInfo->idxStr = NULL;
    pIdxInfo->needToFreeIdxStr = 0;
    pIdxInfo->orderByConsumed = 0;
    pIdxInfo->estimatedCost = 1.0;
    if (pIdxInfo->aConstraintUsage) {
        for (int i = 0; i < pIdxInfo->nConstraint; i++) {
            pIdxInfo->aConstraintUsage[i].argvIndex = 0;
            pIdxInfo->aConstraintUsage[i].omit = 0;
        }
    }

    return SQLITE_OK;
}

/* Dummy implementations */
static int dummy_xDisconnect(sqlite3_vtab *pVTab) { if (pVTab) free(pVTab); return SQLITE_OK; }
static int dummy_xDestroy(sqlite3_vtab *pVTab) { if (pVTab) free(pVTab); return SQLITE_OK; }
static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVTab;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor) memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}
static int dummy_xClose(sqlite3_vtab_cursor *pCursor) { if (pCursor) free(pCursor); return SQLITE_OK; }
static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv; return SQLITE_OK;
}
static int dummy_xNext(sqlite3_vtab_cursor *pCursor) { (void)pCursor; return SQLITE_OK; }
static int dummy_xEof(sqlite3_vtab_cursor *pCursor) { (void)pCursor; return 1; }
static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor; (void)i; sqlite3_result_null(ctx); return SQLITE_OK;
}
static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor; *pRowid = 1; return SQLITE_OK;
}
static int dummy_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVTab; (void)argc; (void)argv; (void)pRowid; return SQLITE_OK;
}
static int dummy_xBegin(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xSync(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xCommit(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xRollback(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName, void (**pxFunc)(sqlite3_context *, int, sqlite3_value **), void **ppArg) {
    (void)pVtab; (void)nArg; (void)zName; (void)pxFunc; (void)ppArg; return SQLITE_OK;
}
static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) { (void)pVtab; (void)zNew; return SQLITE_OK; }
static int dummy_xSavepoint(sqlite3_vtab *pVTab, int) { (void)pVTab; return SQLITE_OK; }
static int dummy_xRelease(sqlite3_vtab *pVTab, int) { (void)pVTab; return SQLITE_OK; }
static int dummy_xRollbackTo(sqlite3_vtab *pVTab, int) { (void)pVTab; return SQLITE_OK; }
static int dummy_xShadowName(const char *) { return SQLITE_OK; }
static int dummy_xIntegrity(sqlite3_vtab *pVTab, const char *zSchema, const char *zTabName, int mFlags, char **pzErr) {
    (void)pVTab; (void)zSchema; (void)zTabName; (void)mFlags; (void)pzErr; return SQLITE_OK;
}

static const sqlite3_module g_dummy_module = {
    3,
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

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    if (Size < 1) return 0;

    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    const char *module_name = "fuzz_module";
    void *client_data = NULL;
    int *config_opts = NULL;

    /* Allocate client data */
    if (Size >= 3) {
        config_opts = malloc(sizeof(int) * 3);
        if (config_opts) {
            config_opts[0] = Data[0];
            config_opts[1] = Data[1];
            config_opts[2] = Data[2];
            client_data = (void *)config_opts;
        }
    }

    /* Fuzz sqlite3_create_module */
    sqlite3_create_module(db, module_name, NULL, NULL);
    if (client_data) {
        sqlite3_create_module(db, module_name, &g_dummy_module, client_data);
    } else {
        sqlite3_create_module(db, module_name, &g_dummy_module, NULL);
    }

    /* Fuzz sqlite3_create_module_v2 */
    /* 
     * We pass cleanup_module as destructor.
     * cleanup_module will free the memory.
     * Therefore, we MUST NOT free client_data here or later in this function.
     * The destructor is called if registration fails OR when DB is closed.
     */
    if (client_data) {
        sqlite3_create_module_v2(db, module_name, &g_dummy_module, client_data, cleanup_module);
    } else {
        sqlite3_create_module_v2(db, module_name, &g_dummy_module, NULL, NULL);
    }

    /* Fuzz sqlite3_overload_function */
    if (Size > 10) {
        char func_name[32];
        int nArg = Data[10] % 10;
        int len = (Data[11] % 10) + 1;
        if (len > 31) len = 31;
        memcpy(func_name, Data + 12, len);
        func_name[len] = '\0';
        sqlite3_overload_function(db, func_name, nArg);
    }

    /* Trigger sqlite3_declare_vtab and sqlite3_vtab_config via SQL */
    if (Size > 20) {
        char sql[256];
        int len = Size - 20;
        if (len > 200) len = 200;
        snprintf(sql, sizeof(sql), "CREATE VIRTUAL TABLE t1 USING fuzz_module('%.*s')", len, (char*)(Data + 20));
        sqlite3_exec(db, sql, NULL, NULL, NULL);
    }

    /* Trigger xBestIndex */
    sqlite3_exec(db, "SELECT * FROM t1 WHERE col1 = 'test';", NULL, NULL, NULL);
    sqlite3_exec(db, "SELECT * FROM t1 ORDER BY col2;", NULL, NULL, NULL);

    /* Cleanup */
    /* 
     * Do NOT free client_data here.
     * If registration succeeded, sqlite3_close will call cleanup_module which frees it.
     * If registration failed, cleanup_module was called immediately, so it's already freed.
     * Freeing here would cause double-free.
     */
    sqlite3_close(db);

    return 0;
}