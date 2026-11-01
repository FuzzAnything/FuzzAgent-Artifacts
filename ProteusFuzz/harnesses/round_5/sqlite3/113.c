// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to open a database connection safely.
 */
static sqlite3 *open_db(const char *filename) {
    sqlite3 *db = NULL;
    int rc = sqlite3_open(filename, &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return NULL;
    }
    return db;
}

/*
 * Static helper to close a database connection.
 */
static void close_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Global storage for the module structure to prevent stack-use-after-return.
 * The module structure must persist for the lifetime of the database connection.
 */
static sqlite3_module g_dummy_module = {0};

/*
 * Static helper to create a dummy virtual table module for testing.
 * This module is minimal and designed to trigger the target APIs during xBestIndex.
 */
static int dummy_vtab_best_index_impl(sqlite3_vtab *pVtab, sqlite3_index_info *p) {
    (void)pVtab;
    
    /* Initialize output fields to avoid undefined behavior */
    p->idxNum = 0;
    p->idxStr = NULL;
    p->needToFreeIdxStr = 0;
    p->orderByConsumed = 0;
    p->estimatedCost = 1.0;
    p->estimatedRows = 1;
    p->idxFlags = 0;

    /* Fuzz sqlite3_vtab_collation */
    if (p->nConstraint > 0) {
        for (int i = 0; i < p->nConstraint; i++) {
            const char *coll = sqlite3_vtab_collation(p, i);
            if (coll) {
                /* Use the collation name to avoid unused variable warnings */
                (void)coll;
            }
        }
    }

    /* Fuzz sqlite3_vtab_distinct */
    int distinct = sqlite3_vtab_distinct(p);
    (void)distinct;

    /* Fuzz sqlite3_vtab_rhs_value */
    if (p->nConstraint > 0) {
        for (int i = 0; i < p->nConstraint; i++) {
            sqlite3_value *val = NULL;
            int rc = sqlite3_vtab_rhs_value(p, i, &val);
            if (rc == SQLITE_OK && val) {
                /* Value is valid only during xBestIndex, do not store it */
                (void)val;
            }
        }
    }

    /* Fuzz sqlite3_vtab_in */
    if (p->nConstraint > 0) {
        for (int i = 0; i < p->nConstraint; i++) {
            /* Check if it's an IN operator */
            int is_in = sqlite3_vtab_in(p, i, -1);
            if (is_in) {
                /* Request to handle all-at-once */
                sqlite3_vtab_in(p, i, 1);
            } else {
                /* Decline handling */
                sqlite3_vtab_in(p, i, 0);
            }
        }
    }

    /* Set a dummy constraint usage to allow the query to proceed */
    if (p->nConstraint > 0) {
        p->aConstraintUsage[0].argvIndex = 1;
        p->aConstraintUsage[0].omit = 0;
    }

    return SQLITE_OK;
}

static int dummy_vtab_filter_impl(sqlite3_vtab_cursor *pCsr, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pCsr;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    /* Minimal implementation: no rows returned */
    return SQLITE_OK;
}

static int dummy_vtab_next_impl(sqlite3_vtab_cursor *pCsr) {
    (void)pCsr;
    return SQLITE_DONE;
}

static int dummy_vtab_eof_impl(sqlite3_vtab_cursor *pCsr) {
    (void)pCsr;
    return 1;
}

static int dummy_vtab_column_impl(sqlite3_vtab_cursor *pCsr, sqlite3_context *ctx, int i) {
    (void)pCsr;
    (void)ctx;
    (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int dummy_vtab_rowid_impl(sqlite3_vtab_cursor *pCsr, sqlite3_int64 *pRowid) {
    (void)pCsr;
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_vtab_update_impl(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVtab;
    (void)argc;
    (void)argv;
    (void)pRowid;
    return SQLITE_OK;
}

static int dummy_vtab_bbegin_impl(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_vtab_bcommit_impl(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_vtab_rollback_impl(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int dummy_vtab_create_impl(sqlite3 *db, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db;
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    /* Fuzz sqlite3_vtab_config during xCreate */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);

    *ppVtab = (sqlite3_vtab *)malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int dummy_vtab_connect_impl(sqlite3 *db, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db;
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    /* Fuzz sqlite3_vtab_config during xConnect */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);

    *ppVtab = (sqlite3_vtab *)malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int dummy_vtab_disconnect_impl(sqlite3_vtab *pVtab) {
    if (pVtab) {
        free(pVtab);
    }
    return SQLITE_OK;
}

static int dummy_vtab_destroy_impl(sqlite3_vtab *pVtab) {
    if (pVtab) {
        free(pVtab);
    }
    return SQLITE_OK;
}

/*
 * Static helper to create the dummy virtual table module.
 * Uses the global g_dummy_module to avoid stack-use-after-return.
 */
static int create_dummy_module(sqlite3 *db) {
    /* Reset the global module structure */
    memset(&g_dummy_module, 0, sizeof(g_dummy_module));
    
    g_dummy_module.iVersion = 3; /* Ensure version is high enough to avoid checks */
    g_dummy_module.xCreate = dummy_vtab_create_impl;
    g_dummy_module.xConnect = dummy_vtab_connect_impl;
    g_dummy_module.xBestIndex = dummy_vtab_best_index_impl;
    g_dummy_module.xDisconnect = dummy_vtab_disconnect_impl;
    g_dummy_module.xDestroy = dummy_vtab_destroy_impl;
    g_dummy_module.xOpen = NULL; /* Not used in this test */
    g_dummy_module.xClose = NULL;
    g_dummy_module.xFilter = dummy_vtab_filter_impl;
    g_dummy_module.xNext = dummy_vtab_next_impl;
    g_dummy_module.xEof = dummy_vtab_eof_impl;
    g_dummy_module.xColumn = dummy_vtab_column_impl;
    g_dummy_module.xRowid = dummy_vtab_rowid_impl;
    g_dummy_module.xUpdate = dummy_vtab_update_impl;
    g_dummy_module.xBegin = dummy_vtab_bbegin_impl;
    g_dummy_module.xSync = NULL;
    g_dummy_module.xCommit = dummy_vtab_bcommit_impl;
    g_dummy_module.xRollback = dummy_vtab_rollback_impl;
    g_dummy_module.xFindFunction = NULL;
    g_dummy_module.xRename = NULL;
    g_dummy_module.xSavepoint = NULL;
    g_dummy_module.xRelease = NULL;
    g_dummy_module.xRollbackTo = NULL;

    return sqlite3_create_module(db, "dummy_vtab", &g_dummy_module, NULL);
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    sqlite3 *db = open_db(":memory:");
    if (!db) return 0;

    /* Create the dummy module using the global structure */
    if (create_dummy_module(db) != SQLITE_OK) {
        close_db(db);
        return 0;
    }

    /* Create the virtual table */
    const char *create_stmt = "CREATE VIRTUAL TABLE test_vtab USING dummy_vtab(a TEXT, b INTEGER);";
    char *err_msg = NULL;
    int rc = sqlite3_exec(db, create_stmt, NULL, NULL, &err_msg);
    if (rc != SQLITE_OK) {
        sqlite3_free(err_msg);
        close_db(db);
        return 0;
    }

    /* Fuzz sqlite3_overload_function */
    /* Use input data to generate function names and argument counts */
    if (Size >= 2) {
        char func_name[64];
        size_t name_len = Size < 63 ? Size : 63;
        memcpy(func_name, Data, name_len);
        func_name[name_len] = '\0';
        int n_arg = Data[name_len] % 10; /* 0 to 9 */
        sqlite3_overload_function(db, func_name, n_arg);
    }

    /* Prepare a query that triggers xBestIndex */
    /* We use the input data to vary the query parameters */
    char query[256];
    int query_len = 0;
    if (Size > 0) {
        int op = Data[0] % 5;
        int val = (int)Data[1];
        switch (op) {
            case 0:
                query_len = snprintf(query, sizeof(query), "SELECT * FROM test_vtab WHERE a = %d", val);
                break;
            case 1:
                query_len = snprintf(query, sizeof(query), "SELECT * FROM test_vtab WHERE b > %d", val);
                break;
            case 2:
                query_len = snprintf(query, sizeof(query), "SELECT DISTINCT * FROM test_vtab ORDER BY a");
                break;
            case 3:
                query_len = snprintf(query, sizeof(query), "SELECT * FROM test_vtab WHERE a IN (%d, %d, %d)", val, val+1, val+2);
                break;
            case 4:
                query_len = snprintf(query, sizeof(query), "SELECT * FROM test_vtab WHERE b = %d COLLATE BINARY", val);
                break;
        }
    } else {
        query_len = snprintf(query, sizeof(query), "SELECT * FROM test_vtab");
    }

    if (query_len > 0 && query_len < (int)sizeof(query)) {
        sqlite3_stmt *stmt;
        rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            /* Execute the statement to trigger xBestIndex */
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Process rows if any */
            }
            sqlite3_finalize(stmt);
        }
    }

    close_db(db);
    return 0;
}