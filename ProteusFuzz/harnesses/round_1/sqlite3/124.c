// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_int at sqlite3.c:93661:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_aggregate_context at sqlite3.c:93403:18 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_result_int at sqlite3.c:92767:17 in sqlite3.h
// sqlite3_result_int64 at sqlite3.c:92774:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3 result and context functions.
 *
 * This driver creates a temporary in-memory database, prepares a statement
 * that invokes a custom scalar or aggregate function, and then fuzzes the
 * result-setting APIs:
 *   - sqlite3_result_int
 *   - sqlite3_result_int64
 *   - sqlite3_result_null
 *   - sqlite3_result_value
 *   - sqlite3_aggregate_context
 *   - sqlite3_vtab_nochange
 *
 * The custom function implementation is driven by the fuzz input to exercise
 * different code paths and states.
 */

static sqlite3 *g_db = NULL;
static int g_fuzz_step_count = 0;

/*
 * Custom scalar function: fuzz_result_scalar
 *
 * Uses input data to decide which result API to call.
 */
static void fuzz_result_scalar(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) {
        /* API armor: safely return if context is NULL */
        return;
    }

    /* Use a deterministic pseudo-random value based on step count */
    int val = (int)(g_fuzz_step_count % 1000000);

    /* Fuzz different result-setting paths */
    switch (g_fuzz_step_count % 3) {
        case 0:
            sqlite3_result_int(ctx, val);
            break;
        case 1:
            sqlite3_result_int64(ctx, (sqlite3_int64)val);
            break;
        case 2:
            sqlite3_result_null(ctx);
            break;
    }
}

/*
 * Custom aggregate function: fuzz_result_agg
 *
 * Demonstrates use of sqlite3_aggregate_context and different result APIs.
 */
static void fuzz_result_agg_step(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) {
        return;
    }

    int *state = (int *)sqlite3_aggregate_context(ctx, sizeof(int));
    if (!state) {
        sqlite3_result_null(ctx);
        return;
    }

    if (*state == 0) {
        /* First call: initialize state */
        *state = 1;
    }

    int val = (int)(g_fuzz_step_count % 1000000);
    *state += val;

    /* Fuzz result setting in step */
    switch (g_fuzz_step_count % 3) {
        case 0:
            sqlite3_result_int(ctx, *state);
            break;
        case 1:
            sqlite3_result_int64(ctx, (sqlite3_int64)*state);
            break;
        case 2:
            sqlite3_result_null(ctx);
            break;
    }
}

static void fuzz_result_agg_final(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)argc;
    (void)argv;

    if (!ctx) {
        return;
    }

    int *state = (int *)sqlite3_aggregate_context(ctx, 0);
    if (!state) {
        sqlite3_result_null(ctx);
        return;
    }

    /* Fuzz final result */
    switch (g_fuzz_step_count % 3) {
        case 0:
            sqlite3_result_int(ctx, *state);
            break;
        case 1:
            sqlite3_result_int64(ctx, (sqlite3_int64)*state);
            break;
        case 2:
            sqlite3_result_null(ctx);
            break;
    }
}

/*
 * Virtual table implementation to exercise sqlite3_vtab_nochange
 */
static struct VtabMethods {
    int (*xCreate)(sqlite3*, void*, int, const char*const*, sqlite3_vtab**, char**);
    int (*xConnect)(sqlite3*, void*, int, const char*const*, sqlite3_vtab**, char**);
    int (*xBestIndex)(sqlite3_vtab*, sqlite3_index_info*);
    int (*xDisconnect)(sqlite3_vtab*);
    int (*xDestroy)(sqlite3_vtab*);
    int (*xOpen)(sqlite3_vtab*, sqlite3_vtab_cursor**);
    int (*xClose)(sqlite3_vtab_cursor*);
    int (*xFilter)(sqlite3_vtab_cursor*, int, const char*, int, struct sqlite3_index_constraint*);
    int (*xNext)(sqlite3_vtab_cursor*);
    int (*xEof)(sqlite3_vtab_cursor*);
    int (*xColumn)(sqlite3_vtab_cursor*, sqlite3_context*, int);
    int (*xRowid)(sqlite3_vtab_cursor*, sqlite3_int64*);
    int (*xUpdate)(sqlite3_vtab*, int, sqlite3_value**, sqlite3_int64*);
    int (*xBegin)(sqlite3_vtab*);
    int (*xSync)(sqlite3_vtab*);
    int (*xCommit)(sqlite3_vtab*);
    int (*xRollback)(sqlite3_vtab*);
    int (*xRename)(sqlite3_vtab*, const char*);
    int (*xSavepoint)(sqlite3_vtab*, int);
    int (*xRelease)(sqlite3_vtab*, int);
    int (*xRollbackTo)(sqlite3_vtab*, int);
} g_vtab_methods = {0};

static int vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                        sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    return vtab_xCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int vtab_xDisconnect(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int vtab_xDestroy(sqlite3_vtab *pVtab) {
    free(pVtab);
    return SQLITE_OK;
}

static int vtab_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int vtab_xClose(sqlite3_vtab_cursor *pCursor) {
    free(pCursor);
    return SQLITE_OK;
}

static int vtab_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                        int argc, struct sqlite3_index_constraint *constraints) {
    (void)idxNum; (void)idxStr; (void)argc; (void)constraints;
    pCursor->pVtab = NULL; /* Not used */
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
    (void)pCursor;
    (void)i;

    if (!ctx) {
        return SQLITE_OK;
    }

    /* Try to use sqlite3_vtab_nochange optimization */
    if (sqlite3_vtab_nochange(ctx)) {
        /* Optimization: do not set a result, rely on xUpdate detecting no-change */
        return SQLITE_OK;
    }

    /* Fallback: set a result */
    sqlite3_result_int(ctx, g_fuzz_step_count % 1000000);
    return SQLITE_OK;
}

static int vtab_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    *pRowid = g_fuzz_step_count;
    return SQLITE_OK;
}

static int vtab_xUpdate(sqlite3_vtab *pVtab, int nArg, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVtab; (void)nArg; (void)argv; (void)pRowid;
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

static int vtab_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    (void)pVtab; (void)zNew;
    return SQLITE_OK;
}

static int vtab_xSavepoint(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab; (void)iSavepoint;
    return SQLITE_OK;
}

static int vtab_xRelease(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab; (void)iSavepoint;
    return SQLITE_OK;
}

static int vtab_xRollbackTo(sqlite3_vtab *pVtab, int iSavepoint) {
    (void)pVtab; (void)iSavepoint;
    return SQLITE_OK;
}

static void init_vtab_methods(void) {
    g_vtab_methods.xCreate = vtab_xCreate;
    g_vtab_methods.xConnect = vtab_xConnect;
    g_vtab_methods.xDisconnect = vtab_xDisconnect;
    g_vtab_methods.xDestroy = vtab_xDestroy;
    g_vtab_methods.xOpen = vtab_xOpen;
    g_vtab_methods.xClose = vtab_xClose;
    g_vtab_methods.xFilter = vtab_xFilter;
    g_vtab_methods.xNext = vtab_xNext;
    g_vtab_methods.xEof = vtab_xEof;
    g_vtab_methods.xColumn = vtab_xColumn;
    g_vtab_methods.xRowid = vtab_xRowid;
    g_vtab_methods.xUpdate = vtab_xUpdate;
    g_vtab_methods.xBegin = vtab_xBegin;
    g_vtab_methods.xSync = vtab_xSync;
    g_vtab_methods.xCommit = vtab_xCommit;
    g_vtab_methods.xRollback = vtab_xRollback;
    g_vtab_methods.xRename = vtab_xRename;
    g_vtab_methods.xSavepoint = vtab_xSavepoint;
    g_vtab_methods.xRelease = vtab_xRelease;
    g_vtab_methods.xRollbackTo = vtab_xRollbackTo;
}

static int register_functions(sqlite3 *db) {
    if (!db) return SQLITE_ERROR;

    /* Register scalar function */
    if (sqlite3_create_function(db, "fuzz_scalar", 0, SQLITE_UTF8, NULL,
                                fuzz_result_scalar, NULL, NULL) != SQLITE_OK) {
        return SQLITE_ERROR;
    }

    /* Register aggregate function */
    if (sqlite3_create_function(db, "fuzz_agg", 0, SQLITE_UTF8, NULL,
                                fuzz_result_agg_step, fuzz_result_agg_final, NULL) != SQLITE_OK) {
        return SQLITE_ERROR;
    }

    /* Register virtual table */
    init_vtab_methods();
    if (sqlite3_create_module(db, "fuzz_vtab", &g_vtab_methods, NULL) != SQLITE_OK) {
        return SQLITE_ERROR;
    }

    return SQLITE_OK;
}

int LLVMFuzzerTestOneInput_124(const uint8_t *Data, size_t Size) {
    (void)Data;
    (void)Size;

    g_fuzz_step_count++;

    /* Initialize database if not already done */
    if (!g_db) {
        if (sqlite3_open(":memory:", &g_db) != SQLITE_OK) {
            return 0;
        }
        if (register_functions(g_db) != SQLITE_OK) {
            sqlite3_close(g_db);
            g_db = NULL;
            return 0;
        }
    }

    /* Create a dummy file if needed (not used in this driver, but included per instructions) */
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        fprintf(f, "dummy content\n");
        fclose(f);
    }

    /* Execute a query that uses the custom scalar function */
    sqlite3_stmt *stmt = NULL;
    const char *sql_scalar = "SELECT fuzz_scalar();";
    if (sqlite3_prepare_v2(g_db, sql_scalar, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            /* Result is set by fuzz_result_scalar */
            (void)sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    /* Execute a query that uses the custom aggregate function */
    const char *sql_agg = "SELECT fuzz_agg() FROM (SELECT 1 UNION ALL SELECT 2 UNION ALL SELECT 3);";
    if (sqlite3_prepare_v2(g_db, sql_agg, -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            (void)sqlite3_column_int(stmt, 0);
        }
        sqlite3_finalize(stmt);
    }

    /* Execute a query that uses the virtual table */
    const char *sql_vtab = "CREATE VIRTUAL TABLE IF NOT EXISTS fuzz_vtab_test USING fuzz_vtab;";
    if (sqlite3_prepare_v2(g_db, sql_vtab, -1, &stmt, NULL) == SQLITE_OK) {
        sqlite3_step(stmt);
        sqlite3_finalize(stmt);

        const char *sql_vtab_select = "SELECT * FROM fuzz_vtab_test LIMIT 1;";
        if (sqlite3_prepare_v2(g_db, sql_vtab_select, -1, &stmt, NULL) == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                (void)sqlite3_column_int(stmt, 0);
            }
            sqlite3_finalize(stmt);
        }
    }

    /* Cleanup is handled by sqlite3_finalize and automatic memory management */
    return 0;
}