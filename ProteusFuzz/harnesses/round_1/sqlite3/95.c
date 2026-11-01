// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 virtual table APIs.
 * Target APIs:
 * 1. sqlite3_vtab_in
 * 2. sqlite3_vtab_config
 * 3. sqlite3_vtab_rhs_value
 * 4. sqlite3_vtab_collation
 * 5. sqlite3_vtab_distinct
 * 6. sqlite3_overload_function
 */

static int g_fuzz_input_idx = 0;
static const uint8_t *g_fuzz_data = NULL;
static size_t g_fuzz_size = 0;

/* Helper to read bytes from fuzz input safely */
static int get_int_from_input(int *val) {
    if (g_fuzz_input_idx + 4 > g_fuzz_size) return 0;
    memcpy(val, &g_fuzz_data[g_fuzz_input_idx], 4);
    g_fuzz_input_idx += 4;
    return 1;
}

/* Virtual Table Implementation */
static int vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux; (void)argc; (void)argv; (void)pzErr;
    
    /* Fuzz sqlite3_vtab_config */
    int op = 1;
    if (get_int_from_input(&op)) {
        if (op < 0 || op > 10) op = 1;
        sqlite3_vtab_config(db, op, 1);
    }
    
    *ppVtab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVtab) {
        memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    }
    return *ppVtab ? SQLITE_OK : SQLITE_NOMEM;
}

static int vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                        sqlite3_vtab **ppVtab, char **pzErr) {
    return vtab_xConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

static int vtab_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab;
    
    /* Fuzz sqlite3_vtab_in */
    int iCons = 0;
    int bHandle = -1;
    
    if (get_int_from_input(&iCons)) {
        if (iCons < 0) iCons = 0;
        if (iCons >= pIdxInfo->nConstraint) iCons = pIdxInfo->nConstraint > 0 ? pIdxInfo->nConstraint - 1 : 0;
    }
    
    if (get_int_from_input(&bHandle)) {
        if (bHandle < -1) bHandle = -1;
        if (bHandle > 1) bHandle = 1;
    }
    
    sqlite3_vtab_in(pIdxInfo, iCons, bHandle);
    
    if (get_int_from_input(&iCons) && get_int_from_input(&bHandle)) {
        if (iCons < 0) iCons = 0;
        if (iCons >= pIdxInfo->nConstraint) iCons = pIdxInfo->nConstraint > 0 ? pIdxInfo->nConstraint - 1 : 0;
        if (bHandle < -1) bHandle = -1;
        if (bHandle > 1) bHandle = 1;
        sqlite3_vtab_in(pIdxInfo, iCons, bHandle);
    }

    /* Fuzz sqlite3_vtab_rhs_value */
    sqlite3_value *pVal = NULL;
    int j = 0;
    if (get_int_from_input(&j)) {
        if (j < 0) j = 0;
        if (j >= pIdxInfo->nConstraint) j = pIdxInfo->nConstraint > 0 ? pIdxInfo->nConstraint - 1 : 0;
    }
    sqlite3_vtab_rhs_value(pIdxInfo, j, &pVal);

    /* Fuzz sqlite3_vtab_collation */
    int k = 0;
    if (get_int_from_input(&k)) {
        if (k < 0) k = 0;
        if (k >= pIdxInfo->nConstraint) k = pIdxInfo->nConstraint > 0 ? pIdxInfo->nConstraint - 1 : 0;
    }
    const char *coll = sqlite3_vtab_collation(pIdxInfo, k);
    (void)coll;

    /* Fuzz sqlite3_vtab_distinct */
    int distinct = sqlite3_vtab_distinct(pIdxInfo);
    (void)distinct;

    pIdxInfo->estimatedCost = 1.0;
    pIdxInfo->estimatedRows = 1;
    return SQLITE_OK;
}

static int vtab_xDisconnect(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int vtab_xDestroy(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int vtab_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    *ppCursor = (sqlite3_vtab_cursor *)sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor) memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return *ppCursor ? SQLITE_OK : SQLITE_NOMEM;
}

static int vtab_xClose(sqlite3_vtab_cursor *pCursor) {
    sqlite3_free(pCursor);
    return SQLITE_OK;
}

static int vtab_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                        int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}

static int vtab_xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_DONE;
}

static int vtab_xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int vtab_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor; (void)ctx; (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int vtab_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 1;
    return SQLITE_OK;
}

/* Fixed signature: must include sqlite3_vtab *pVtab as first argument */
static int vtab_xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVtab; (void)argc; (void)argv; (void)pRowid;
    return SQLITE_OK;
}

static int vtab_xBegin(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int vtab_xSync(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int vtab_xCommit(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int vtab_xRollback(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static const sqlite3_module g_vtab_module = {
    0, /* iVersion */
    vtab_xCreate,
    vtab_xConnect,
    vtab_xBestIndex,
    vtab_xDisconnect,
    vtab_xDestroy,
    vtab_xOpen,
    vtab_xClose,
    vtab_xFilter,
    vtab_xNext,
    vtab_xEof,
    vtab_xColumn,
    vtab_xRowid,
    vtab_xUpdate,
    vtab_xBegin,
    vtab_xSync,
    vtab_xCommit,
    vtab_xRollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
};

int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    g_fuzz_data = Data;
    g_fuzz_size = Size;
    g_fuzz_input_idx = 0;

    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    rc = sqlite3_create_module(db, "fuzz_vtab", &g_vtab_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    const char *create_sql = "CREATE TABLE t1 USING fuzz_vtab(a, b, c);";
    sqlite3_exec(db, create_sql, NULL, NULL, NULL);

    const char *query_sql = "SELECT * FROM t1 WHERE a = ? AND b IN (?, ?);";
    sqlite3_stmt *stmt = NULL;
    
    rc = sqlite3_prepare_v2(db, query_sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, 1);
        sqlite3_bind_int(stmt, 2, 2);
        sqlite3_bind_int(stmt, 3, 3);
        
        while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            /* Iterate */
        }
        sqlite3_finalize(stmt);
    }

    /* Fuzz sqlite3_overload_function */
    char func_name[32];
    int nArg = 0;
    
    int name_len = 0;
    if (g_fuzz_input_idx < g_fuzz_size) {
        name_len = g_fuzz_data[g_fuzz_input_idx] % 16 + 1;
        g_fuzz_input_idx++;
    }
    if (name_len > 31) name_len = 31;
    
    if (g_fuzz_input_idx + name_len <= g_fuzz_size) {
        memcpy(func_name, &g_fuzz_data[g_fuzz_input_idx], name_len);
        func_name[name_len] = '\0';
        g_fuzz_input_idx += name_len;
    } else {
        strcpy(func_name, "fuzz_func");
    }

    if (get_int_from_input(&nArg)) {
        if (nArg < 0) nArg = 0;
        if (nArg > 10) nArg = 10;
    }
    
    sqlite3_overload_function(db, func_name, nArg);

    sqlite3_exec(db, "DROP TABLE t1", NULL, NULL, NULL);
    sqlite3_close(db);

    return 0;
}