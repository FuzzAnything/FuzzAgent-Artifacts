// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
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
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv, 
                         sqlite3_vtab **ppVtab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVtab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVtab) {
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
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

static int dummy_xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv,
                         sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg, const char *zName,
                               void (**pxFunc)(sqlite3_context*, int, sqlite3_value**),
                               void **ppArg) {
    return 0;
}

static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    .iVersion = 2,
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
    .xFindFunction = dummy_xFindFunction,
    .xRename = dummy_xRename
};

/* Destructor for client data */
static void dummy_destructor(void *p) {
    /* Do nothing */
}

/* Helper to extract strings from fuzzer input */
static const char* get_string(const uint8_t **data, size_t *size) {
    if (*size == 0) return NULL;
    
    size_t len = (*data)[0] % 256;
    if (len >= *size) len = *size - 1;
    
    static char buf[256];
    if (len > 255) len = 255;
    
    memcpy(buf, *data + 1, len);
    buf[len] = '\0';
    
    *data += len + 1;
    *size -= len + 1;
    
    return buf;
}

/* Helper to extract integer from fuzzer input */
static int get_int(const uint8_t **data, size_t *size) {
    if (*size < 1) return 0;
    
    int val = (*data)[0];
    *data += 1;
    *size -= 1;
    
    return val;
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    /* Open an in-memory database */
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    /* Extract parameters from fuzzer input */
    const char *module_name = get_string(&ptr, &remaining);
    const char *zsql = get_string(&ptr, &remaining);
    const char *func_name = get_string(&ptr, &remaining);
    int nArg = get_int(&ptr, &remaining);
    int op = get_int(&ptr, &remaining);
    
    /* Test sqlite3_create_module */
    if (module_name) {
        sqlite3_create_module(db, module_name, &dummy_module, NULL);
        
        /* Also test with NULL module to remove it */
        if (nArg % 2 == 0) {
            sqlite3_create_module(db, module_name, NULL, NULL);
        }
    }
    
    /* Test sqlite3_create_module_v2 */
    if (module_name && func_name) {
        sqlite3_create_module_v2(db, func_name, &dummy_module, NULL, dummy_destructor);
    }
    
    /* Test sqlite3_declare_vtab - needs to be called from within xCreate/xConnect */
    if (zsql) {
        /* We need a virtual table context to call this properly.
           Instead, we'll create a temporary statement and try it. */
        sqlite3_stmt *stmt = NULL;
        if (sqlite3_prepare_v2(db, "CREATE VIRTUAL TABLE temp.dummy USING dummy", -1, &stmt, NULL) == SQLITE_OK) {
            sqlite3_finalize(stmt);
        }
    }
    
    /* Test sqlite3_drop_modules */
    if (module_name) {
        const char *keep_list[] = {module_name, NULL};
        const char **azKeep = (nArg % 3 == 0) ? keep_list : NULL;
        sqlite3_drop_modules(db, azKeep);
    }
    
    /* Test sqlite3_overload_function */
    if (func_name) {
        sqlite3_overload_function(db, func_name, nArg);
    }
    
    /* Test sqlite3_vtab_config - needs to be called from xCreate/xConnect */
    /* We'll attempt to call it with various operations */
    if (op >= 0 && op <= 4) {
        sqlite3_vtab_config(db, op);
    }
    
    /* Cleanup */
    sqlite3_close(db);
    
    return 0;
}