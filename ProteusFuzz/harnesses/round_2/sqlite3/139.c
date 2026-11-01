// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_value at sqlite3.c:92899:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzzing Driver for SQLite3 API Functions
 * Target APIs:
 * 1. sqlite3_result_null
 * 2. sqlite3_aggregate_context
 * 3. sqlite3_result_int
 * 4. sqlite3_vtab_nochange
 * 5. sqlite3_result_value
 * 6. sqlite3_result_int64
 */

/* Global database handle */
static sqlite3 *g_db = NULL;

/* Custom Scalar Function: Tests sqlite3_result_null, sqlite3_result_int, sqlite3_result_int64, sqlite3_result_value */
static void scalar_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* Test 1: sqlite3_result_null */
    sqlite3_result_null(ctx);

    /* Test 2: sqlite3_result_int */
    sqlite3_result_int(ctx, 12345);

    /* Test 3: sqlite3_result_int64 */
    sqlite3_result_int64(ctx, 9223372036854775807LL);

    /* Test 4: sqlite3_result_value (using first argument if available) */
    if (argc > 0) {
        sqlite3_result_value(ctx, argv[0]);
    }
}

/* Custom Aggregate Function Step: Tests sqlite3_aggregate_context */
static void agg_func_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) return;

    /* Test: sqlite3_aggregate_context with varying sizes based on input */
    void *pCtx = sqlite3_aggregate_context(ctx, 64);
    if (pCtx) {
        int *state = (int*)pCtx;
        *state = 1;
    }
    
    /* Also test with 0 bytes */
    void *pCtxZero = sqlite3_aggregate_context(ctx, 0);
    (void)pCtxZero;
}

/* Custom Aggregate Function Final: Tests sqlite3_aggregate_context (nBytes=0) and result setters */
static void agg_func_final(sqlite3_context *ctx) {
    if (!ctx) return;

    /* Test: sqlite3_aggregate_context with nBytes=0 */
    void *pCtx = sqlite3_aggregate_context(ctx, 0);
    if (pCtx) {
        int *state = (int*)pCtx;
        sqlite3_result_int(ctx, *state);
    } else {
        sqlite3_result_null(ctx);
    }
}

/* Custom Virtual Table Implementation */
typedef struct MyVtab MyVtab;
struct MyVtab {
    sqlite3_vtab base;
};

/* xCreate and xConnect are identical for this simple implementation */
static int myVtabCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
    MyVtab *p = (MyVtab*)sqlite3_malloc(sizeof(MyVtab));
    if (!p) return SQLITE_NOMEM;
    memset(p, 0, sizeof(MyVtab));
    *ppVtab = (sqlite3_vtab*)p;
    return SQLITE_OK;
}

static int myVtabConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVtab, char **pzErr) {
    return myVtabCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int myVtabDisconnect(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int myVtabDestroy(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int myVtabBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdx) {
    (void)pVtab; (void)pIdx;
    return SQLITE_OK;
}

static int myVtabOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    *ppCursor = NULL; 
    return SQLITE_OK;
}

static int myVtabClose(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int myVtabFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}

static int myVtabNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_DONE;
}

static int myVtabEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int myVtabColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor; (void)i;
    if (!ctx) return SQLITE_OK;

    /* Test: sqlite3_vtab_nochange */
    int noChange = sqlite3_vtab_nochange(ctx);
    (void)noChange;

    /* Return a dummy value to satisfy the query */
    sqlite3_result_int(ctx, 0);
    return SQLITE_OK;
}

static int myVtabRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 1;
    return SQLITE_OK;
}

static int myVtabUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVtab; (void)argc; (void)argv; (void)pRowid;
    return SQLITE_OK;
}

static int myVtabBegin(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int myVtabSync(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int myVtabCommit(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int myVtabRollback(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static const sqlite3_module myVtabModule = {
    0,                          /* iVersion */
    myVtabCreate,               /* xCreate */
    myVtabConnect,              /* xConnect */
    myVtabBestIndex,            /* xBestIndex */
    myVtabDisconnect,           /* xDisconnect */
    myVtabDestroy,              /* xDestroy */
    myVtabOpen,                 /* xOpen */
    myVtabClose,                /* xClose */
    myVtabFilter,               /* xFilter */
    myVtabNext,                 /* xNext */
    myVtabEof,                  /* xEof */
    myVtabColumn,               /* xColumn */
    myVtabRowid,                /* xRowid */
    myVtabUpdate,               /* xUpdate */
    myVtabBegin,                /* xBegin */
    myVtabSync,                 /* xSync */
    myVtabCommit,               /* xCommit */
    myVtabRollback,             /* xRollback */
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
};

/* Helper to initialize the database and register functions */
static int init_fuzz_env(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }

    int rc = sqlite3_open(":memory:", &g_db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Register Scalar Function */
    rc = sqlite3_create_function(g_db, "fuzz_scalar", -1, SQLITE_UTF8, NULL, scalar_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return 0;
    }

    /* Register Aggregate Function */
    rc = sqlite3_create_function(g_db, "fuzz_agg", -1, SQLITE_UTF8, NULL, NULL, agg_func_step, agg_func_final);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return 0;
    }

    /* Register Virtual Table */
    rc = sqlite3_create_module(g_db, "fuzz_vtab", &myVtabModule, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(g_db);
        g_db = NULL;
        return 0;
    }

    return 1;
}

/* Helper to cleanup */
static void cleanup_fuzz_env(void) {
    if (g_db) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}

/*
 * LLVM Fuzzer Entry Point
 */
int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    /* Ensure environment is initialized */
    if (!init_fuzz_env()) {
        return 0;
    }

    /* Prepare a safe SQL query string based on input size */
    size_t max_query_len = 1024;
    if (Size > max_query_len) Size = max_query_len;

    /* Construct SQL queries to trigger the target APIs */
    char query[2048];
    int query_len = 0;

    /* Query 1: Test Scalar Function (result_null, result_int, result_int64, result_value) */
    query_len = snprintf(query, sizeof(query), 
        "SELECT fuzz_scalar('%.*s');", 
        (int)Size, Data ? (char*)Data : "");

    /* Execute Query 1 */
    char *zErrMsg = 0;
    sqlite3_exec(g_db, query, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    /* Query 2: Test Aggregate Function (aggregate_context) */
    sqlite3_exec(g_db, "CREATE TABLE IF NOT EXISTS fuzz_data (val TEXT);", NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    /* Insert fuzzed data into the table */
    char insert_query[1024];
    int insert_len = snprintf(insert_query, sizeof(insert_query),
        "INSERT INTO fuzz_data VALUES ('%.*s');",
        (int)(Size > 200 ? 200 : Size), Data ? (char*)Data : "");
    
    sqlite3_exec(g_db, insert_query, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    /* Run aggregate query */
    query_len = snprintf(query, sizeof(query), "SELECT fuzz_agg(val) FROM fuzz_data;");
    sqlite3_exec(g_db, query, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    /* Query 3: Test Virtual Table (vtab_nochange) */
    sqlite3_exec(g_db, "CREATE VIRTUAL TABLE IF NOT EXISTS fuzz_vt USING fuzz_vtab;", NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    /* Select from vtab to trigger xColumn */
    query_len = snprintf(query, sizeof(query), "SELECT * FROM fuzz_vt;");
    sqlite3_exec(g_db, query, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    /* Attempt an UPDATE to potentially trigger the no-change logic in xColumn */
    query_len = snprintf(query, sizeof(query), "UPDATE fuzz_vt SET col1 = 1 WHERE 0;");
    sqlite3_exec(g_db, query, NULL, NULL, &zErrMsg);
    if (zErrMsg) sqlite3_free(zErrMsg);

    /* Cleanup */
    cleanup_fuzz_env();

    return 0;
}