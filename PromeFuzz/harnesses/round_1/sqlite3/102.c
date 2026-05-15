// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

#ifdef __cplusplus
extern "C" {
#endif

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

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *info) {
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

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor,
                         sqlite3_context *ctx, int i) {
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int n,
                         sqlite3_value **argv, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_xFindFunction(sqlite3_vtab *pVtab, int nArg,
                               const char *zName,
                               void (**pxFunc)(sqlite3_context *, int,
                                               sqlite3_value **),
                               void **ppArg) {
    return 0;
}

static int dummy_xRename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static void dummy_destructor(void *p) {
    /* Do nothing */
}

static sqlite3_module dummy_module = {
    .iVersion = 1,
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
    .xRename = dummy_xRename,
};

int LLVMFuzzerTestOneInput_102(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3 *db = NULL;
    int rc;
    
    /* Open an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }
    
    /* Use first byte to determine which API to test */
    uint8_t selector = Data[0] % 6;
    size_t offset = 1;
    
    switch (selector) {
        case 0: { /* sqlite3_create_module */
            const char *zName = "test_module";
            if (offset < Size) {
                /* Use up to 32 bytes for module name */
                size_t name_len = (Size - offset) > 32 ? 32 : (Size - offset);
                char *name_buf = malloc(name_len + 1);
                if (name_buf) {
                    memcpy(name_buf, Data + offset, name_len);
                    name_buf[name_len] = '\0';
                    zName = name_buf;
                }
            }
            rc = sqlite3_create_module(db, zName, &dummy_module, NULL);
            if (offset < Size && zName != "test_module") {
                free((void*)zName);
            }
            break;
        }
        case 1: { /* sqlite3_create_module_v2 */
            const char *zName = "test_module_v2";
            if (offset < Size) {
                size_t name_len = (Size - offset) > 32 ? 32 : (Size - offset);
                char *name_buf = malloc(name_len + 1);
                if (name_buf) {
                    memcpy(name_buf, Data + offset, name_len);
                    name_buf[name_len] = '\0';
                    zName = name_buf;
                }
            }
            rc = sqlite3_create_module_v2(db, zName, &dummy_module, NULL,
                                          dummy_destructor);
            if (offset < Size && zName != "test_module_v2") {
                free((void*)zName);
            }
            break;
        }
        case 2: { /* sqlite3_get_table */
            char **pazResult = NULL;
            int nRow = 0;
            int nColumn = 0;
            char *pzErrmsg = NULL;
            const char *zSql = "SELECT 1";
            
            if (offset < Size) {
                /* Use remaining data as SQL statement */
                size_t sql_len = Size - offset;
                char *sql_buf = malloc(sql_len + 1);
                if (sql_buf) {
                    memcpy(sql_buf, Data + offset, sql_len);
                    sql_buf[sql_len] = '\0';
                    zSql = sql_buf;
                }
            }
            
            rc = sqlite3_get_table(db, zSql, &pazResult, &nRow,
                                   &nColumn, &pzErrmsg);
            
            if (pazResult) {
                sqlite3_free_table(pazResult);
            }
            if (pzErrmsg) {
                sqlite3_free(pzErrmsg);
            }
            if (offset < Size && zSql != "SELECT 1") {
                free((void*)zSql);
            }
            break;
        }
        case 3: { /* sqlite3_drop_modules */
            const char **azKeep = NULL;
            if (offset + 1 < Size) {
                /* Create a simple keep list if we have enough data */
                const char *keep_str = "test_module";
                azKeep = &keep_str;
                /* NULL-terminate the array */
                azKeep[1] = NULL;
            }
            rc = sqlite3_drop_modules(db, azKeep);
            break;
        }
        case 4: { /* sqlite3_overload_function */
            const char *zFuncName = "test_func";
            int nArg = 1;
            
            if (offset < Size) {
                /* Use up to 32 bytes for function name */
                size_t name_len = (Size - offset) > 32 ? 32 : (Size - offset);
                char *name_buf = malloc(name_len + 1);
                if (name_buf) {
                    memcpy(name_buf, Data + offset, name_len);
                    name_buf[name_len] = '\0';
                    zFuncName = name_buf;
                    offset += name_len;
                }
                
                if (offset < Size) {
                    nArg = Data[offset] % 10; /* Limit to 10 args */
                }
            }
            
            rc = sqlite3_overload_function(db, zFuncName, nArg);
            
            if (offset < Size && zFuncName != "test_func") {
                free((void*)zFuncName);
            }
            break;
        }
        case 5: { /* sqlite3_declare_vtab */
            const char *zSQL = "CREATE TABLE test(x)";
            
            if (offset < Size) {
                /* Use remaining data as SQL statement */
                size_t sql_len = Size - offset;
                char *sql_buf = malloc(sql_len + 1);
                if (sql_buf) {
                    memcpy(sql_buf, Data + offset, sql_len);
                    sql_buf[sql_len] = '\0';
                    zSQL = sql_buf;
                }
            }
            
            /* Need to be in a virtual table creation context */
            /* We'll create a temporary module and use it */
            rc = sqlite3_create_module(db, "temp_module", &dummy_module, NULL);
            if (rc == SQLITE_OK) {
                /* Create a virtual table to trigger declare_vtab */
                rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE temp_vtab USING temp_module", 
                                 NULL, NULL, NULL);
                if (rc == SQLITE_OK) {
                    /* In real usage, declare_vtab would be called from xCreate/xConnect */
                    /* We'll just call it directly for fuzzing */
                    rc = sqlite3_declare_vtab(db, zSQL);
                    sqlite3_exec(db, "DROP TABLE temp_vtab", NULL, NULL, NULL);
                }
                sqlite3_drop_modules(db, NULL);
            }
            
            if (offset < Size && zSQL != "CREATE TABLE test(x)") {
                free((void*)zSQL);
            }
            break;
        }
    }
    
    sqlite3_close(db);
    return 0;
}

#ifdef __cplusplus
}
#endif