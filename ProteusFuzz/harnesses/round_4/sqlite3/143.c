// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_vtab_distinct at sqlite3.c:171785:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:171750:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:171730:16 in sqlite3.h
// sqlite3_vtab_collation at sqlite3.c:171710:24 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* Static helper to avoid name conflicts */
static void cleanup_db(sqlite3 *db) {
    if (db) {
        sqlite3_close(db);
    }
}

/*
 * Virtual Table Module Implementation
 */

static int vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    int rc = sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    (void)rc;

    *ppVtab = malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));

    return SQLITE_OK;
}

static int vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                        sqlite3_vtab **ppVtab, char **pzErr) {
    return vtab_xConnect(db, pAux, argc, argv, ppVtab, pzErr);
}

static int vtab_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab;

    int i;
    int nCons = pIdxInfo->nConstraint;

    int distinct_mode = sqlite3_vtab_distinct(pIdxInfo);
    (void)distinct_mode;

    for (i = 0; i < nCons; i++) {
        sqlite3_value *pVal = NULL;
        int rc;
        const char *coll;
        int in_check;
        int in_handle;

        rc = sqlite3_vtab_rhs_value(pIdxInfo, i, &pVal);
        if (rc == SQLITE_OK && pVal) {
            /* Value retrieved */
        }

        in_check = sqlite3_vtab_in(pIdxInfo, i, -1);
        (void)in_check;

        in_handle = sqlite3_vtab_in(pIdxInfo, i, 1);
        (void)in_handle;

        in_handle = sqlite3_vtab_in(pIdxInfo, i, 0);
        (void)in_handle;

        coll = sqlite3_vtab_collation(pIdxInfo, i);
        (void)coll;
    }

    pIdxInfo->idxNum = 1;
    pIdxInfo->estimatedCost = 1.0;
    pIdxInfo->estimatedRows = 1;

    return SQLITE_OK;
}

static int vtab_xDisconnect(sqlite3_vtab *pVtab) {
    if (pVtab) {
        free(pVtab);
    }
    return SQLITE_OK;
}

static int vtab_xDestroy(sqlite3_vtab *pVtab) {
    return vtab_xDisconnect(pVtab);
}

static int vtab_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int vtab_xClose(sqlite3_vtab_cursor *cur) {
    if (cur) {
        free(cur);
    }
    return SQLITE_OK;
}

static int vtab_xFilter(sqlite3_vtab_cursor *cur, int idxNum, const char *idxStr,
                        int argc, sqlite3_value **argv) {
    (void)cur;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    return SQLITE_OK;
}

static int vtab_xNext(sqlite3_vtab_cursor *cur) {
    (void)cur;
    return SQLITE_DONE;
}

static int vtab_xEof(sqlite3_vtab_cursor *cur) {
    (void)cur;
    return 1;
}

static int vtab_xColumn(sqlite3_vtab_cursor *cur, sqlite3_context *ctx, int i) {
    (void)cur;
    (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int vtab_xRowid(sqlite3_vtab_cursor *cur, sqlite_int64 *pRowid) {
    (void)cur;
    *pRowid = 1;
    return SQLITE_OK;
}

static int vtab_xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv,
                        sqlite_int64 *pRowid) {
    (void)pVtab;
    (void)argc;
    (void)argv;
    (void)pRowid;
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

/* Module definition */
static const sqlite3_module vtab_module = {
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
    NULL  /* xRollbackTo */
};

/*
 * Fuzzing Entry Point
 */
int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    sqlite3_stmt *stmt = NULL;
    char *zSql = NULL;

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Register the virtual table module */
    rc = sqlite3_create_module(db, "fuzz_vtab", &vtab_module, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /* Create the virtual table */
    rc = sqlite3_exec(db, "CREATE TABLE t1 USING fuzz_vtab(a, b, c);", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        cleanup_db(db);
        return 0;
    }

    /*
     * 2. Fuzz sqlite3_overload_function
     */
    if (Size > 0) {
        size_t len = (Size < 64) ? Size : 64;
        char func_name[65];
        memcpy(func_name, Data, len);
        func_name[len] = '\0';

        for (size_t i = 0; i < len; i++) {
            if (!((func_name[i] >= 'a' && func_name[i] <= 'z') ||
                  (func_name[i] >= 'A' && func_name[i] <= 'Z') ||
                  (func_name[i] >= '0' && func_name[i] <= '9') ||
                  func_name[i] == '_')) {
                func_name[i] = '_';
            }
        }

        int nArg = (Size > 64) ? (Data[64] % 10) : 0;
        rc = sqlite3_overload_function(db, func_name, nArg);
        (void)rc;
    }

    /*
     * 3. Explore program states by invoking target functions via SQL.
     */
    if (Size > 10) {
        size_t sql_len = (Size < 256) ? Size : 256;
        zSql = malloc(sql_len + 100);
        if (zSql) {
            snprintf(zSql, sql_len + 100, "SELECT * FROM t1 WHERE ");
            size_t offset = strlen(zSql);

            int i = 0;
            while (i + 1 < sql_len && offset < sql_len + 50) {
                char col = 'a' + (Data[i] % 3);
                char op = '=';
                int op_idx = Data[i+1] % 5;
                if (op_idx == 0) op = '=';
                else if (op_idx == 1) op = '<';
                else if (op_idx == 2) op = '>';
                else if (op_idx == 3) op = '<';
                else if (op_idx == 4) op = '>';

                if (op_idx == 3) {
                    zSql[offset++] = '<';
                    zSql[offset++] = '=';
                } else if (op_idx == 4) {
                    zSql[offset++] = '>';
                    zSql[offset++] = '=';
                } else {
                    zSql[offset++] = op;
                }

                int val = Data[i+2] % 100;
                offset += snprintf(zSql + offset, sql_len + 100 - offset, "%d", val);

                if (i + 3 < sql_len) {
                    zSql[offset++] = ' ';
                    zSql[offset++] = 'A';
                    zSql[offset++] = 'N';
                    zSql[offset++] = 'D';
                    zSql[offset++] = ' ';
                }

                i += 3;
            }
            zSql[offset] = '\0';

            rc = sqlite3_prepare_v2(db, zSql, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    /* Iterate rows */
                }
                sqlite3_finalize(stmt);
                stmt = NULL;
            }
        }
        free(zSql);
    }

    /*
     * Try to trigger IN clause handling specifically
     */
    if (Size > 20) {
        const char *in_sql = "SELECT * FROM t1 WHERE a IN (1, 2, 3, 4, 5);";
        rc = sqlite3_prepare_v2(db, in_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Iterate */
            }
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    /*
     * Try to trigger DISTINCT and ORDER BY handling
     */
    if (Size > 30) {
        const char *distinct_sql = "SELECT DISTINCT a FROM t1 ORDER BY b;";
        rc = sqlite3_prepare_v2(db, distinct_sql, -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                /* Iterate */
            }
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    /*
     * 4. Cleanup
     */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    cleanup_db(db);

    return 0;
}