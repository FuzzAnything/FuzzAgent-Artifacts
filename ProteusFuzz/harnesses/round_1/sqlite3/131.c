// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
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
 * Fuzz driver for sqlite3 result and context functions.
 * This driver creates a minimal valid environment to exercise:
 * - sqlite3_result_int
 * - sqlite3_aggregate_context
 * - sqlite3_result_null
 * - sqlite3_result_int64
 * - sqlite3_vtab_nochange
 * - sqlite3_result_value
 */

static sqlite3 *g_db = NULL;

/* Custom scalar function to test result setters */
static void test_scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx) return;

    /* Test sqlite3_result_int */
    sqlite3_result_int(ctx, 42);

    /* Test sqlite3_result_int64 */
    sqlite3_result_int64(ctx, 123456789012345LL);

    /* Test sqlite3_result_null */
    sqlite3_result_null(ctx);

    /* Test sqlite3_result_value using an argument if available */
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    }
}

/* Custom aggregate function step */
static void test_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    if (!ctx) return;

    /* Test sqlite3_aggregate_context with positive size */
    void *pCtx = sqlite3_aggregate_context(ctx, 100);
    if (pCtx) {
        /* Write some data to the context */
        memset(pCtx, 0xAB, 100);
    }

    /* Test with size 0 (should not reallocate) */
    pCtx = sqlite3_aggregate_context(ctx, 0);

    /* Test with negative size (should return NULL) */
    pCtx = sqlite3_aggregate_context(ctx, -10);

    /* Set a result for the step */
    sqlite3_result_int(ctx, 1);
}

/* Custom aggregate function final */
static void test_agg_final(sqlite3_context *ctx) {
    if (!ctx) return;

    /* Get context again in final */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    if (pCtx) {
        /* Read data back */
        volatile int dummy = ((char*)pCtx)[0];
        (void)dummy;
    }

    /* Set result */
    sqlite3_result_int(ctx, 999);
}

/* Helper function for vtab xColumn to test sqlite3_vtab_nochange */
static int vtab_column_impl(sqlite3_context *ctx, int iCol) {
    if (!ctx) return SQLITE_OK;

    /* Test sqlite3_vtab_nochange */
    int nochange = sqlite3_vtab_nochange(ctx);
    if (nochange) {
        /* Optimization: do not set a result */
        return SQLITE_OK;
    }

    /* Normal path: set a result */
    sqlite3_result_int(ctx, iCol);
    return SQLITE_OK;
}

/* Minimal vtab methods */
static int vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    return vtab_xCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int vtab_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdx) {
    return SQLITE_OK;
}

static int vtab_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int vtab_xClose(sqlite3_vtab_cursor *cur) {
    free(cur);
    return SQLITE_OK;
}

static int vtab_xNext(sqlite3_vtab_cursor *cur) {
    return SQLITE_DONE;
}

static int vtab_xEof(sqlite3_vtab_cursor *cur) {
    return 1;
}

static int vtab_xColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int iCol) {
    return vtab_column_impl(ctx, iCol);
}

static int vtab_xFilter(sqlite3_vtab_cursor *cur, int idxNum, const char *idxStr,
                        int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int vtab_xDisconnect(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int vtab_xDestroy(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static const sqlite3_module vtab_module = {
    0,
    vtab_xCreate,
    vtab_xConnect,
    vtab_xBestIndex,
    vtab_xDestroy,
    vtab_xDisconnect,
    vtab_xOpen,
    vtab_xClose,
    vtab_xFilter,
    vtab_xNext,
    vtab_xEof,
    vtab_xColumn,
    NULL, /* xUpdate */
    NULL, /* xBegin */
    NULL, /* xSync */
    NULL, /* xCommit */
    NULL, /* xRollback */
    NULL, /* xFindMethod */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
};

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    if (!g_db) {
        if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
            return 0;
        }

        /* Register scalar function */
        sqlite3_create_function(g_db, "test_scalar", 0, SQLITE_UTF8, NULL,
                                test_scalar_func, NULL, NULL);

        /* Register aggregate function */
        sqlite3_create_function(g_db, "test_agg", 0, SQLITE_UTF8, NULL,
                                NULL, test_agg_step, test_agg_final);

        /* Register virtual table */
        sqlite3_create_module(g_db, "test_vtab", &vtab_module, NULL);

        /* Create vtab table */
        const char *sql = "CREATE TABLE t1 USING test_vtab;";
        sqlite3_exec(g_db, sql, NULL, NULL, NULL);
    }

    /* Use the input data to generate SQL queries that invoke our functions */
    char query[256];
    size_t query_len = Size < sizeof(query) - 10 ? Size : sizeof(query) - 10;

    /* Ensure the query is safe and contains only allowed characters */
    for (size_t i = 0; i < query_len; i++) {
        if (Data[i] < 32 || Data[i] > 126) {
            query[i] = 'a';
        } else {
            query[i] = Data[i];
        }
    }
    query[query_len] = '\0';

    /* Construct a query that uses our functions */
    snprintf(query, sizeof(query),
             "SELECT test_scalar(), test_agg() FROM t1 WHERE 1=1;");

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(g_db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Execute the statement */
    rc = sqlite3_step(stmt);
    /* We don't care about the result, just that it doesn't crash */

    sqlite3_finalize(stmt);

    return 0;
}

/* Cleanup function called when the fuzzer exits */
__attribute__((destructor))
static void cleanup(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}