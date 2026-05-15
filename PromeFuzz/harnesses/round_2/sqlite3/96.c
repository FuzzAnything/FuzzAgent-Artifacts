// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:78774:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Static variables to avoid global state */
static sqlite3 *g_db = NULL;
static sqlite3_module g_module = {0};
static const char *g_module_name = "test_module";
static const char *g_keep_modules[] = {"test_module", NULL};

/* Dummy virtual table methods */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    *ppVtab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    
    /* Use fuzzer data to call vtab config */
    if (argc > 0 && argv[0]) {
        int op = (int)((uintptr_t)pAux) % 10;
        sqlite3_vtab_config(db, op);
        
        /* Declare a dummy schema */
        const char *zSQL = "CREATE TABLE x(a, b, c)";
        sqlite3_declare_vtab(db, zSQL);
    }
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVtab, char **pzErr) {
    return dummy_xCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int dummy_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVtab) {
    return dummy_xDisconnect(pVtab);
}

static int dummy_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = (sqlite3_vtab_cursor *)sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    return *ppCursor ? SQLITE_OK : SQLITE_NOMEM;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    sqlite3_free(pCursor);
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
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static void module_destructor(void *p) {
    /* Nothing to clean up */
}

/* Initialize the dummy module */
static void init_module(void) {
    g_module.iVersion = 1;
    g_module.xCreate = dummy_xCreate;
    g_module.xConnect = dummy_xConnect;
    g_module.xBestIndex = dummy_xBestIndex;
    g_module.xDisconnect = dummy_xDisconnect;
    g_module.xDestroy = dummy_xDestroy;
    g_module.xOpen = dummy_xOpen;
    g_module.xClose = dummy_xClose;
    g_module.xFilter = dummy_xFilter;
    g_module.xNext = dummy_xNext;
    g_module.xEof = dummy_xEof;
    g_module.xColumn = dummy_xColumn;
    g_module.xRowid = dummy_xRowid;
}

/* Create a dummy file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite("test data", 1, 9, f);
        fclose(f);
    }
}

/* Main fuzzer entry point */
int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    int rc;
    
    /* Initialize database if not already done */
    if (!g_db) {
        rc = sqlite3_open(":memory:", &g_db);
        if (rc != SQLITE_OK) return 0;
        init_module();
        create_dummy_file();
    }
    
    if (!g_db) return 0;
    
    /* Use fuzzer data to determine which API to test */
    if (Size == 0) return 0;
    
    uint8_t api_selector = Data[0] % 6;
    const uint8_t *api_data = Data + 1;
    size_t api_data_size = Size > 1 ? Size - 1 : 0;
    
    switch (api_selector) {
        case 0: { /* sqlite3_vtab_config */
            /* Need to be in vtab context - create a temporary module */
            void *client_data = (void *)(uintptr_t)(api_data_size > 0 ? api_data[0] : 0);
            rc = sqlite3_create_module_v2(g_db, "temp_module", &g_module, 
                                          client_data, NULL);
            if (rc == SQLITE_OK) {
                /* Create a virtual table to enter vtab context */
                char *errmsg = NULL;
                rc = sqlite3_exec(g_db, "CREATE VIRTUAL TABLE temp_vtab USING temp_module", 
                                  NULL, NULL, &errmsg);
                if (errmsg) sqlite3_free(errmsg);
                sqlite3_exec(g_db, "DROP TABLE IF EXISTS temp_vtab", NULL, NULL, NULL);
            }
            break;
        }
        
        case 1: { /* sqlite3_declare_vtab */
            /* Similar to case 0 - need vtab context */
            void *client_data = (void *)(uintptr_t)(api_data_size > 0 ? api_data[0] : 0);
            rc = sqlite3_create_module_v2(g_db, "temp_module2", &g_module, 
                                          client_data, NULL);
            if (rc == SQLITE_OK) {
                char *errmsg = NULL;
                rc = sqlite3_exec(g_db, "CREATE VIRTUAL TABLE temp_vtab2 USING temp_module2", 
                                  NULL, NULL, &errmsg);
                if (errmsg) sqlite3_free(errmsg);
                sqlite3_exec(g_db, "DROP TABLE IF EXISTS temp_vtab2", NULL, NULL, NULL);
            }
            break;
        }
        
        case 2: { /* sqlite3_drop_modules */
            /* First ensure our test module is registered */
            rc = sqlite3_create_module(g_db, g_module_name, &g_module, NULL);
            
            /* Create array of modules to keep based on fuzzer data */
            const char **keep = NULL;
            if (api_data_size > 0 && (api_data[0] & 1)) {
                keep = g_keep_modules;
            }
            
            rc = sqlite3_drop_modules(g_db, keep);
            break;
        }
        
        case 3: { /* sqlite3_create_module_v2 */
            /* Use fuzzer data for module name */
            char module_name[32];
            size_t name_len = api_data_size < 31 ? api_data_size : 31;
            if (name_len > 0) {
                memcpy(module_name, api_data, name_len);
                module_name[name_len] = '\0';
            } else {
                strcpy(module_name, "fuzz_module");
            }
            
            void *client_data = (void *)(uintptr_t)(api_data_size > 0 ? api_data[0] : 0);
            rc = sqlite3_create_module_v2(g_db, module_name, &g_module, 
                                          client_data, module_destructor);
            break;
        }
        
        case 4: { /* sqlite3_create_module */
            char module_name[32];
            size_t name_len = api_data_size < 31 ? api_data_size : 31;
            if (name_len > 0) {
                memcpy(module_name, api_data, name_len);
                module_name[name_len] = '\0';
            } else {
                strcpy(module_name, "fuzz_module2");
            }
            
            void *client_data = (void *)(uintptr_t)(api_data_size > 0 ? api_data[0] : 0);
            rc = sqlite3_create_module(g_db, module_name, &g_module, client_data);
            break;
        }
        
        case 5: { /* sqlite3_overload_function */
            char func_name[32];
            size_t name_len = api_data_size < 31 ? api_data_size : 31;
            if (name_len > 0) {
                memcpy(func_name, api_data, name_len);
                func_name[name_len] = '\0';
            } else {
                strcpy(func_name, "fuzz_func");
            }
            
            int nArg = api_data_size > 0 ? (api_data[0] % 10) : 1;
            rc = sqlite3_overload_function(g_db, func_name, nArg);
            break;
        }
        
        default:
            break;
    }
    
    /* Reset database state periodically to avoid accumulation */
    static int call_count = 0;
    if (++call_count % 100 == 0) {
        sqlite3_close(g_db);
        g_db = NULL;
    }
    
    return 0;
}