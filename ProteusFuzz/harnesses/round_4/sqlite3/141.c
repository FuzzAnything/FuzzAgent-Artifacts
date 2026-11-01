// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Static helper to create a dummy virtual table module.
 * This is necessary because the target APIs (sqlite3_vtab_rhs_value, etc.)
 * are only valid inside an xBestIndex call of a virtual table.
 */
static int xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab;
    sqlite3_value *pVal = NULL;
    int i;
    
    /* Fuzz sqlite3_vtab_rhs_value */
    /* Try iterating through constraints to trigger different states */
    for (i = 0; i < pIdxInfo->nConstraint; i++) {
        /* Attempt to retrieve RHS value */
        int rc = sqlite3_vtab_rhs_value(pIdxInfo, i, &pVal);
        (void)rc; /* Suppress unused warning, fuzzing checks side effects */
        (void)pVal;
    }

    /* Fuzz sqlite3_vtab_in */
    /* Check feasibility (-1) and request handling (1) or decline (0) */
    for (i = 0; i < pIdxInfo->nConstraint; i++) {
        int rc1 = sqlite3_vtab_in(pIdxInfo, i, -1);
        int rc2 = sqlite3_vtab_in(pIdxInfo, i, 1);
        int rc3 = sqlite3_vtab_in(pIdxInfo, i, 0);
        (void)rc1;
        (void)rc2;
        (void)rc3;
    }

    /* Fuzz sqlite3_vtab_distinct */
    int distinct = sqlite3_vtab_distinct(pIdxInfo);
    (void)distinct;

    /* Fuzz sqlite3_vtab_collation */
    for (i = 0; i < pIdxInfo->nConstraint; i++) {
        const char *coll = sqlite3_vtab_collation(pIdxInfo, i);
        (void)coll;
    }

    /* Set some basic output to satisfy the contract of xBestIndex */
    pIdxInfo->idxNum = 0;
    pIdxInfo->estimatedCost = 1.0;
    pIdxInfo->estimatedRows = 1;
    
    return SQLITE_OK;
}

static int xConnect(sqlite3 *db, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db;
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;
    
    /* Fuzz sqlite3_vtab_config during xConnect */
    int rc = sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    (void)rc;
    rc = sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 0);
    (void)rc;
    
    /* Allocate dummy vtab */
    *ppVtab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVtab) {
        memset(*ppVtab, 0, sizeof(sqlite3_vtab));
        /* Set module pointer to a dummy value to avoid null pointer issues if accessed */
        (*ppVtab)->pModule = NULL; 
    }
    return SQLITE_OK;
}

static int xCreate(sqlite3 *db, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVtab, char **pzErr) {
    /* Reuse xConnect logic for xCreate */
    return xConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

static int xDisconnect(sqlite3_vtab *pVtab) {
    if (pVtab) {
        sqlite3_free(pVtab);
    }
    return SQLITE_OK;
}

static int xDestroy(sqlite3_vtab *pVtab) {
    if (pVtab) {
        sqlite3_free(pVtab);
    }
    return SQLITE_OK;
}

static int xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    /* Allocate a dummy cursor */
    *ppCursor = (sqlite3_vtab_cursor *)sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor) {
        memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    }
    return SQLITE_OK;
}

static int xClose(sqlite3_vtab_cursor *cur) {
    if (cur) {
        sqlite3_free(cur);
    }
    return SQLITE_OK;
}

/* 
 * Fixed signature for xFilter to match sqlite3_module definition:
 * int (*xFilter)(sqlite3_vtab_cursor*, int, const char*, int, sqlite3_value**);
 */
static int xFilter(sqlite3_vtab_cursor *v, int idxNum, const char *zIdxStr, int nArg, sqlite3_value **argv) {
    (void)v;
    (void)idxNum;
    (void)zIdxStr;
    (void)nArg;
    (void)argv;
    return SQLITE_OK;
}

static int xNext(sqlite3_vtab_cursor *cur) {
    (void)cur;
    return SQLITE_DONE;
}

static int xEof(sqlite3_vtab_cursor *cur) {
    (void)cur;
    return 1;
}

static int xColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i) {
    (void)cur;
    (void)ctx;
    (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int xRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid) {
    (void)cur;
    *pRowid = 0;
    return SQLITE_OK;
}

/* 
 * Fixed signature for xUpdate to match sqlite3_module definition:
 * int (*xUpdate)(sqlite3_vtab*, int, sqlite3_value**, sqlite_int64*);
 */
static int xUpdate(sqlite3_vtab *v, int nArg, sqlite3_value **argv, sqlite_int64 *pRowid) {
    (void)v;
    (void)nArg;
    (void)argv;
    (void)pRowid;
    return SQLITE_OK;
}

static int xBegin(sqlite3_vtab *v) {
    (void)v;
    return SQLITE_OK;
}

static int xSync(sqlite3_vtab *v) {
    (void)v;
    return SQLITE_OK;
}

static int xCommit(sqlite3_vtab *v) {
    (void)v;
    return SQLITE_OK;
}

static int xRollback(sqlite3_vtab *v) {
    (void)v;
    return SQLITE_OK;
}

static const sqlite3_module dummy_module = {
    0, /* iVersion */
    xCreate,
    xConnect,
    xBestIndex,
    xDisconnect,
    xDestroy,
    xOpen,
    xClose,
    xFilter,
    xNext,
    xEof,
    xColumn,
    xRowid,
    xUpdate,
    xBegin,
    xSync,
    xCommit,
    xRollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
};

int LLVMFuzzerTestOneInput_141(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* 1. Prepare the environment */
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create the dummy virtual table module */
    rc = sqlite3_create_module(db, "dummy_vtab", &dummy_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * 2. Explore program states 
     * We need to trigger xBestIndex to fuzz the target APIs.
     * We construct a SQL query using the input data to drive the planner.
     */
    
    /* Create a dummy table to query */
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE test_table USING dummy_vtab(id INTEGER, val TEXT);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 
     * Construct a query that forces the planner to call xBestIndex.
     * We use the input data to generate random SQL fragments to explore paths.
     */
    char sql[4096];
    size_t sql_len = sizeof(sql) - 1;
    
    /* Ensure we don't overflow */
    size_t data_len = (Size < sql_len) ? Size : sql_len;
    
    /* Create a safe SQL string based on input */
    /* We use a simple query structure: SELECT * FROM test_table WHERE ... */
    /* The input data is used to form the WHERE clause constraints */
    
    int offset = 0;
    int sql_pos = 0;
    
    /* Start with base query */
    const char *base = "SELECT * FROM test_table WHERE ";
    int base_len = strlen(base);
    
    if (sql_pos + base_len < sql_len) {
        memcpy(sql + sql_pos, base, base_len);
        sql_pos += base_len;
    }

    /* Add constraints based on input bytes */
    /* We try to generate various operators and values to trigger different constraint types */
    while (offset < data_len && sql_pos < sql_len - 20) {
        int op = Data[offset] % 6; // 0-5
        int col = (Data[offset+1] % 2) + 1; // 1 or 2 (id or val)
        
        const char *ops[] = {"id = ", "val = ", "id > ", "val < ", "id IS ", "val IS NOT "};
        const char *vals[] = {"1", "'a'", "10", "5", "NULL", "NULL"};
        
        const char *op_str = ops[op];
        const char *val_str = vals[op];
        
        /* Add column name if not in op_str (simplified logic) */
        if (col == 2 && (op == 0 || op == 2)) {
             /* Adjust for val column */
             op_str = (op == 0) ? "val = " : "val > ";
        }
        
        int op_len = strlen(op_str);
        int val_len = strlen(val_str);
        
        if (sql_pos + op_len + val_len + 2 < sql_len) {
            memcpy(sql + sql_pos, op_str, op_len);
            sql_pos += op_len;
            memcpy(sql + sql_pos, val_str, val_len);
            sql_pos += val_len;
            
            /* Add AND if more data */
            if (offset + 2 < data_len) {
                memcpy(sql + sql_pos, " AND ", 5);
                sql_pos += 5;
            }
        }
        
        offset += 2;
        if (offset >= data_len) break;
    }
    
    sql[sql_pos] = '\0';

    /* Execute the query to trigger xBestIndex */
    sqlite3_stmt *stmt = NULL;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    
    if (rc == SQLITE_OK) {
        /* Step through the statement to ensure execution */
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Do nothing, just iterate */
        }
        sqlite3_finalize(stmt);
    }

    /* 
     * Fuzz sqlite3_overload_function directly 
     * This function does not require xBestIndex context.
     */
    if (Size > 0) {
        char func_name[64];
        int n_arg = (Data[0] % 10) + 1;
        size_t fn_len = (Size > 63) ? 63 : Size;
        memcpy(func_name, Data, fn_len);
        func_name[fn_len] = '\0';
        
        /* Ensure it's a valid identifier (replace non-alnum with 'a') */
        for (int i = 0; i < fn_len; i++) {
            if (!((func_name[i] >= 'a' && func_name[i] <= 'z') || 
                  (func_name[i] >= 'A' && func_name[i] <= 'Z') || 
                  (func_name[i] >= '0' && func_name[i] <= '9') || 
                  func_name[i] == '_')) {
                func_name[i] = 'a';
            }
        }
        
        rc = sqlite3_overload_function(db, func_name, n_arg);
        (void)rc;
    }

    /* 3. Cleanup */
    sqlite3_exec(db, "DROP TABLE IF EXISTS test_table;", NULL, NULL, NULL);
    sqlite3_close(db);

    return 0;
}