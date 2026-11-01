// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Static helper to create a dummy module for testing.
 * This module implements minimal required callbacks to allow registration.
 */
static int dummy_vtab_create(sqlite3 *db, void *pAux,
                             int argc, const char *const *argv,
                             sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
    /* Minimal implementation: just return SQLITE_OK to allow creation */
    return SQLITE_OK;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux,
                              int argc, const char *const *argv,
                              sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
    return SQLITE_OK;
}

static int dummy_vtab_best_index(sqlite3_vtab *pVTab, sqlite3_index_info *pIdx) {
    (void)pVTab; (void)pIdx;
    return SQLITE_OK;
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVTab; (void)ppCursor;
    return SQLITE_OK;
}

static int dummy_vtab_close(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_vtab_filter(sqlite3_vtab_cursor *pCursor, int idxNum,
                             const char *idxStr, int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}

static int dummy_vtab_next(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_vtab_eof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1; /* Always EOF */
}

static int dummy_vtab_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor; (void)ctx; (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int dummy_vtab_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    if (pRowid) *pRowid = 1;
    return SQLITE_OK;
}

static int dummy_vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVTab; (void)argc; (void)argv; (void)pRowid;
    return SQLITE_OK;
}

static int dummy_vtab_begin(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_sync(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_commit(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_vtab_rollback(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

/* Global dummy module instance */
static const sqlite3_module g_dummy_module = {
    0, /* iVersion */
    dummy_vtab_create,
    dummy_vtab_connect,
    dummy_vtab_best_index,
    dummy_vtab_disconnect,
    dummy_vtab_destroy,
    dummy_vtab_open,
    dummy_vtab_close,
    dummy_vtab_filter,
    dummy_vtab_next,
    dummy_vtab_eof,
    dummy_vtab_column,
    dummy_vtab_rowid,
    dummy_vtab_update,
    dummy_vtab_begin,
    dummy_vtab_sync,
    dummy_vtab_commit,
    dummy_vtab_rollback,
    0, /* xFindFunction */
    0, /* xRename */
    0, /* xSavepoint */
    0, /* xRelease */
    0, /* xRollbackTo */
    0, /* xShadowName */
    0  /* xIntegrity */
};

/*
 * Helper to extract a C-string from fuzz data, ensuring null termination.
 * Returns a pointer to a static buffer or a newly allocated string if needed.
 * For safety in fuzzing, we limit length to avoid huge allocations.
 */
static const char *get_string_from_input(const uint8_t *Data, size_t Size, size_t *outLen) {
    static char buf[4096];
    size_t len = 0;
    
    if (Size == 0) {
        buf[0] = '\0';
        *outLen = 0;
        return buf;
    }

    /* Find first null terminator or limit to buffer size */
    while (len < Size && len < sizeof(buf) - 1 && Data[len] != '\0') {
        len++;
    }
    
    memcpy(buf, Data, len);
    buf[len] = '\0';
    *outLen = len;
    return buf;
}

int LLVMFuzzerTestOneInput_74(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    
    /* 1. Prepare the environment */
    /* Open an in-memory database to avoid file system issues */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* 2. Fuzzing Logic */
    
    /* Strategy: Use the input data to drive different API calls and states */
    
    /* Case A: Test sqlite3_create_module and sqlite3_create_module_v2 */
    if (Size > 0) {
        size_t nameLen;
        const char *moduleName = get_string_from_input(Data, Size, &nameLen);
        
        /* Register a module with a name derived from input */
        /* Ensure name is not empty to avoid SQLITE_MISUSE */
        if (nameLen > 0) {
            sqlite3_create_module(db, moduleName, &g_dummy_module, NULL);
            sqlite3_create_module_v2(db, moduleName, &g_dummy_module, NULL, NULL);
        }
        
        /* Test dropping all modules */
        sqlite3_drop_modules(db, NULL);
        
        /* Re-register to test dropping specific modules */
        if (nameLen > 0) {
            sqlite3_create_module(db, moduleName, &g_dummy_module, NULL);
            sqlite3_create_module_v2(db, moduleName, &g_dummy_module, NULL, NULL);
            
            /* Create a keep list (whitelist) */
            const char *keepList[] = { moduleName, NULL };
            sqlite3_drop_modules(db, keepList);
            
            /* Drop all again */
            sqlite3_drop_modules(db, NULL);
        }
    }

    /* Case B: Test sqlite3_declare_vtab via SQL execution */
    /* We cannot call sqlite3_declare_vtab directly, so we trigger it via CREATE VIRTUAL TABLE */
    if (Size > 0) {
        size_t sqlLen;
        const char *sqlStr = get_string_from_input(Data, Size, &sqlLen);
        
        /* Construct a CREATE VIRTUAL TABLE statement */
        /* We use a safe module name "dummy" which we registered above if possible */
        char query[2048];
        int qLen = snprintf(query, sizeof(query), "CREATE VIRTUAL TABLE IF NOT EXISTS fuzz_vtab USING dummy (%s);", 
                            (sqlLen > 0 && sqlLen < 2000) ? sqlStr : "(c1 TEXT)");
        
        if (qLen > 0 && qLen < (int)sizeof(query)) {
            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                rc = sqlite3_step(stmt);
                /* Ignore errors, just explore state */
                sqlite3_finalize(stmt);
            }
        }
        
        /* Try to drop the table to reset state for next iteration */
        sqlite3_exec(db, "DROP TABLE IF EXISTS fuzz_vtab;", NULL, NULL, NULL);
    }

    /* Case C: Test sqlite3_vtab_config */
    /* This must be called from within xConnect/xCreate, but we can trigger it indirectly
       by creating a vtab. However, direct calls outside the context are undefined.
       We rely on the module's xConnect/xCreate to potentially call it if we implemented it.
       Since our dummy module doesn't call it, we skip direct calls to avoid UB.
       The fuzzing of the vtab creation process above covers the path where this might be called.
    */

    /* Case D: Test sqlite3_vtab_collation */
    /* This must be called from xBestIndex. Our dummy xBestIndex doesn't call it.
       To test it, we would need a more complex module.
       However, we can try to trigger the vtab creation with constraints to see if
       the internal logic handles it.
    */
    if (Size > 10) {
        /* Try a query with COLLATE to see if it triggers vtab collation logic */
        char query[1024];
        size_t sqlLen;
        const char *colName = get_string_from_input(Data + 1, Size - 1, &sqlLen);
        
        /* Ensure colName is safe */
        if (sqlLen > 0 && sqlLen < 50) {
            snprintf(query, sizeof(query), "SELECT * FROM fuzz_vtab WHERE %s COLLATE BINARY = 'test';", colName);
            sqlite3_stmt *stmt;
            rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
            if (rc == SQLITE_OK) {
                sqlite3_step(stmt);
                sqlite3_finalize(stmt);
            }
        }
    }

    /* 3. Cleanup */
    sqlite3_close(db);
    
    return 0;
}