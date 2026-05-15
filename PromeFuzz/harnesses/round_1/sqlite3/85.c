// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:146910:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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
static int dummy_xCreate(sqlite3 *db, void *pAux,
                         int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux,
                          int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv,
                         sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                               void (**pxFunc)(sqlite3_context *, int, sqlite3_value **),
                               void **ppArg) {
    return 0;
}

static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static void dummy_destroy(void *p) {
    /* Do nothing */
}

static sqlite3_module dummy_module = {
    1,                          /* iVersion */
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
    NULL,                       /* xBegin */
    NULL,                       /* xSync */
    NULL,                       /* xCommit */
    NULL,                       /* xRollback */
    dummy_xFindFunction,
    dummy_xRename,
    NULL,                       /* xSavepoint */
    NULL,                       /* xRelease */
    NULL,                       /* xRollbackTo */
    NULL,                       /* xShadowName */
    NULL                        /* xIntegrity */
};

/* Helper function to create a dummy sqlite3_index_info structure */
static sqlite3_index_info* create_dummy_index_info(const uint8_t *data, size_t size) {
    static struct sqlite3_index_constraint dummy_constraints[2];
    static struct sqlite3_index_constraint_usage dummy_usage[2];
    static sqlite3_index_info dummy_info;
    
    /* Initialize constraints */
    for (int i = 0; i < 2; i++) {
        dummy_constraints[i].iColumn = (size > 0) ? (data[0] % 10) : 0;
        dummy_constraints[i].op = (size > 1) ? (data[1] % 20) : SQLITE_INDEX_CONSTRAINT_EQ;
        dummy_constraints[i].usable = (size > 2) ? (data[2] % 2) : 1;
        dummy_constraints[i].iTermOffset = 0;
        
        dummy_usage[i].argvIndex = (size > 3) ? (data[3] % 3) : 0;
        dummy_usage[i].omit = (size > 4) ? (data[4] % 2) : 0;
    }
    
    dummy_info.nConstraint = 2;
    dummy_info.aConstraint = dummy_constraints;
    dummy_info.nOrderBy = 0;
    dummy_info.aOrderBy = NULL;
    dummy_info.aConstraintUsage = dummy_usage;
    dummy_info.idxNum = (size > 5) ? data[5] : 0;
    dummy_info.idxStr = NULL;
    dummy_info.needToFreeIdxStr = 0;
    dummy_info.orderByConsumed = 0;
    dummy_info.estimatedCost = 100.0;
    dummy_info.estimatedRows = 100;
    dummy_info.idxFlags = 0;
    dummy_info.colUsed = 0;
    
    return &dummy_info;
}

int LLVMFuzzerTestOneInput_85(const uint8_t *data, size_t size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }
    
    /* Test sqlite3_create_module_v2 */
    if (size > 0) {
        const char *module_name = (const char*)data;
        size_t name_len = (size < 100) ? size : 100;
        char *name_buf = malloc(name_len + 1);
        if (name_buf) {
            memcpy(name_buf, data, name_len);
            name_buf[name_len] = '\0';
            
            rc = sqlite3_create_module_v2(db, name_buf, &dummy_module, 
                                         (void*)(uintptr_t)data[0], 
                                         size > 1 ? dummy_destroy : NULL);
            free(name_buf);
        }
    }
    
    /* Test sqlite3_vtab_config - we need to be in xCreate/xConnect context */
    if (size > 2) {
        /* Create a temporary virtual table to get into the right context */
        rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE temp.dummy USING dummy", 
                         NULL, NULL, &zErrMsg);
        if (zErrMsg) sqlite3_free(zErrMsg);
        
        /* Try to call vtab_config with various operations */
        for (int i = 0; i < 5 && i * 4 < (int)size; i++) {
            int op = (data[i * 4] % 10) + SQLITE_VTAB_CONSTRAINT_SUPPORT;
            int arg = (i * 4 + 1 < (int)size) ? data[i * 4 + 1] : 0;
            sqlite3_vtab_config(db, op, arg);
        }
    }
    
    /* Test sqlite3_vtab_in - requires xBestIndex context */
    if (size > 10) {
        sqlite3_index_info *pIdxInfo = create_dummy_index_info(data, size);
        
        /* Test with various constraint indices and bHandle values */
        for (int i = 0; i < 3 && i * 2 + 10 < (int)size; i++) {
            int iCons = data[10 + i * 2] % 2;  /* Only 2 constraints in our dummy */
            int bHandle = (int8_t)data[10 + i * 2 + 1];  /* Can be negative */
            sqlite3_vtab_in(pIdxInfo, iCons, bHandle);
        }
    }
    
    /* Test sqlite3_vtab_on_conflict - requires xUpdate context */
    if (size > 20) {
        /* We can't easily get into xUpdate context, but we can still call it */
        /* The behavior is undefined outside xUpdate, but we test it anyway */
        sqlite3_vtab_on_conflict(db);
    }
    
    /* Test sqlite3_vtab_rhs_value - requires xBestIndex context */
    if (size > 30) {
        sqlite3_index_info *pIdxInfo = create_dummy_index_info(data + 30, size - 30);
        sqlite3_value *pVal = NULL;
        
        for (int i = 0; i < 2 && i + 30 < (int)size; i++) {
            int iCons = data[30 + i] % 2;
            rc = sqlite3_vtab_rhs_value(pIdxInfo, iCons, &pVal);
            /* pVal is protected and will be deallocated automatically */
        }
    }
    
    /* Test sqlite3_overload_function */
    if (size > 40) {
        const char *func_name = (const char*)(data + 40);
        size_t name_len = (size - 40 < 50) ? size - 40 : 50;
        char *name_buf = malloc(name_len + 1);
        if (name_buf) {
            memcpy(name_buf, data + 40, name_len);
            name_buf[name_len] = '\0';
            
            int nArg = (size > 90) ? data[90] % 10 : 1;
            rc = sqlite3_overload_function(db, name_buf, nArg);
            free(name_buf);
        }
    }
    
    /* Clean up */
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}