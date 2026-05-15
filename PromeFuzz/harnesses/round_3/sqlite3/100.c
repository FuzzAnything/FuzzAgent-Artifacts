// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// Dummy module implementation for testing
static int dummy_xCreate(sqlite3 *db, void *pAux,
                         int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    // Allocate and initialize a minimal sqlite3_vtab structure
    *ppVTab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVTab == NULL) {
        return SQLITE_NOMEM;
    }
    memset(*ppVTab, 0, sizeof(sqlite3_vtab));
    
    // Declare a simple virtual table schema
    const char *zSQL = "CREATE TABLE x(a, b, c)";
    sqlite3_declare_vtab(db, zSQL);
    
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux,
                          int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    // Allocate and initialize a minimal sqlite3_vtab structure
    *ppVTab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVTab == NULL) {
        return SQLITE_NOMEM;
    }
    memset(*ppVTab, 0, sizeof(sqlite3_vtab));
    
    // Declare a simple virtual table schema
    const char *zSQL = "CREATE TABLE x(a, b, c)";
    sqlite3_declare_vtab(db, zSQL);
    
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    if (pVTab) {
        sqlite3_free(pVTab);
    }
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    if (pVTab) {
        sqlite3_free(pVTab);
    }
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    // Allocate a minimal cursor
    *ppCursor = (sqlite3_vtab_cursor *)sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor == NULL) {
        return SQLITE_NOMEM;
    }
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    if (pCursor) {
        sqlite3_free(pCursor);
    }
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

static sqlite3_module dummy_module = {
    1,                      // iVersion
    dummy_xCreate,          // xCreate
    dummy_xConnect,         // xConnect
    dummy_xBestIndex,       // xBestIndex
    dummy_xDisconnect,      // xDisconnect
    dummy_xDestroy,         // xDestroy
    dummy_xOpen,            // xOpen
    dummy_xClose,           // xClose
    dummy_xFilter,          // xFilter
    dummy_xNext,            // xNext
    dummy_xEof,             // xEof
    dummy_xColumn,          // xColumn
    dummy_xRowid,           // xRowid
    dummy_xUpdate,          // xUpdate
    NULL,                   // xBegin
    NULL,                   // xSync
    NULL,                   // xCommit
    NULL,                   // xRollback
    dummy_xFindFunction,    // xFindFunction
    dummy_xRename,          // xRename
    NULL,                   // xSavepoint
    NULL,                   // xRelease
    NULL,                   // xRollbackTo
    NULL,                   // xShadowName
    NULL                    // xIntegrity
};

// Helper function to extract strings from fuzzer input
static const char* get_string(const uint8_t **data, size_t *size) {
    if (*size == 0) return NULL;
    
    // Find null terminator or use remaining bytes
    size_t len = 0;
    while (len < *size && (*data)[len] != 0) {
        len++;
    }
    
    // Allocate and copy string
    char *str = malloc(len + 1);
    if (!str) return NULL;
    
    memcpy(str, *data, len);
    str[len] = '\0';
    
    // Update data pointer and size
    *data += (len < *size) ? len + 1 : len;
    *size -= (len < *size) ? len + 1 : len;
    
    return str;
}

int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    // Open in-memory database
    sqlite3 *db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Parse fuzzer input for parameters
    const uint8_t *ptr = Data;
    size_t remaining = Size;
    
    // Get module name
    const char *module_name = get_string(&ptr, &remaining);
    
    // Get SQL for declare_vtab
    const char *declare_sql = get_string(&ptr, &remaining);
    
    // Get function name for overload
    const char *func_name = get_string(&ptr, &remaining);
    
    // Get SQL for get_table
    const char *table_sql = get_string(&ptr, &remaining);
    
    // Get nArg for overload_function (use first byte if available)
    int nArg = 0;
    if (remaining > 0) {
        nArg = ptr[0] % 10;  // Limit to reasonable values
    }
    
    // Test sqlite3_create_module
    if (module_name && strlen(module_name) > 0) {
        // Test with valid module
        sqlite3_create_module(db, module_name, &dummy_module, NULL);
        
        // Test with NULL module (to remove)
        sqlite3_create_module(db, module_name, NULL, NULL);
        
        // Test again with valid module
        sqlite3_create_module(db, module_name, &dummy_module, NULL);
        
        // Test sqlite3_declare_vtab directly
        if (declare_sql && strlen(declare_sql) > 0) {
            // Create a simple table first to have a valid context
            sqlite3_exec(db, "CREATE TABLE dummy(id INTEGER)", NULL, NULL, NULL);
        }
    }
    
    // Test sqlite3_drop_modules
    if (module_name && strlen(module_name) > 0) {
        // Create array of strings to keep
        const char *keep_list[] = {module_name, NULL};
        
        // Test dropping all modules
        sqlite3_drop_modules(db, NULL);
        
        // Re-add module
        sqlite3_create_module(db, module_name, &dummy_module, NULL);
        
        // Test dropping all except specified
        sqlite3_drop_modules(db, keep_list);
    }
    
    // Test sqlite3_overload_function
    if (func_name && strlen(func_name) > 0) {
        sqlite3_overload_function(db, func_name, nArg);
    }
    
    // Test sqlite3_get_table and sqlite3_free_table
    if (table_sql && strlen(table_sql) > 0) {
        char **pazResult = NULL;
        int pnRow = 0;
        int pnColumn = 0;
        char *pzErrmsg = NULL;
        
        // Create a simple table first
        sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test(id INTEGER, name TEXT)", 
                     NULL, NULL, NULL);
        
        // Insert some data
        sqlite3_exec(db, "INSERT INTO test VALUES(1, 'test1')", NULL, NULL, NULL);
        sqlite3_exec(db, "INSERT INTO test VALUES(2, 'test2')", NULL, NULL, NULL);
        
        // Test get_table with fuzzer SQL
        sqlite3_get_table(db, table_sql, &pazResult, &pnRow, &pnColumn, &pzErrmsg);
        
        // Free results
        if (pazResult) {
            sqlite3_free_table(pazResult);
        }
        if (pzErrmsg) {
            sqlite3_free(pzErrmsg);
        }
    }
    
    // Clean up
    if (module_name) free((void*)module_name);
    if (declare_sql) free((void*)declare_sql);
    if (func_name) free((void*)func_name);
    if (table_sql) free((void*)table_sql);
    
    sqlite3_close(db);
    
    return 0;
}