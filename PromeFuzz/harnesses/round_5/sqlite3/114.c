// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
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

/* Dummy module implementation for fuzzing */
static int dummy_xCreate(sqlite3 *db, void *pAux,
                         int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    /* Allocate minimal vtab structure */
    *ppVTab = sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVTab == NULL) return SQLITE_NOMEM;
    memset(*ppVTab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux,
                          int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    /* Allocate minimal vtab structure */
    *ppVTab = sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVTab == NULL) return SQLITE_NOMEM;
    memset(*ppVTab, 0, sizeof(sqlite3_vtab));
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    if (pVTab) sqlite3_free(pVTab);
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    if (pVTab) sqlite3_free(pVTab);
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    /* Allocate minimal cursor structure */
    *ppCursor = sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor == NULL) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    if (pCursor) sqlite3_free(pCursor);
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

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor,
                         sqlite3_context *ctx, int i) {
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int n, sqlite3_value **v,
                         sqlite3_int64 *rowid) {
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg,
                               const char *zName,
                               void (**pxFunc)(sqlite3_context*,int,sqlite3_value**),
                               void **ppArg) {
    return 0;
}

static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    1,                          /* iVersion */
    dummy_xCreate,              /* xCreate */
    dummy_xConnect,             /* xConnect */
    dummy_xBestIndex,           /* xBestIndex */
    dummy_xDisconnect,          /* xDisconnect */
    dummy_xDestroy,             /* xDestroy */
    dummy_xOpen,                /* xOpen */
    dummy_xClose,               /* xClose */
    dummy_xFilter,              /* xFilter */
    dummy_xNext,                /* xNext */
    dummy_xEof,                 /* xEof */
    dummy_xColumn,              /* xColumn */
    dummy_xRowid,               /* xRowid */
    dummy_xUpdate,              /* xUpdate */
    NULL,                       /* xBegin */
    NULL,                       /* xSync */
    NULL,                       /* xCommit */
    NULL,                       /* xRollback */
    dummy_xFindFunction,        /* xFindFunction */
    dummy_xRename,              /* xRename */
    NULL,                       /* xSavepoint */
    NULL,                       /* xRelease */
    NULL,                       /* xRollbackTo */
    NULL,                       /* xShadowName */
    NULL                        /* xIntegrity */
};

/* Module destructor for create_module_v2 */
static void dummy_destructor(void *p) {
    /* Nothing to do */
}

/* Helper to create keep list for sqlite3_drop_modules */
static const char** create_keep_list(const uint8_t *data, size_t size, int *count) {
    if (size == 0) return NULL;
    
    /* Count null-terminated strings in data */
    int str_count = 0;
    size_t pos = 0;
    while (pos < size && data[pos] != 0 && str_count < 10) {
        while (pos < size && data[pos] != 0) pos++;
        if (pos < size) {
            str_count++;
            pos++; /* Skip null terminator */
        }
    }
    
    if (str_count == 0) return NULL;
    
    const char **keep_list = malloc((str_count + 1) * sizeof(char*));
    if (!keep_list) return NULL;
    
    pos = 0;
    for (int i = 0; i < str_count && pos < size; i++) {
        size_t start = pos;
        while (pos < size && data[pos] != 0) pos++;
        if (pos < size) {
            char *str = malloc(pos - start + 1);
            if (str) {
                memcpy(str, data + start, pos - start);
                str[pos - start] = '\0';
                keep_list[i] = str;
            } else {
                keep_list[i] = NULL;
            }
            pos++; /* Skip null terminator */
        }
    }
    keep_list[str_count] = NULL;
    *count = str_count;
    return keep_list;
}

/* Helper to free keep list */
static void free_keep_list(const char **keep_list, int count) {
    if (!keep_list) return;
    for (int i = 0; i < count; i++) {
        if (keep_list[i]) free((void*)keep_list[i]);
    }
    free(keep_list);
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* Open in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) {
        return 0;
    }
    
    /* Use first byte to select which API to test */
    int api_selector = 0;
    if (Size > 0) {
        api_selector = Data[0] % 6;
        Data++;
        Size--;
    }
    
    switch (api_selector) {
        case 0: { /* sqlite3_drop_modules */
            int keep_count = 0;
            const char **keep_list = create_keep_list(Data, Size, &keep_count);
            
            /* First create some modules to potentially drop */
            sqlite3_create_module(db, "test_module1", &dummy_module, NULL);
            sqlite3_create_module(db, "test_module2", &dummy_module, NULL);
            sqlite3_create_module_v2(db, "test_module3", &dummy_module, 
                                     NULL, dummy_destructor);
            
            rc = sqlite3_drop_modules(db, keep_list);
            (void)rc; /* Ignore result for fuzzing */
            
            free_keep_list(keep_list, keep_count);
            break;
        }
        
        case 1: { /* sqlite3_get_table */
            char **pazResult = NULL;
            int nRow = 0, nColumn = 0;
            char *pzErrmsg = NULL;
            
            /* Create a simple table first */
            rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t1(a, b);", 
                             NULL, NULL, NULL);
            rc = sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test');", 
                             NULL, NULL, NULL);
            
            /* Use input as SQL query (ensure null termination) */
            char *sql = NULL;
            if (Size > 0) {
                sql = malloc(Size + 1);
                if (sql) {
                    memcpy(sql, Data, Size);
                    sql[Size] = '\0';
                    
                    rc = sqlite3_get_table(db, sql, &pazResult, 
                                          &nRow, &nColumn, &pzErrmsg);
                    (void)rc;
                    
                    if (pazResult) sqlite3_free_table(pazResult);
                    if (pzErrmsg) sqlite3_free(pzErrmsg);
                    free(sql);
                }
            } else {
                /* Test with simple query if no input */
                rc = sqlite3_get_table(db, "SELECT * FROM t1;", 
                                      &pazResult, &nRow, &nColumn, &pzErrmsg);
                (void)rc;
                if (pazResult) sqlite3_free_table(pazResult);
                if (pzErrmsg) sqlite3_free(pzErrmsg);
            }
            break;
        }
        
        case 2: { /* sqlite3_create_module_v2 */
            /* Use input for module name (ensure null termination) */
            char *module_name = NULL;
            if (Size > 0) {
                module_name = malloc(Size + 1);
                if (module_name) {
                    memcpy(module_name, Data, Size);
                    module_name[Size] = '\0';
                    
                    rc = sqlite3_create_module_v2(db, module_name, 
                                                 &dummy_module, 
                                                 NULL, dummy_destructor);
                    (void)rc;
                    free(module_name);
                }
            } else {
                rc = sqlite3_create_module_v2(db, "fuzz_module", 
                                             &dummy_module, 
                                             NULL, dummy_destructor);
                (void)rc;
            }
            break;
        }
        
        case 3: { /* sqlite3_overload_function */
            /* Use input for function name (ensure null termination) */
            char *func_name = NULL;
            if (Size > 0) {
                func_name = malloc(Size + 1);
                if (func_name) {
                    memcpy(func_name, Data, Size);
                    func_name[Size] = '\0';
                    
                    /* Use last byte for nArg (or 0 if no data) */
                    int nArg = (Size > 0) ? (Data[Size-1] % 10) : 0;
                    
                    rc = sqlite3_overload_function(db, func_name, nArg);
                    (void)rc;
                    free(func_name);
                }
            } else {
                rc = sqlite3_overload_function(db, "fuzz_func", 1);
                (void)rc;
            }
            break;
        }
        
        case 4: { /* sqlite3_create_module */
            /* Use input for module name (ensure null termination) */
            char *module_name = NULL;
            if (Size > 0) {
                module_name = malloc(Size + 1);
                if (module_name) {
                    memcpy(module_name, Data, Size);
                    module_name[Size] = '\0';
                    
                    rc = sqlite3_create_module(db, module_name, 
                                              &dummy_module, NULL);
                    (void)rc;
                    free(module_name);
                }
            } else {
                rc = sqlite3_create_module(db, "fuzz_module2", 
                                          &dummy_module, NULL);
                (void)rc;
            }
            break;
        }
        
        case 5: { /* sqlite3_declare_vtab */
            /* Use input for SQL schema (ensure null termination) */
            char *sql = NULL;
            if (Size > 0) {
                sql = malloc(Size + 1);
                if (sql) {
                    memcpy(sql, Data, Size);
                    sql[Size] = '\0';
                    
                    /* Create a module first */
                    rc = sqlite3_create_module(db, "vtab_for_declare", 
                                             &dummy_module, NULL);
                    if (rc == SQLITE_OK) {
                        /* Note: We're not creating a virtual table here.
                           sqlite3_declare_vtab should be called from within
                           xCreate or xConnect methods, not directly. */
                        (void)rc;
                    }
                    
                    free(sql);
                }
            } else {
                /* Test with simple schema */
                rc = sqlite3_create_module(db, "vtab_for_declare2", 
                                         &dummy_module, NULL);
                (void)rc;
            }
            break;
        }
    }
    
    /* Clean up any remaining modules */
    sqlite3_drop_modules(db, NULL);
    
    /* Clean up */
    sqlite3_close(db);
    
    return 0;
}