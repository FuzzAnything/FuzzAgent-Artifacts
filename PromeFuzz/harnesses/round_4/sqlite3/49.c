// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free_table at sqlite3.c:141896:17 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:145733:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:146404:16 in sqlite3.h
// sqlite3_get_table at sqlite3.c:141827:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static sqlite3_module dummy_module = {
    .iVersion = 0,
    .xCreate = NULL,
    .xConnect = NULL,
    .xBestIndex = NULL,
    .xDisconnect = NULL,
    .xDestroy = NULL,
    .xOpen = NULL,
    .xClose = NULL,
    .xFilter = NULL,
    .xNext = NULL,
    .xEof = NULL,
    .xColumn = NULL,
    .xRowid = NULL,
    .xUpdate = NULL,
    .xBegin = NULL,
    .xSync = NULL,
    .xCommit = NULL,
    .xRollback = NULL,
    .xFindFunction = NULL,
    .xRename = NULL,
    .xSavepoint = NULL,
    .xRelease = NULL,
    .xRollbackTo = NULL,
    .xShadowName = NULL,
    .xIntegrity = NULL
};

static void dummy_destroy(void *p) {
    // Do nothing
}

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Open an in-memory database
    sqlite3 *db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    // Use first byte to decide which function to test
    uint8_t selector = Data[0] % 6;
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;

    switch (selector) {
        case 0: {
            // Test sqlite3_declare_vtab
            // Create a minimal CREATE TABLE statement from fuzzer data
            char *zSQL = NULL;
            if (func_size > 0) {
                zSQL = (char *)malloc(func_size + 50);
                if (zSQL) {
                    // Ensure null termination
                    size_t copy_size = func_size > 100 ? 100 : func_size;
                    memcpy(zSQL, "CREATE TABLE x(", 15);
                    memcpy(zSQL + 15, func_data, copy_size);
                    zSQL[15 + copy_size] = ')';
                    zSQL[16 + copy_size] = '\0';
                    
                    // Create a dummy VtabCtx context
                    // Note: This is a hack to allow sqlite3_declare_vtab to be called
                    // In real usage, this should be called from xCreate/xConnect
                    sqlite3_declare_vtab(db, zSQL);
                    
                    free(zSQL);
                }
            }
            break;
        }
        
        case 1: {
            // Test sqlite3_get_table
            if (func_size > 0) {
                char **pazResult = NULL;
                int nRow = 0;
                int nColumn = 0;
                char *pzErrmsg = NULL;
                
                // Create a null-terminated SQL string from fuzzer data
                char *zSql = (char *)malloc(func_size + 1);
                if (zSql) {
                    size_t copy_size = func_size > 1000 ? 1000 : func_size;
                    memcpy(zSql, func_data, copy_size);
                    zSql[copy_size] = '\0';
                    
                    // Execute the SQL
                    sqlite3_get_table(db, zSql, &pazResult, &nRow, &nColumn, &pzErrmsg);
                    
                    // Clean up
                    if (pazResult) {
                        sqlite3_free_table(pazResult);
                    }
                    if (pzErrmsg) {
                        sqlite3_free(pzErrmsg);
                    }
                    free(zSql);
                }
            }
            break;
        }
        
        case 2: {
            // Test sqlite3_drop_modules
            // Create a NULL-terminated array of strings to keep
            if (func_size > 0) {
                // Simple approach: create array with one string from fuzzer data
                char *keep_str = NULL;
                if (func_size > 0) {
                    keep_str = (char *)malloc(func_size + 1);
                    if (keep_str) {
                        size_t copy_size = func_size > 100 ? 100 : func_size;
                        memcpy(keep_str, func_data, copy_size);
                        keep_str[copy_size] = '\0';
                        
                        const char *azKeep[] = {keep_str, NULL};
                        sqlite3_drop_modules(db, azKeep);
                        
                        free(keep_str);
                    }
                } else {
                    // Test with NULL to remove all modules
                    sqlite3_drop_modules(db, NULL);
                }
            }
            break;
        }
        
        case 3: {
            // Test sqlite3_create_module_v2
            if (func_size > 0) {
                char *zName = (char *)malloc(func_size + 1);
                if (zName) {
                    size_t copy_size = func_size > 50 ? 50 : func_size;
                    memcpy(zName, func_data, copy_size);
                    zName[copy_size] = '\0';
                    
                    sqlite3_create_module_v2(db, zName, &dummy_module, NULL, dummy_destroy);
                    
                    free(zName);
                }
            }
            break;
        }
        
        case 4: {
            // Test sqlite3_overload_function
            if (func_size > 0) {
                char *zFuncName = (char *)malloc(func_size + 1);
                if (zFuncName) {
                    size_t copy_size = func_size > 50 ? 50 : func_size;
                    memcpy(zFuncName, func_data, copy_size);
                    zFuncName[copy_size] = '\0';
                    
                    // Use a random number of arguments (0-10)
                    int nArg = (func_data[0] % 11);
                    sqlite3_overload_function(db, zFuncName, nArg);
                    
                    free(zFuncName);
                }
            }
            break;
        }
        
        case 5: {
            // Test sqlite3_create_module
            if (func_size > 0) {
                char *zName = (char *)malloc(func_size + 1);
                if (zName) {
                    size_t copy_size = func_size > 50 ? 50 : func_size;
                    memcpy(zName, func_data, copy_size);
                    zName[copy_size] = '\0';
                    
                    sqlite3_create_module(db, zName, &dummy_module, NULL);
                    
                    free(zName);
                }
            }
            break;
        }
    }

    // Clean up
    sqlite3_close(db);
    return 0;
}