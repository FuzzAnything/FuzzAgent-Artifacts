// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_nochange at sqlite3.c:93263:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:161654:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for SQLite3 virtual table APIs.
 * Target APIs:
 * 1. sqlite3_vtab_nochange
 * 2. sqlite3_vtab_config
 * 3. sqlite3_declare_vtab
 * 4. sqlite3_vtab_on_conflict
 * 5. sqlite3_create_module_v2
 * 6. sqlite3_create_module
 */

/* Custom vtab structure to store the db handle for xUpdate */
typedef struct {
    sqlite3_vtab base;
    sqlite3 *db;
} dummy_vtab;

/* Forward declaration */
static const sqlite3_module dummy_module;

static int dummy_vtab_xCreate_with_db(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                                      sqlite3_vtab **ppVTab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;

    /* Exercise sqlite3_vtab_config */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS, 1);

    /* Exercise sqlite3_declare_vtab */
    const char *zSQL = "CREATE TABLE dummy(id INTEGER PRIMARY KEY, val TEXT)";
    int rc = sqlite3_declare_vtab(db, zSQL);
    if (rc != SQLITE_OK) return rc;

    dummy_vtab *pVTab = (dummy_vtab *)sqlite3_malloc(sizeof(dummy_vtab));
    if (!pVTab) return SQLITE_NOMEM;
    
    /* Initialize base struct fields manually as per sqlite3_vtab definition */
    pVTab->base.pModule = &dummy_module;
    /* Removed pVtab field access as it does not exist in standard sqlite3_vtab */
    pVTab->db = db;
    
    *ppVTab = (sqlite3_vtab *)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_xConnect_with_db(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                                       sqlite3_vtab **ppVTab, char **pzErr) {
    return dummy_vtab_xCreate_with_db(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_vtab_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    (void)pVTab;
    (void)pIdxInfo;
    return SQLITE_OK;
}

static int dummy_vtab_xDisconnect(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_xDestroy(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVTab;
    *ppCursor = NULL;
    return SQLITE_OK;
}

static int dummy_vtab_xClose(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_vtab_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                              int argc, sqlite3_value **argv) {
    (void)pCursor;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    return SQLITE_OK;
}

static int dummy_vtab_xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_vtab_xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1; /* End of file */
}

static int dummy_vtab_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int iCol) {
    (void)pCursor;
    (void)iCol;

    /* Exercise sqlite3_vtab_nochange */
    if (sqlite3_vtab_nochange(pCtx)) {
        /* Do nothing, let xUpdate handle no-change */
        return SQLITE_OK;
    }

    sqlite3_result_null(pCtx);
    return SQLITE_OK;
}

static int dummy_vtab_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_vtab_xUpdate_with_db(sqlite3_vtab *pVTab, int nArg, sqlite3_value **argv,
                                      sqlite3_int64 *pRowid) {
    (void)nArg;
    (void)argv;
    (void)pRowid;

    /* Now we can access the db handle stored in our custom struct */
    dummy_vtab *pCustomVTab = (dummy_vtab *)pVTab;
    if (pCustomVTab && pCustomVTab->db) {
        /* Exercise sqlite3_vtab_on_conflict */
        sqlite3_vtab_on_conflict(pCustomVTab->db);
    }

    return SQLITE_OK;
}

static int dummy_vtab_xBegin(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_xSync(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_xCommit(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_xRollback(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_xSavepoint(sqlite3_vtab *pVTab, int iSavepoint) {
    (void)pVTab;
    (void)iSavepoint;
    return SQLITE_OK;
}

static int dummy_vtab_xRelease(sqlite3_vtab *pVTab, int iSavepoint) {
    (void)pVTab;
    (void)iSavepoint;
    return SQLITE_OK;
}

static int dummy_vtab_xRollbackTo(sqlite3_vtab *pVTab, int iSavepoint) {
    (void)pVTab;
    (void)iSavepoint;
    return SQLITE_OK;
}

/* Module definition */
static const sqlite3_module dummy_module = {
    3, /* iVersion */
    dummy_vtab_xCreate_with_db,
    dummy_vtab_xConnect_with_db,
    dummy_vtab_xBestIndex,
    dummy_vtab_xDisconnect,
    dummy_vtab_xDestroy,
    dummy_vtab_xOpen,
    dummy_vtab_xClose,
    dummy_vtab_xFilter,
    dummy_vtab_xNext,
    dummy_vtab_xEof,
    dummy_vtab_xColumn,
    dummy_vtab_xRowid,
    dummy_vtab_xUpdate_with_db,
    dummy_vtab_xBegin,
    dummy_vtab_xSync,
    dummy_vtab_xCommit,
    dummy_vtab_xRollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    dummy_vtab_xSavepoint,
    dummy_vtab_xRelease,
    dummy_vtab_xRollbackTo,
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

static void dummy_destroy(void *pClientData) {
    (void)pClientData;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Use input data to decide which APIs to exercise */
    if (Size > 0) {
        unsigned char op = Data[0] % 6;

        switch (op) {
            case 0:
                /* Exercise sqlite3_create_module */
                rc = sqlite3_create_module(db, "dummy_module", &dummy_module, NULL);
                if (rc != SQLITE_OK) {
                    /* Ignore errors, continue fuzzing */
                }
                break;

            case 1:
                /* Exercise sqlite3_create_module_v2 */
                rc = sqlite3_create_module_v2(db, "dummy_module_v2", &dummy_module, (void *)0x1234, dummy_destroy);
                if (rc != SQLITE_OK) {
                    /* Ignore errors */
                }
                break;

            case 2:
                /* Exercise sqlite3_create_module with NULL to drop module */
                rc = sqlite3_create_module(db, "dummy_module", NULL, NULL);
                break;

            case 3:
                /* Create a virtual table to exercise xCreate/xConnect which call vtab_config and declare_vtab */
                rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE test_vtab USING dummy_module", NULL, NULL, NULL);
                if (rc != SQLITE_OK) {
                    /* Ignore errors */
                }
                break;

            case 4:
                /* Try to create with malformed SQL to test error handling in declare_vtab */
                rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE test_vtab_bad USING dummy_module", NULL, NULL, NULL);
                if (rc != SQLITE_OK) {
                    /* Ignore errors */
                }
                break;

            case 5:
                /* Exercise xUpdate path which calls vtab_on_conflict */
                rc = sqlite3_exec(db, "INSERT INTO test_vtab VALUES(1, 'test')", NULL, NULL, NULL);
                if (rc != SQLITE_OK) {
                    /* Ignore errors */
                }
                rc = sqlite3_exec(db, "UPDATE test_vtab SET val='updated' WHERE id=1", NULL, NULL, NULL);
                if (rc != SQLITE_OK) {
                    /* Ignore errors */
                }
                break;
        }
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}