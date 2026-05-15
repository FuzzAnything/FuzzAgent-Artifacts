// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_initialize at sqlite3.c:171079:16 in sqlite3.h
// sqlite3_open at sqlite3.c:174566:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:145701:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:126718:16 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:173104:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:132472:16 in sqlite3.h
// sqlite3_step at sqlite3.c:79161:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:78363:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_close at sqlite3.c:172232:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:146928:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_in at sqlite3.c:156986:16 in sqlite3.h
// sqlite3_vtab_rhs_value at sqlite3.c:157006:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:79335:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:79343:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:17395:18 in sqlite3.h
// sqlite3_free at sqlite3.c:17470:17 in sqlite3.h
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

static int xCreate(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    *ppVtab = (sqlite3_vtab*)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    
    // Call sqlite3_vtab_config with various options from fuzz data
    if (argc > 0) {
        // Use the first byte of the first argument as configuration option
        int op = (unsigned char)argv[0][0];
        sqlite3_vtab_config(db, op);
    }
    
    return SQLITE_OK;
}

static int xConnect(sqlite3* db, void* pAux, int argc, const char* const* argv, sqlite3_vtab** ppVtab, char** pzErr) {
    return xCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int xBestIndex(sqlite3_vtab* pVtab, sqlite3_index_info* pIdxInfo) {
    // Use fuzz data to test sqlite3_vtab_in and sqlite3_vtab_rhs_value
    static int call_count = 0;
    
    // Test sqlite3_vtab_in with various parameters
    if (pIdxInfo->nConstraint > 0) {
        for (int i = 0; i < pIdxInfo->nConstraint && i < 5; i++) {
            // Test with bHandle = -1 (check feasibility)
            int can_batch = sqlite3_vtab_in(pIdxInfo, i, -1);
            
            // Test with bHandle = 0 or 1 (set preference)
            if (can_batch) {
                int bHandle = (call_count % 2);
                sqlite3_vtab_in(pIdxInfo, i, bHandle);
            }
        }
    }
    
    // Test sqlite3_vtab_rhs_value
    if (pIdxInfo->nConstraint > 0) {
        for (int i = 0; i < pIdxInfo->nConstraint && i < 3; i++) {
            sqlite3_value* ppVal = NULL;
            int rc = sqlite3_vtab_rhs_value(pIdxInfo, i, &ppVal);
            // ppVal may be NULL or valid, both are acceptable
        }
    }
    
    call_count++;
    return SQLITE_OK;
}

static int xFilter(sqlite3_vtab_cursor* pCursor, int idxNum, const char* idxStr, int argc, sqlite3_value** argv) {
    // Test sqlite3_vtab_in_first and sqlite3_vtab_in_next
    if (argc > 0) {
        for (int i = 0; i < argc && i < 3; i++) {
            sqlite3_value* pVal = argv[i];
            sqlite3_value* ppOut = NULL;
            
            int rc = sqlite3_vtab_in_first(pVal, &ppOut);
            while (rc == SQLITE_OK && ppOut != NULL) {
                // Process value
                rc = sqlite3_vtab_in_next(pVal, &ppOut);
            }
        }
    }
    
    return SQLITE_OK;
}

static int xOpen(sqlite3_vtab* pVtab, sqlite3_vtab_cursor** ppCursor) {
    *ppCursor = (sqlite3_vtab_cursor*)sqlite3_malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int xClose(sqlite3_vtab_cursor* pCursor) {
    sqlite3_free(pCursor);
    return SQLITE_OK;
}

static int xNext(sqlite3_vtab_cursor* pCursor) {
    return SQLITE_OK;
}

static int xEof(sqlite3_vtab_cursor* pCursor) {
    return 1;
}

static int xColumn(sqlite3_vtab_cursor* pCursor, sqlite3_context* ctx, int i) {
    return SQLITE_OK;
}

static int xRowid(sqlite3_vtab_cursor* pCursor, sqlite3_int64* pRowid) {
    return SQLITE_OK;
}

static int xDestroy(sqlite3_vtab* pVtab) {
    sqlite3_free(pVtab);
    return SQLITE_OK;
}

static int xDisconnect(sqlite3_vtab* pVtab) {
    return xDestroy(pVtab);
}

static sqlite3_module fuzz_module = {
    0,              /* iVersion */
    xCreate,        /* xCreate */
    xConnect,       /* xConnect */
    xBestIndex,     /* xBestIndex */
    xDisconnect,    /* xDisconnect */
    xDestroy,       /* xDestroy */
    xOpen,          /* xOpen */
    xClose,         /* xClose */
    xFilter,        /* xFilter */
    xNext,          /* xNext */
    xEof,           /* xEof */
    xColumn,        /* xColumn */
    xRowid,         /* xRowid */
    NULL,           /* xUpdate */
    NULL,           /* xBegin */
    NULL,           /* xSync */
    NULL,           /* xCommit */
    NULL,           /* xRollback */
    NULL,           /* xFindFunction */
    NULL,           /* xRename */
    NULL,           /* xSavepoint */
    NULL,           /* xRelease */
    NULL,           /* xRollbackTo */
    NULL            /* xShadowName */
};

int LLVMFuzzerTestOneInput_96(const uint8_t *Data, size_t Size) {
    static int initialized = 0;
    
    // Initialize sqlite3 once
    if (!initialized) {
        sqlite3_initialize();
        initialized = 1;
    }
    
    // Open an in-memory database
    sqlite3* db = NULL;
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 0;
    }
    
    // Create a virtual table for testing
    int rc = sqlite3_create_module(db, "fuzz_vtab", &fuzz_module, NULL);
    if (rc == SQLITE_OK) {
        // Create the virtual table
        char* errmsg = NULL;
        rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE fuzz_test USING fuzz_vtab;", NULL, NULL, &errmsg);
        if (errmsg) {
            sqlite3_free(errmsg);
        }
        
        // Test sqlite3_overload_function with various function names
        if (Size > 0) {
            // Use first few bytes as function name (max 10 chars)
            char func_name[11] = {0};
            size_t name_len = Size < 10 ? Size : 10;
            for (size_t i = 0; i < name_len; i++) {
                // Ensure printable characters
                func_name[i] = 32 + (Data[i] % 95);
            }
            func_name[name_len] = '\0';
            
            // Use next byte as number of arguments (mod 8)
            int nArg = 0;
            if (Size > name_len) {
                nArg = Data[name_len] % 8;
            }
            
            sqlite3_overload_function(db, func_name, nArg);
        }
        
        // Execute a query that will trigger xBestIndex and xFilter
        sqlite3_stmt* stmt = NULL;
        rc = sqlite3_prepare_v2(db, "SELECT * FROM fuzz_test WHERE col IN (1,2,3);", -1, &stmt, NULL);
        if (rc == SQLITE_OK) {
            // Trigger xFilter
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                // Just step through results
            }
            sqlite3_finalize(stmt);
        }
        
        // Test sqlite3_vtab_config directly (even though context may be wrong)
        if (Size > 0) {
            int op = Data[0] % 10;  // Use first byte as op
            sqlite3_vtab_config(db, op);
        }
    }
    
    // Clean up
    sqlite3_close(db);
    
    return 0;
}