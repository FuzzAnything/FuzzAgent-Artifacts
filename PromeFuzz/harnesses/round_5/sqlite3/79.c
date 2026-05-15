// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:146910:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Dummy virtual table module implementation */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                        sqlite3_vtab **ppVTab, char **pzErr) {
    /* Call sqlite3_vtab_config with random options */
    if (argc > 0) {
        /* Use fuzz data to determine which options to set */
        sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
        sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);
        sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS, 1);
    }
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    return dummy_xCreate(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    /* Test sqlite3_vtab_rhs_value */
    sqlite3_value *pVal = NULL;
    if (pIdxInfo->nConstraint > 0) {
        for (int i = 0; i < pIdxInfo->nConstraint; i++) {
            int rc = sqlite3_vtab_rhs_value(pIdxInfo, i, &pVal);
            if (rc == SQLITE_OK && pVal) {
                /* Do nothing, just test the call */
            }
        }
    }
    
    /* Test sqlite3_vtab_in */
    if (pIdxInfo->nConstraint > 0) {
        for (int i = 0; i < pIdxInfo->nConstraint; i++) {
            int result = sqlite3_vtab_in(pIdxInfo, i, -1); /* Check feasibility */
            if (result) {
                sqlite3_vtab_in(pIdxInfo, i, 1); /* Set preference */
            }
        }
    }
    
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int nArg, sqlite3_value **argv,
                        sqlite3_int64 *pRowid) {
    /* Test sqlite3_vtab_on_conflict */
    int conflict = sqlite3_vtab_on_conflict((sqlite3*)pVTab->pModule);
    (void)conflict; /* Use variable to avoid unused warning */
    return SQLITE_OK;
}

/* Minimal virtual table module */
static sqlite3_module dummy_module = {
    3, /* iVersion */
    dummy_xCreate,
    dummy_xConnect,
    dummy_xBestIndex,
    NULL, /* xDisconnect */
    NULL, /* xDestroy */
    NULL, /* xOpen */
    NULL, /* xClose */
    NULL, /* xFilter */
    NULL, /* xNext */
    NULL, /* xEof */
    NULL, /* xColumn */
    NULL, /* xRowid */
    dummy_xUpdate,
    NULL, /* xBegin */
    NULL, /* xSync */
    NULL, /* xCommit */
    NULL, /* xRollback */
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

static void dummy_destroy(void *p) {
    /* Do nothing */
    (void)p;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Test sqlite3_create_module_v2 */
    rc = sqlite3_create_module_v2(db, "dummy_module", &dummy_module, 
                                 (void*)Data, dummy_destroy);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }
    
    /* Test sqlite3_overload_function with various function names */
    const char *func_names[] = {"dummy_func", "test_func", "my_func"};
    for (size_t i = 0; i < sizeof(func_names)/sizeof(func_names[0]); i++) {
        sqlite3_overload_function(db, func_names[i], (int)(Size % 5));
    }
    
    /* Create a virtual table using our module */
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE test USING dummy_module()", 
                     NULL, NULL, &zErrMsg);
    if (rc != SQLITE_OK && zErrMsg) {
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }
    
    /* Try to use the virtual table to trigger xBestIndex and xUpdate */
    if (Size > 0) {
        /* Create a dummy file if needed */
        FILE *fp = fopen("./dummy_file", "wb");
        if (fp) {
            fwrite(Data, 1, Size > 1024 ? 1024 : Size, fp);
            fclose(fp);
        }
        
        /* Execute some SQL to trigger virtual table methods */
        sqlite3_exec(db, "INSERT INTO test VALUES(1)", NULL, NULL, &zErrMsg);
        if (zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }
        
        sqlite3_exec(db, "SELECT * FROM test", NULL, NULL, &zErrMsg);
        if (zErrMsg) {
            sqlite3_free(zErrMsg);
            zErrMsg = NULL;
        }
        
        /* Clean up dummy file */
        remove("./dummy_file");
    }
    
    /* Clean up */
    sqlite3_exec(db, "DROP TABLE IF EXISTS test", NULL, NULL, NULL);
    sqlite3_close(db);
    
    return 0;
}