// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_snprintf at sqlite3.c:19388:18 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
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

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum,
                         const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx,
                         int i) {
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv,
                         sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xBegin(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *pVTab) {
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

static int dummy_xSavepoint(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xRelease(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xRollbackTo(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_xShadowName(const char *zName) {
    return 0;
}

static int dummy_xIntegrity(sqlite3_vtab *pVTab, const char *zSchema,
                            const char *zTabName, int mFlags, char **pzErr) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    .iVersion = 4,
    .xCreate = dummy_xCreate,
    .xConnect = dummy_xConnect,
    .xBestIndex = dummy_xBestIndex,
    .xDisconnect = dummy_xDisconnect,
    .xDestroy = dummy_xDestroy,
    .xOpen = dummy_xOpen,
    .xClose = dummy_xClose,
    .xFilter = dummy_xFilter,
    .xNext = dummy_xNext,
    .xEof = dummy_xEof,
    .xColumn = dummy_xColumn,
    .xRowid = dummy_xRowid,
    .xUpdate = dummy_xUpdate,
    .xBegin = dummy_xBegin,
    .xSync = dummy_xSync,
    .xCommit = dummy_xCommit,
    .xRollback = dummy_xRollback,
    .xFindFunction = dummy_xFindFunction,
    .xRename = dummy_xRename,
    .xSavepoint = dummy_xSavepoint,
    .xRelease = dummy_xRelease,
    .xRollbackTo = dummy_xRollbackTo,
    .xShadowName = dummy_xShadowName,
    .xIntegrity = dummy_xIntegrity
};

static void dummy_destructor(void *p) {
    /* Nothing to do */
}

/* Helper function to extract strings from fuzzer input */
static char *extract_string(const uint8_t **data, size_t *size) {
    if (*size == 0) return NULL;
    
    const uint8_t *start = *data;
    const uint8_t *end = start;
    size_t remaining = *size;
    
    while (remaining > 0 && *end != 0) {
        end++;
        remaining--;
    }
    
    size_t len = end - start;
    
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    if (len > 0) {
        memcpy(str, start, len);
    }
    str[len] = 0;
    
    *data += len;
    *size -= len;
    
    if (remaining > 0) {
        (*data)++;
        (*size)--;
    }
    
    return str;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) return 0;
    
    /* Extract parameters from fuzzer input */
    char *module_name = extract_string(&ptr, &remaining);
    char *sql_declaration = extract_string(&ptr, &remaining);
    char *func_name = extract_string(&ptr, &remaining);
    char *keep_module = extract_string(&ptr, &remaining);
    
    /* Get integer parameters */
    int op = 0;
    int nArg = 0;
    
    if (remaining >= sizeof(int)) {
        memcpy(&op, ptr, sizeof(int));
        ptr += sizeof(int);
        remaining -= sizeof(int);
    }
    
    if (remaining >= sizeof(int)) {
        memcpy(&nArg, ptr, sizeof(int));
        ptr += sizeof(int);
        remaining -= sizeof(int);
    }
    
    /* Test sqlite3_create_module */
    if (module_name) {
        sqlite3_create_module(db, module_name, &dummy_module, NULL);
        
        /* Also test with NULL module to drop it */
        if (Size % 2 == 0) {
            sqlite3_create_module(db, module_name, NULL, NULL);
        }
    }
    
    /* Test sqlite3_create_module_v2 */
    if (module_name && Size % 3 == 0) {
        sqlite3_create_module_v2(db, module_name, &dummy_module, 
                                 (void*)0x1234, dummy_destructor);
    }
    
    /* Test sqlite3_drop_modules */
    const char *keep_list[2] = {NULL, NULL};
    if (keep_module) {
        keep_list[0] = keep_module;
        sqlite3_drop_modules(db, keep_list);
    } else {
        sqlite3_drop_modules(db, NULL);
    }
    
    /* Test sqlite3_overload_function */
    if (func_name) {
        sqlite3_overload_function(db, func_name, nArg);
    }
    
    /* Test sqlite3_vtab_config and sqlite3_declare_vtab within a virtual table context */
    if (module_name && sql_declaration) {
        /* Re-register module for testing */
        sqlite3_create_module(db, module_name, &dummy_module, NULL);
        
        /* Create a virtual table to establish context */
        char create_sql[256];
        sqlite3_snprintf(sizeof(create_sql), create_sql,
                        "CREATE VIRTUAL TABLE test_vtab USING %s", module_name);
        sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    }
    
    /* Cleanup - only free pointers that were actually allocated */
    if (module_name) free(module_name);
    if (sql_declaration) free(sql_declaration);
    if (func_name) free(func_name);
    if (keep_module) free(keep_module);
    
    sqlite3_close(db);
    return 0;
}