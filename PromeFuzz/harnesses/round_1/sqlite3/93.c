// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
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

/* Global variables for fuzzing state */
static sqlite3 *g_db = NULL;
static const char *g_module_name = "fuzz_module";
static sqlite3_module g_dummy_module = {0};
static void *g_client_data = NULL;
static char g_dummy_sql[256] = "CREATE TABLE dummy(x)";

/* Dummy destructor for sqlite3_create_module_v2 */
static void dummy_destructor(void *p) {
    /* Do nothing */
}

/* Dummy virtual table methods */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pInfo) {
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

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int iCol) {
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

/* Initialize the dummy module */
static void init_dummy_module(void) {
    g_dummy_module.iVersion = 3;
    g_dummy_module.xCreate = dummy_xCreate;
    g_dummy_module.xConnect = dummy_xConnect;
    g_dummy_module.xBestIndex = dummy_xBestIndex;
    g_dummy_module.xDisconnect = dummy_xDisconnect;
    g_dummy_module.xDestroy = dummy_xDestroy;
    g_dummy_module.xOpen = dummy_xOpen;
    g_dummy_module.xClose = dummy_xClose;
    g_dummy_module.xFilter = dummy_xFilter;
    g_dummy_module.xNext = dummy_xNext;
    g_dummy_module.xEof = dummy_xEof;
    g_dummy_module.xColumn = dummy_xColumn;
    g_dummy_module.xRowid = dummy_xRowid;
    g_dummy_module.xUpdate = dummy_xUpdate;
    g_dummy_module.xFindFunction = dummy_xFindFunction;
    g_dummy_module.xRename = dummy_xRename;
}

/* Helper function to extract strings from fuzzer input */
static const char *get_string(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset >= size) return NULL;
    
    const char *str = (const char *)(data + *offset);
    size_t len = 0;
    
    /* Find null terminator or end of input */
    while (*offset + len < size && str[len] != '\0') {
        len++;
    }
    
    if (*offset + len >= size) {
        return NULL;
    }
    
    *offset += len + 1; /* Skip null terminator */
    return str;
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_93(const uint8_t *data, size_t size) {
    int rc;
    size_t offset = 0;
    
    /* Initialize database if not already done */
    if (g_db == NULL) {
        rc = sqlite3_open(":memory:", &g_db);
        if (rc != SQLITE_OK || g_db == NULL) {
            return 0;
        }
        init_dummy_module();
        g_client_data = (void *)0x12345678;
    }
    
    /* Extract parameters from fuzzer input */
    const char *module_name = get_string(data, size, &offset);
    const char *func_name = get_string(data, size, &offset);
    const char *sql_str = get_string(data, size, &offset);
    
    /* Use defaults if extraction failed */
    if (module_name == NULL) module_name = g_module_name;
    if (func_name == NULL) func_name = "fuzz_func";
    if (sql_str == NULL) sql_str = g_dummy_sql;
    
    /* Extract integer parameters */
    int nArg = 0;
    int op = 0;
    if (offset + 2 * sizeof(int) <= size) {
        nArg = *(int *)(data + offset);
        offset += sizeof(int);
        op = *(int *)(data + offset);
        offset += sizeof(int);
    }
    
    /* Test sqlite3_create_module */
    rc = sqlite3_create_module(g_db, module_name, &g_dummy_module, g_client_data);
    if (rc != SQLITE_OK) {
        /* Try with NULL module to remove existing module */
        sqlite3_create_module(g_db, module_name, NULL, NULL);
    }
    
    /* Test sqlite3_create_module_v2 */
    rc = sqlite3_create_module_v2(g_db, module_name, &g_dummy_module, 
                                  g_client_data, dummy_destructor);
    if (rc != SQLITE_OK) {
        sqlite3_create_module_v2(g_db, module_name, NULL, NULL, NULL);
    }
    
    /* Test sqlite3_vtab_config - must be called within xCreate/xConnect context */
    /* We'll create a temporary context by calling dummy_xCreate */
    sqlite3_vtab *dummy_vtab = NULL;
    char *dummy_err = NULL;
    const char *dummy_argv[] = {"dummy_table"};
    
    dummy_xCreate(g_db, g_client_data, 1, dummy_argv, &dummy_vtab, &dummy_err);
    if (dummy_vtab != NULL) {
        /* Now we're in a valid context */
        rc = sqlite3_vtab_config(g_db, op);
        /* Try with more arguments */
        if (offset + sizeof(int) <= size) {
            int arg1 = *(int *)(data + offset);
            offset += sizeof(int);
            sqlite3_vtab_config(g_db, op, arg1);
        }
        
        dummy_xDestroy(dummy_vtab);
    }
    
    /* Test sqlite3_drop_modules */
    const char *keep_modules[] = {module_name, NULL};
    rc = sqlite3_drop_modules(g_db, keep_modules);
    if (rc != SQLITE_OK) {
        /* Try with NULL to remove all modules */
        sqlite3_drop_modules(g_db, NULL);
    }
    
    /* Test sqlite3_overload_function */
    rc = sqlite3_overload_function(g_db, func_name, nArg);
    if (rc != SQLITE_OK) {
        /* Try with different argument counts */
        sqlite3_overload_function(g_db, func_name, 1);
        sqlite3_overload_function(g_db, func_name, 2);
    }
    
    /* Test sqlite3_declare_vtab - must be called within xCreate/xConnect context */
    dummy_xCreate(g_db, g_client_data, 1, dummy_argv, &dummy_vtab, &dummy_err);
    if (dummy_vtab != NULL) {
        rc = sqlite3_declare_vtab(g_db, sql_str);
        dummy_xDestroy(dummy_vtab);
    }
    
    /* Clean up any remaining modules */
    sqlite3_drop_modules(g_db, NULL);
    
    return 0;
}

/* Optional: Cleanup function to close database */
__attribute__((destructor))
static void cleanup(void) {
    if (g_db != NULL) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
}