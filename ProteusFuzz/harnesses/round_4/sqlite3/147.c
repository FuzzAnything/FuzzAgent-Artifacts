// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a dummy virtual table module.
 * This is necessary because the target APIs (sqlite3_vtab_rhs_value, etc.)
 * are only valid when called from within an xBestIndex method of a virtual table.
 */

/* Forward declaration */
static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo);

/* Fix: Use const char *const * for argv to match sqlite3_module signature */
static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVtab, char **pzErr) {
    /* 
     * Call sqlite3_vtab_config here as it is allowed in xConnect.
     * We use a dummy operation code. SQLITE_VTAB_CONSTRAINT_SUPPORT is a common one.
     */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    
    /* Allocate a dummy vtab structure */
    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (*ppVtab == NULL) {
        return SQLITE_NOMEM;
    }
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    (*ppVtab)->pModule = (sqlite3_module*)pAux;
    return SQLITE_OK;
}

static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVtab, char **pzErr) {
    return dummy_xConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    /* 
     * The fuzzer will invoke the target APIs here.
     * Since we cannot easily pass the fuzz input directly into this callback 
     * without complex state management, we will invoke the APIs with 
     * safe default parameters or derived from the input if possible.
     * However, the primary goal is to trigger the code paths inside these functions.
     * We will perform a few safe calls to ensure coverage.
     */
    
    /* 1. sqlite3_vtab_rhs_value */
    sqlite3_value *pVal = NULL;
    /* Try to get RHS value for the first constraint if it exists */
    if (pIdxInfo->nConstraint > 0) {
        sqlite3_vtab_rhs_value(pIdxInfo, 0, &pVal);
        /* pVal is protected and valid only during this call, so we don't free it. */
    }

    /* 2. sqlite3_vtab_in */
    /* Check feasibility (-1) and then request (1) or decline (0) */
    if (pIdxInfo->nConstraint > 0) {
        sqlite3_vtab_in(pIdxInfo, 0, -1);
        sqlite3_vtab_in(pIdxInfo, 0, 1);
        sqlite3_vtab_in(pIdxInfo, 0, 0);
    }

    /* 3. sqlite3_vtab_distinct */
    sqlite3_vtab_distinct(pIdxInfo);

    /* 4. sqlite3_vtab_collation */
    if (pIdxInfo->nConstraint > 0) {
        sqlite3_vtab_collation(pIdxInfo, 0);
    }

    /* 
     * sqlite3_overload_function and sqlite3_vtab_config require a sqlite3* handle.
     * We cannot easily access the connection handle (pVtab->db) safely here without 
     * risking undefined behavior if the vtab context isn't fully set up, 
     * but typically pVtab->db is valid.
     * However, to be safe and robust in a fuzzer, we will rely on the 
     * main fuzzer loop to call these with a valid connection.
     * We will not call them here to avoid potential crashes if pVtab->db is NULL 
     * in some edge cases of the fuzzer setup.
     */

    return SQLITE_OK;
}

/* Placeholder for other required methods */
static int dummy_xFilter(sqlite3_vtab_cursor *pCsr, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) { return SQLITE_OK; }
static int dummy_xNext(sqlite3_vtab_cursor *pCsr) { return SQLITE_DONE; }
static int dummy_xEof(sqlite3_vtab_cursor *pCsr) { return 1; }
static int dummy_xColumn(sqlite3_vtab_cursor *pCsr, sqlite3_context *ctx, int i) { sqlite3_result_null(ctx); return SQLITE_OK; }
static int dummy_xRowid(sqlite3_vtab_cursor *pCsr, sqlite3_int64 *pRowid) { *pRowid = 0; return SQLITE_OK; }
static int dummy_xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) { return SQLITE_OK; }
static int dummy_xBegin(sqlite3_vtab *pVtab) { return SQLITE_OK; }
static int dummy_xSync(sqlite3_vtab *pVtab) { return SQLITE_OK; }
static int dummy_xCommit(sqlite3_vtab *pVtab) { return SQLITE_OK; }
static int dummy_xRollback(sqlite3_vtab *pVtab) { return SQLITE_OK; }
static int dummy_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) { 
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor)); 
    if (*ppCursor) memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK; 
}
static int dummy_xClose(sqlite3_vtab_cursor *pCursor) { free(pCursor); return SQLITE_OK; }

static const sqlite3_module dummy_module = {
    0, /* iVersion */
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
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xFindFunction */
    NULL, /* xRename */
};

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the dummy module */
    rc = sqlite3_create_module(db, "dummy_vtab", &dummy_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create the virtual table */
    rc = sqlite3_exec(db, "CREATE TABLE t1 USING dummy_vtab(a INTEGER, b TEXT)", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * 2. Fuzzing Logic: 
     * We construct SQL queries based on the input to trigger xBestIndex,
     * which in turn calls the target APIs.
     * We also explicitly call sqlite3_overload_function and sqlite3_vtab_config
     * where appropriate.
     */

    /* Call sqlite3_overload_function directly with fuzzed parameters */
    if (Size > 0) {
        /* Ensure null termination for function name */
        char funcName[256];
        size_t len = (Size < 255) ? Size : 255;
        memcpy(funcName, Data, len);
        funcName[len] = '\0';
        
        int nArg = (Size > 1) ? (int)Data[1] : 0;
        
        /* Fuzz sqlite3_overload_function */
        sqlite3_overload_function(db, funcName, nArg);
    }

    /* 
     * Construct a query to trigger xBestIndex.
     * The input data is used to generate random WHERE clauses to exercise 
     * sqlite3_vtab_rhs_value, sqlite3_vtab_in, sqlite3_vtab_distinct, 
     * and sqlite3_vtab_collation inside the xBestIndex callback.
     */
    
    /* We use a simple query that forces the optimizer to call xBestIndex */
    /* The specific constraints depend on the input bytes to vary the state */
    char *sql = NULL;
    int sql_len = 1024;
    sql = malloc(sql_len);
    if (!sql) {
        sqlite3_close(db);
        return 0;
    }

    if (Size > 0) {
        /* Generate a query based on input bytes */
        int op = Data[0] % 4;
        switch (op) {
            case 0:
                /* Simple equality constraint */
                snprintf(sql, sql_len, "SELECT * FROM t1 WHERE a = %d", (int)Data[0]);
                break;
            case 1:
                /* IN clause to test sqlite3_vtab_in */
                snprintf(sql, sql_len, "SELECT * FROM t1 WHERE a IN (%d, %d, %d)", 
                         (int)Data[0], (int)(Size > 1 ? Data[1] : 0), (int)(Size > 2 ? Data[2] : 0));
                break;
            case 2:
                /* DISTINCT query to test sqlite3_vtab_distinct */
                snprintf(sql, sql_len, "SELECT DISTINCT * FROM t1 WHERE a > %d", (int)Data[0]);
                break;
            case 3:
                /* COLLATE to test sqlite3_vtab_collation */
                snprintf(sql, sql_len, "SELECT * FROM t1 WHERE b COLLATE BINARY = '%c'", (char)Data[0]);
                break;
        }
    } else {
        snprintf(sql, sql_len, "SELECT * FROM t1");
    }

    /* Execute the query to trigger xBestIndex */
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Process rows if any */
        }
        sqlite3_finalize(stmt);
    }

    /* Cleanup */
    free(sql);
    sqlite3_exec(db, "DROP TABLE t1", NULL, NULL, NULL);
    sqlite3_close(db);

    return 0;
}