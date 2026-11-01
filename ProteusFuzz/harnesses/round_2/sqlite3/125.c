// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 API functions:
 * - sqlite3_result_null
 * - sqlite3_aggregate_context
 * - sqlite3_result_int
 * - sqlite3_vtab_nochange
 * - sqlite3_result_value
 * - sqlite3_result_int64
 */

static sqlite3 *g_db = NULL;
static int g_initialized = 0;

/* Custom scalar function: uses sqlite3_result_null, sqlite3_result_int, sqlite3_result_int64, sqlite3_result_value */
static void scalarFunc(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    /* Call sqlite3_result_null */
    sqlite3_result_null(ctx);
    
    /* Call sqlite3_result_int */
    sqlite3_result_int(ctx, 42);
    
    /* Call sqlite3_result_int64 */
    sqlite3_result_int64(ctx, 9223372036854775807LL);
    
    /* Call sqlite3_result_value if we have an argument */
    if (argc > 0 && argv[0]) {
        sqlite3_result_value(ctx, argv[0]);
    }
}

/* Custom aggregate function step: uses sqlite3_aggregate_context */
static void aggStep(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;
    
    /* Allocate aggregate context */
    void *pCtx = sqlite3_aggregate_context(ctx, 100);
    if (pCtx) {
        int *counter = (int *)pCtx;
        (*counter)++;
    }
}

/* Custom aggregate function final: uses sqlite3_aggregate_context and result functions */
static void aggFinal(sqlite3_context *ctx) {
    /* Get context, size 0 to avoid reallocation */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    
    if (pCtx) {
        int *counter = (int *)pCtx;
        sqlite3_result_int(ctx, *counter);
    } else {
        sqlite3_result_int(ctx, 0);
    }
}

/* Custom virtual table xColumn method: uses sqlite3_vtab_nochange */
static int vtabColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int iCol) {
    (void)cur;
    
    /* Check if this column is unchanged in an UPDATE */
    int nochange = sqlite3_vtab_nochange(ctx);
    
    if (nochange) {
        /* Optimization: Do not set a result. */
        return SQLITE_OK;
    }
    
    /* Normal case: return a value */
    sqlite3_result_int(ctx, iCol);
    return SQLITE_OK;
}

/* Custom virtual table xUpdate method */
static int vtabUpdate(sqlite3_vtab *v, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)v;
    (void)argc;
    (void)argv;
    (void)pRowid;
    return SQLITE_OK;
}

/* Virtual table methods */
static int vtabCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                      sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db;
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;
    
    sqlite3_vtab *vtab = malloc(sizeof(sqlite3_vtab));
    if (!vtab) return SQLITE_NOMEM;
    
    memset(vtab, 0, sizeof(sqlite3_vtab));
    
    *ppVtab = vtab;
    return SQLITE_OK;
}

static int vtabConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                       sqlite3_vtab **ppVtab, char **pzErr) {
    return vtabCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int vtabDisconnect(sqlite3_vtab *vtab) {
    free(vtab);
    return SQLITE_OK;
}

static int vtabDestroy(sqlite3_vtab *vtab) {
    free(vtab);
    return SQLITE_OK;
}

/* Placeholder for missing methods */
static int vtabBestIndex(sqlite3_vtab *v, sqlite3_index_info *idx) { return SQLITE_OK; }
static int vtabOpen(sqlite3_vtab *v, sqlite3_vtab_cursor **ppCur) { return SQLITE_OK; }
static int vtabClose(sqlite3_vtab_cursor *cur) { return SQLITE_OK; }
static int vtabFilter(sqlite3_vtab_cursor *cur, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) { return SQLITE_OK; }
static int vtabNext(sqlite3_vtab_cursor *cur) { return SQLITE_OK; }
static int vtabEof(sqlite3_vtab_cursor *cur) { return SQLITE_OK; }
static int vtabRowid(sqlite3_vtab_cursor *cur, sqlite3_int64 *pRowid) { return SQLITE_OK; }
static int vtabBegin(sqlite3_vtab *v) { return SQLITE_OK; }
static int vtabSync(sqlite3_vtab *v) { return SQLITE_OK; }
static int vtabCommit(sqlite3_vtab *v) { return SQLITE_OK; }
static int vtabRollback(sqlite3_vtab *v) { return SQLITE_OK; }
static int vtabSavepoint(sqlite3_vtab *v, int iSavepoint) { return SQLITE_OK; }
static int vtabRelease(sqlite3_vtab *v, int iSavepoint) { return SQLITE_OK; }
static int vtabRollbackTo(sqlite3_vtab *v, int iSavepoint) { return SQLITE_OK; }

/* 
 * xFindMethod signature matches sqlite3_module definition:
 * int (*xFindMethod)(sqlite3_vtab *pVtab, int nArg, const char *zArg,
 *                    void (**pxFunc)(sqlite3_context*,int,sqlite3_value**), void **pArg);
 */
static int vtabFindMethod(sqlite3_vtab *pVtab, int nArg, const char *zArg,
                          void (**pxFunc)(sqlite3_context*,int,sqlite3_value**), void **pArg) {
    (void)pVtab;
    (void)nArg;
    (void)zArg;
    (void)pxFunc;
    (void)pArg;
    return SQLITE_OK;
}

static int vtabRename(sqlite3_vtab *v, const char *zNew) { return SQLITE_OK; }

/* Virtual table module */
static const sqlite3_module vtabModule = {
    0,                  /* iVersion */
    vtabCreate,         /* xCreate */
    vtabConnect,        /* xConnect */
    vtabBestIndex,      /* xBestIndex */
    vtabDisconnect,     /* xDisconnect */
    vtabDestroy,        /* xDestroy */
    vtabOpen,           /* xOpen */
    vtabClose,          /* xClose */
    vtabFilter,         /* xFilter */
    vtabNext,           /* xNext */
    vtabEof,            /* xEof */
    vtabColumn,         /* xColumn */
    vtabRowid,          /* xRowid */
    vtabUpdate,         /* xUpdate */
    vtabBegin,          /* xBegin */
    vtabSync,           /* xSync */
    vtabCommit,         /* xCommit */
    vtabRollback,       /* xRollback */
    vtabFindMethod,     /* xFindMethod */
    vtabRename,         /* xRename */
    vtabSavepoint,      /* xSavepoint */
    vtabRelease,        /* xRelease */
    vtabRollbackTo      /* xRollbackTo */
};

static int initDatabase(void) {
    if (g_initialized) return SQLITE_OK;
    
    int rc = sqlite3_open(":memory:", &g_db);
    if (rc != SQLITE_OK) {
        return rc;
    }
    
    /* Create a table */
    const char *sql = "CREATE TABLE test_table (id INTEGER PRIMARY KEY, value TEXT);";
    rc = sqlite3_exec(g_db, sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return rc;
    }
    
    /* Register custom scalar function */
    rc = sqlite3_create_function(g_db, "fuzz_scalar", -1, SQLITE_UTF8, NULL, scalarFunc, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return rc;
    }
    
    /* Register custom aggregate function */
    rc = sqlite3_create_function(g_db, "fuzz_agg", -1, SQLITE_UTF8, NULL, NULL, aggStep, aggFinal);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return rc;
    }
    
    /* Register virtual table module */
    rc = sqlite3_create_module(g_db, "fuzz_vtab", &vtabModule, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return rc;
    }
    
    /* Create a virtual table */
    const char *vtab_sql = "CREATE TABLE fuzz_vtab_table (col1 TEXT, col2 INTEGER) USING fuzz_vtab;";
    rc = sqlite3_exec(g_db, vtab_sql, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return rc;
    }
    
    g_initialized = 1;
    return SQLITE_OK;
}

static void cleanupDatabase(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
        g_initialized = 0;
    }
}

/* Helper to generate SQL from input data */
static int executeFuzzSql(const uint8_t *data, size_t size) {
    if (!g_db) return -1;
    
    char *sql_buf = malloc(size + 200);
    if (!sql_buf) return -1;
    
    int type = 0;
    if (size > 0) {
        type = data[0] % 4;
    }
    
    switch (type) {
        case 0:
            /* SELECT fuzz_scalar */
            snprintf(sql_buf, size + 200, "SELECT fuzz_scalar(%d)", size);
            break;
        case 1:
            /* SELECT fuzz_agg */
            snprintf(sql_buf, size + 200, "SELECT fuzz_agg(*) FROM test_table");
            break;
        case 2:
            /* UPDATE virtual table to trigger vtabColumn and vtab_nochange */
            snprintf(sql_buf, size + 200, "UPDATE fuzz_vtab_table SET col1 = 'test' WHERE col2 = 1");
            break;
        case 3:
            /* INSERT into regular table */
            snprintf(sql_buf, size + 200, "INSERT INTO test_table (value) VALUES ('%.*s')", (int)size, data);
            break;
    }
    
    char *err_msg = NULL;
    int rc = sqlite3_exec(g_db, sql_buf, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
    }
    
    free(sql_buf);
    return 0;
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (!g_initialized) {
        if (initDatabase() != SQLITE_OK) {
            return 0;
        }
    }
    
    executeFuzzSql(Data, Size);
    
    return 0;
}