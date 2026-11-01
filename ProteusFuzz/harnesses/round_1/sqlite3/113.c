// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_reset_auto_extension at sqlite3.c:142457:17 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_enable_load_extension at sqlite3.c:142330:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_close_v2 at sqlite3.c:186977:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Dummy virtual table module implementation for sqlite3_create_module
 * All methods are no-ops to prevent crashes during fuzzing.
 */
static int dummy_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    return SQLITE_OK;
}

static int dummy_vtab_best_index(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) {
    return SQLITE_OK;
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    *ppCursor = NULL;
    return SQLITE_OK;
}

static int dummy_vtab_close(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_vtab_filter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) {
    return SQLITE_OK;
}

static int dummy_vtab_next(sqlite3_vtab_cursor *pCursor) {
    return SQLITE_OK;
}

static int dummy_vtab_eof(sqlite3_vtab_cursor *pCursor) {
    return 1;
}

static int dummy_vtab_column(sqlite3_vtab_cursor *pCursor, sqlite3_context *pCtx, int iCol) {
    sqlite3_result_null(pCtx);
    return SQLITE_OK;
}

static int dummy_vtab_rowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    *pRowid = 0;
    return SQLITE_OK;
}

static int dummy_vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    return SQLITE_OK;
}

static int dummy_vtab_begin(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_sync(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_commit(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_rollback(sqlite3_vtab *pVTab) {
    return SQLITE_OK;
}

static int dummy_vtab_find_function(sqlite3_vtab *pVtab, int nArg, const char *zName, void (**pxFunc)(sqlite3_context *, int, sqlite3_value **), void **ppArg) {
    return SQLITE_OK;
}

static int dummy_vtab_rename(sqlite3_vtab *pVtab, const char *zNew) {
    return SQLITE_OK;
}

static int dummy_vtab_savepoint(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_vtab_release(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_vtab_rollback_to(sqlite3_vtab *pVTab, int iSavepoint) {
    return SQLITE_OK;
}

static int dummy_vtab_shadow_name(const char *zName) {
    return SQLITE_OK;
}

static int dummy_vtab_integrity(sqlite3_vtab *pVTab, const char *zSchema, const char *zTabName, int mFlags, char **pzErr) {
    return SQLITE_OK;
}

/* Global dummy module instance */
static const sqlite3_module dummy_module = {
    4, /* iVersion */
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
    dummy_vtab_find_function,
    dummy_vtab_rename,
    dummy_vtab_savepoint,
    dummy_vtab_release,
    dummy_vtab_rollback_to,
    dummy_vtab_shadow_name,
    dummy_vtab_integrity
};

/*
 * Helper to extract a null-terminated string from fuzz input safely.
 * Returns a pointer to a newly allocated string if a null terminator is found within bounds,
 * otherwise returns NULL. The caller must free the returned string.
 */
static char *extract_string_safe(const uint8_t *Data, size_t Size) {
    size_t i = 0;
    while (i < Size && Data[i] != 0) {
        i++;
    }
    if (i == Size) {
        return NULL; // No null terminator found
    }
    // Found null terminator, allocate and copy
    char *str = (char *)malloc(i + 1);
    if (!str) return NULL;
    memcpy(str, Data, i);
    str[i] = '\0';
    return str;
}

int LLVMFuzzerTestOneInput_113(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;
    char *zErrMsg = NULL;
    
    /* 1. Prepare the environment */
    if (Size < 2) return 0;

    /* Open an in-memory database to avoid file system dependencies */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Fuzzing Logic */

    /* 
     * Branch 1: sqlite3_reset_auto_extension
     * This is a global function, no arguments. Call it directly.
     */
    sqlite3_reset_auto_extension();

    /* 
     * Branch 2: sqlite3_set_errmsg
     * Requires: sqlite3*, int errcode, const char* zErrMsg
     */
    if (Size >= 4) {
        int errcode = (int)(Data[0] | (Data[1] << 8));
        const char *msg = NULL;
        
        if (Size > 4) {
            char *safeMsg = extract_string_safe(Data + 4, Size - 4);
            if (safeMsg) {
                msg = safeMsg;
                rc = sqlite3_set_errmsg(db, errcode, msg);
                free((void*)msg);
            } else {
                rc = sqlite3_set_errmsg(db, errcode, NULL);
            }
        } else {
            rc = sqlite3_set_errmsg(db, errcode, NULL);
        }
        (void)rc; 
    }

    /* 
     * Branch 3: sqlite3_create_module
     * Requires: sqlite3*, const char* name, const sqlite3_module*, void* clientData
     */
    if (Size >= 2) {
        char *moduleName = extract_string_safe(Data, Size);
        void *clientData = (void *)(uintptr_t)(Data[0]); 
        
        if (moduleName) {
            rc = sqlite3_create_module(db, moduleName, &dummy_module, clientData);
            free(moduleName);
        } else {
            // Fallback to a safe default name if extraction fails
            rc = sqlite3_create_module(db, "fuzz_module", &dummy_module, clientData);
        }
        (void)rc;
    }

    /* 
     * Branch 4: sqlite3_drop_modules
     * Requires: sqlite3*, const char** azKeep
     */
    {
        const char *keepList[] = { NULL };
        rc = sqlite3_drop_modules(db, keepList);
        (void)rc;

        rc = sqlite3_drop_modules(db, NULL);
        (void)rc;
    }

    /* 
     * Branch 5: sqlite3_load_extension
     * Requires: sqlite3*, const char* zFile, const char* zProc, char** pzErrMsg
     */
    {
        rc = sqlite3_enable_load_extension(db, 1);
        if (rc == SQLITE_OK) {
            const char *zFile = "./dummy_file";
            const char *zProc = NULL;
            char *pzErr = NULL;
            
            rc = sqlite3_load_extension(db, zFile, zProc, &pzErr);
            if (rc != SQLITE_OK && pzErr) {
                sqlite3_free(pzErr);
            }
        }
    }

    /* 
     * Branch 6: sqlite3_close_v2
     * Requires: sqlite3*
     * Note: After this call, 'db' is closed and should not be used.
     */
    rc = sqlite3_close_v2(db);
    (void)rc;
    
    /* Ensure db is NULL after close to prevent use-after-free in subsequent logic */
    db = NULL;

    /* 
     * Additional exploration: Re-open and try combinations 
     * to ensure state changes are tested.
     */
    if (Size > 10) {
        rc = sqlite3_open(":memory:", &db);
        if (rc == SQLITE_OK && db) {
            /* Reset auto extensions again */
            sqlite3_reset_auto_extension();

            /* Set error msg with different data */
            int err2 = (int)(Data[10]);
            char *msg2 = NULL;
            if (Size > 11) {
                msg2 = extract_string_safe(Data + 11, Size - 11);
            }
            sqlite3_set_errmsg(db, err2, msg2);
            if (msg2) free(msg2);

            /* Create module with different name */
            char *name2 = NULL;
            if (Size > 15) {
                name2 = extract_string_safe(Data + 15, Size - 15);
            }
            if (name2) {
                sqlite3_create_module(db, name2, &dummy_module, NULL);
                free(name2);
            } else {
                sqlite3_create_module(db, "fuzz_module_2", &dummy_module, NULL);
            }

            /* Drop modules */
            sqlite3_drop_modules(db, NULL);

            /* Try load extension again */
            sqlite3_enable_load_extension(db, 1);
            char *pzErr2 = NULL;
            sqlite3_load_extension(db, "./dummy_file", NULL, &pzErr2);
            if (pzErr2) sqlite3_free(pzErr2);

            /* Close */
            sqlite3_close_v2(db);
            db = NULL; /* Mark as closed */
        }
    }

    /* 2. Cleanup */
    /* If db is still open (e.g., if the second block didn't run or failed), close it */
    if (db) {
        sqlite3_close_v2(db);
    }

    return 0;
}