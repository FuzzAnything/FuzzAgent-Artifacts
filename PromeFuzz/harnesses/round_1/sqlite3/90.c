// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:79669:27 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:145716:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "sqlite3.h"

static int dummy_xConnect(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static int dummy_xCreate(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    return SQLITE_OK;
}

static int dummy_xBestIndex(sqlite3_vtab* pVtab, sqlite3_index_info* pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor* pCursor, int idxNum, const char* idxStr, int argc, sqlite3_value** argv) {
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab* pVtab) {
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab* pVtab) {
    return SQLITE_OK;
}

static sqlite3_module dummy_module = {
    .xConnect = dummy_xConnect,
    .xCreate = dummy_xCreate,
    .xBestIndex = dummy_xBestIndex,
    .xFilter = dummy_xFilter,
    .xDisconnect = dummy_xDisconnect,
    .xDestroy = dummy_xDestroy,
};

static int create_dummy_vtab(sqlite3* db) {
    int rc = sqlite3_create_module_v2(db, "dummy_vtab", &dummy_module, NULL, NULL);
    if (rc != SQLITE_OK) return rc;
    
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE temp.dummy USING dummy_vtab()", NULL, NULL, NULL);
    return rc;
}

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    sqlite3* db = NULL;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || !db) return 0;
    
    // Test sqlite3_overload_function
    if (Size > 3) {
        size_t func_name_len = Data[0] % 64;
        if (func_name_len > 0 && func_name_len + 1 < Size) {
            char* func_name = (char*)malloc(func_name_len + 1);
            if (func_name) {
                memcpy(func_name, Data + 1, func_name_len);
                func_name[func_name_len] = '\0';
                int nArg = Data[func_name_len + 1] % 10;
                sqlite3_overload_function(db, func_name, nArg);
                free(func_name);
            }
        }
    }
    
    // Test sqlite3_vtab_config - needs virtual table context
    if (create_dummy_vtab(db) == SQLITE_OK) {
        // Try different configuration options
        int op = Size > 0 ? Data[0] % 10 : 0;
        sqlite3_vtab_config(db, op);
        
        // Test with additional arguments based on op
        switch (op) {
            case SQLITE_VTAB_CONSTRAINT_SUPPORT:
            case SQLITE_VTAB_DIRECTONLY:
            case SQLITE_VTAB_INNOCUOUS:
                sqlite3_vtab_config(db, op, 1);
                break;
            case SQLITE_VTAB_USES_ALL_SCHEMAS:
                sqlite3_vtab_config(db, op, 0);
                break;
        }
    }
    
    // Test sqlite3_vtab_in and sqlite3_vtab_rhs_value - needs sqlite3_index_info context
    // Create a minimal sqlite3_index_info structure
    if (Size > sizeof(sqlite3_index_info)) {
        sqlite3_index_info idxInfo;
        memset(&idxInfo, 0, sizeof(idxInfo));
        
        // Copy as much data as we can from fuzz input
        size_t copy_size = sizeof(idxInfo) < Size ? sizeof(idxInfo) : Size;
        memcpy(&idxInfo, Data, copy_size);
        
        // Test sqlite3_vtab_in
        if (Size > 2) {
            int iCons = Data[0] % 10;
            int bHandle = (int8_t)Data[1] % 3 - 1; // -1, 0, or 1
            sqlite3_vtab_in(&idxInfo, iCons, bHandle);
        }
        
        // Test sqlite3_vtab_rhs_value
        if (Size > 1) {
            int iCons = Data[0] % 10;
            sqlite3_value* ppVal = NULL;
            sqlite3_vtab_rhs_value(&idxInfo, iCons, &ppVal);
        }
    }
    
    // Test sqlite3_vtab_in_first and sqlite3_vtab_in_next
    // We need actual sqlite3_value objects. Create them by executing queries.
    sqlite3_exec(db, "CREATE TABLE t1(x INTEGER, y TEXT)", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(1, 'test')", NULL, NULL, NULL);
    sqlite3_exec(db, "INSERT INTO t1 VALUES(2, 'test2')", NULL, NULL, NULL);
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, "SELECT x, y FROM t1", -1, &stmt, NULL) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            sqlite3_value* col1 = sqlite3_column_value(stmt, 0);
            sqlite3_value* col2 = sqlite3_column_value(stmt, 1);
            
            sqlite3_value* ppOut = NULL;
            
            // Test sqlite3_vtab_in_first
            int rc_first = sqlite3_vtab_in_first(col1, &ppOut);
            
            // Test sqlite3_vtab_in_next in a loop
            if (rc_first == SQLITE_OK && ppOut) {
                while (sqlite3_vtab_in_next(col2, &ppOut) == SQLITE_OK && ppOut) {
                    // Continue iterating
                }
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with another query to get more sqlite3_value objects
    if (sqlite3_prepare_v2(db, "SELECT 1, 2, 3", -1, &stmt, NULL) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            for (int i = 0; i < 3; i++) {
                sqlite3_value* val = sqlite3_column_value(stmt, i);
                sqlite3_value* ppOut = NULL;
                sqlite3_vtab_in_first(val, &ppOut);
                sqlite3_vtab_in_next(val, &ppOut);
            }
        }
        sqlite3_finalize(stmt);
    }
    
    // Test with NULL values
    sqlite3_value* null_val = NULL;
    sqlite3_value* ppOut = NULL;
    sqlite3_vtab_in_first(null_val, &ppOut);
    sqlite3_vtab_in_next(null_val, &ppOut);
    
    sqlite3_close(db);
    return 0;
}