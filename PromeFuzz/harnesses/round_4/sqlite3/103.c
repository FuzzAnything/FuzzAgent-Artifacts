// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:146910:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vtab_on_conflict at sqlite3.c:146910:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int dummy_xUpdate(sqlite3_vtab *pVtab, int nData, sqlite3_value **azData, sqlite_int64 *pRowid) {
    // This is a dummy xUpdate method to allow calling sqlite3_vtab_on_conflict
    // We need to be in an xUpdate context
    sqlite3_vtab_on_conflict((sqlite3*)pVtab);
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    // This is a dummy xBestIndex method to allow calling sqlite3_vtab_in and sqlite3_vtab_rhs_value
    int i;
    sqlite3_value *pVal = NULL;
    
    // Test sqlite3_vtab_in with various bHandle values
    for (i = 0; i < pIdxInfo->nConstraint && i < 3; i++) {
        sqlite3_vtab_in(pIdxInfo, i, -1);  // Check feasibility
        sqlite3_vtab_in(pIdxInfo, i, 0);   // Set preference (no batch)
        sqlite3_vtab_in(pIdxInfo, i, 1);   // Set preference (batch)
    }
    
    // Test sqlite3_vtab_rhs_value
    for (i = 0; i < pIdxInfo->nConstraint && i < 3; i++) {
        sqlite3_vtab_rhs_value(pIdxInfo, i, &pVal);
    }
    
    return SQLITE_OK;
}

static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const*argv, sqlite3_vtab **ppVtab, char **pzErr) {
    // This is a dummy xCreate method to allow calling sqlite3_vtab_config
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);
    sqlite3_vtab_config(db, SQLITE_VTAB_INNOCUOUS, 1);
    
    *ppVtab = (sqlite3_vtab*)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static void dummy_xDestroy(void *p) {
    // Dummy destructor for sqlite3_create_module_v2
}

int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    static const sqlite3_module dummy_module = {
        0,                          /* iVersion */
        dummy_xCreate,              /* xCreate */
        dummy_xCreate,              /* xConnect */
        dummy_xBestIndex,           /* xBestIndex */
        NULL,                       /* xDisconnect */
        NULL,                       /* xDestroy */
        NULL,                       /* xOpen */
        NULL,                       /* xClose */
        NULL,                       /* xFilter */
        NULL,                       /* xNext */
        NULL,                       /* xEof */
        NULL,                       /* xColumn */
        NULL,                       /* xRowid */
        dummy_xUpdate,              /* xUpdate */
        NULL,                       /* xBegin */
        NULL,                       /* xSync */
        NULL,                       /* xCommit */
        NULL,                       /* xRollback */
        NULL,                       /* xFindFunction */
        NULL,                       /* xRename */
        NULL,                       /* xSavepoint */
        NULL,                       /* xRelease */
        NULL,                       /* xRollbackTo */
        NULL,                       /* xShadowName */
    };
    
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    // Test sqlite3_overload_function with various function names
    if (Size > 0) {
        // Use first byte to determine function name length (max 100)
        size_t func_len = Data[0] % 100;
        if (func_len > Size - 1) func_len = Size - 1;
        
        if (func_len > 0) {
            char *func_name = (char*)malloc(func_len + 1);
            if (func_name) {
                memcpy(func_name, Data + 1, func_len);
                func_name[func_len] = '\0';
                
                // Test with 0-3 arguments
                for (int nArg = 0; nArg <= 3; nArg++) {
                    sqlite3_overload_function(db, func_name, nArg);
                }
                
                free(func_name);
            }
        }
    }
    
    // Test sqlite3_create_module_v2
    if (Size > 10) {
        // Use bytes 1-10 for module name (if available)
        size_t name_len = (Size > 20) ? 10 : Size - 10;
        char *module_name = (char*)malloc(name_len + 1);
        if (module_name) {
            memcpy(module_name, Data + (Size > 20 ? 10 : 0), name_len);
            module_name[name_len] = '\0';
            
            sqlite3_create_module_v2(db, module_name, &dummy_module, NULL, dummy_xDestroy);
            free(module_name);
        }
    }
    
    // Test sqlite3_vtab_on_conflict
    // Note: This should only be called from within xUpdate, but we test it anyway
    // The function will return whatever value is in db->vtabOnConflict
    if (db) {
        sqlite3_vtab_on_conflict(db);
    }
    
    // Clean up
    if (db) {
        sqlite3_close(db);
    }
    
    return 0;
}