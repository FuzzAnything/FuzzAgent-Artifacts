// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module_v2 at sqlite3.c:160460:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_vtab_config at sqlite3.c:161672:16 in sqlite3.h
// sqlite3_declare_vtab at sqlite3.c:161148:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a minimal virtual table module for fuzzing.
 * This module allows us to trigger the xCreate/xConnect methods where
 * sqlite3_vtab_config and sqlite3_declare_vtab are valid to call.
 */
static int dummy_vtab_create(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;
    
    /* Fuzz sqlite3_vtab_config options */
    /* SQLITE_VTAB_CONSTRAINT_SUPPORT */
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 1);
    sqlite3_vtab_config(db, SQLITE_VTAB_CONSTRAINT_SUPPORT, 0);
    
    /* SQLITE_VTAB_DIRECTONLY */
    sqlite3_vtab_config(db, SQLITE_VTAB_DIRECTONLY, 0);
    
    /* Fuzz sqlite3_declare_vtab with a simple valid schema */
    const char *zSchema = "CREATE TABLE dummy(id INTEGER PRIMARY KEY, val TEXT)";
    sqlite3_declare_vtab(db, zSchema);
    
    /* Allocate a dummy vtab structure */
    *ppVTab = malloc(sizeof(sqlite3_vtab));
    if (*ppVTab) {
        memset(*ppVTab, 0, sizeof(sqlite3_vtab));
    }
    return (*ppVTab) ? SQLITE_OK : SQLITE_NOMEM;
}

static int dummy_vtab_connect(sqlite3 *db, void *pAux, int argc, const char *const *argv, sqlite3_vtab **ppVTab, char **pzErr) {
    /* Reuse create logic for simplicity in fuzzing context */
    return dummy_vtab_create(db, pAux, argc, argv, ppVTab, pzErr);
}

static int dummy_vtab_disconnect(sqlite3_vtab *pVTab) {
    if (pVTab) free(pVTab);
    return SQLITE_OK;
}

static int dummy_vtab_destroy(sqlite3_vtab *pVTab) {
    if (pVTab) free(pVTab);
    return SQLITE_OK;
}

static int dummy_vtab_open(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVTab;
    *ppCursor = malloc(sizeof(sqlite3_vtab_cursor));
    if (*ppCursor) memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return (*ppCursor) ? SQLITE_OK : SQLITE_NOMEM;
}

static int dummy_vtab_close(sqlite3_vtab_cursor *pCursor) {
    if (pCursor) free(pCursor);
    return SQLITE_OK;
}

/* Minimal stubs for other required methods */
static int dummy_vtab_bestindex(sqlite3_vtab *pVTab, sqlite3_index_info *pIdxInfo) { (void)pVTab; (void)pIdxInfo; return SQLITE_OK; }
static int dummy_vtab_filter(sqlite3_vtab_cursor *pCur, int idxNum, const char *idxStr, int argc, sqlite3_value **argv) { (void)pCur; (void)idxNum; (void)idxStr; (void)argc; (void)argv; return SQLITE_OK; }
static int dummy_vtab_next(sqlite3_vtab_cursor *pCur) { (void)pCur; return SQLITE_OK; }
static int dummy_vtab_eof(sqlite3_vtab_cursor *pCur) { (void)pCur; return 1; }
static int dummy_vtab_column(sqlite3_vtab_cursor *pCur, sqlite3_context *ctx, int i) { (void)pCur; (void)ctx; (void)i; return SQLITE_OK; }
static int dummy_vtab_rowid(sqlite3_vtab_cursor *pCur, sqlite3_int64 *pRowid) { (void)pCur; if(pRowid) *pRowid = 1; return SQLITE_OK; }

static int dummy_vtab_update(sqlite3_vtab *pVTab, int argc, sqlite3_value **argv, sqlite3_int64 *pRowid) { 
    (void)argc; (void)argv; (void)pRowid;
    return SQLITE_OK;
}
static int dummy_vtab_begin(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_sync(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_commit(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_rollback(sqlite3_vtab *pVTab) { (void)pVTab; return SQLITE_OK; }
static int dummy_vtab_findfunc(sqlite3_vtab *pVTab, int nArg, const char *zName, void (**pxFunc)(sqlite3_context*,int,sqlite3_value**), void **ppArg) { 
    (void)pVTab; (void)nArg; (void)zName; (void)pxFunc; (void)ppArg; 
    return SQLITE_OK; 
}
static int dummy_vtab_rename(sqlite3_vtab *pVTab, const char *zNew) { (void)pVTab; (void)zNew; return SQLITE_OK; }
static int dummy_vtab_savepoint(sqlite3_vtab *pVTab, int i) { (void)pVTab; (void)i; return SQLITE_OK; }
static int dummy_vtab_release(sqlite3_vtab *pVTab, int i) { (void)pVTab; (void)i; return SQLITE_OK; }
static int dummy_vtab_rollback_to(sqlite3_vtab *pVTab, int i) { (void)pVTab; (void)i; return SQLITE_OK; }
static int dummy_vtab_shadowname(const char *z) { (void)z; return SQLITE_OK; }
static int dummy_vtab_integrity(sqlite3_vtab *pVTab, const char *zSchema, const char *zTabName, int mFlags, char **pzErr) { (void)pVTab; (void)zSchema; (void)zTabName; (void)mFlags; (void)pzErr; return SQLITE_OK; }

static const sqlite3_module dummy_module = {
    0, /* iVersion */
    dummy_vtab_create,
    dummy_vtab_connect,
    dummy_vtab_bestindex,
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
    dummy_vtab_findfunc,
    dummy_vtab_rename,
    dummy_vtab_savepoint,
    dummy_vtab_release,
    dummy_vtab_rollback_to,
    dummy_vtab_shadowname,
    dummy_vtab_integrity
};

int LLVMFuzzerTestOneInput_119(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Register the dummy module */
    rc = sqlite3_create_module_v2(db, "dummy_vtab", &dummy_module, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* 2. Fuzz sqlite3_overload_function */
    /* We must ensure the function name is null-terminated and within bounds.
       The previous crash occurred because we passed a pointer into the fuzz buffer
       that wasn't null-terminated, causing strlen to read past the buffer.
    */
    if (Size > 0) {
        /* Create a safe, null-terminated string from the input data */
        size_t max_name_len = 255;
        char safe_name[256];
        size_t copy_len = (Size < max_name_len) ? Size : max_name_len;
        
        /* Copy data and ensure null termination */
        memcpy(safe_name, Data, copy_len);
        safe_name[copy_len] = '\0';

        /* Check if the string is empty after copy (e.g., if input was all nulls or too short) */
        if (strlen(safe_name) > 0) {
            int nArg = (Size > 0) ? (Data[0] % 10) : 0;
            sqlite3_overload_function(db, safe_name, nArg);
        }
    }

    /* 3. Fuzz virtual table creation to trigger xCreate/xConnect context */
    /* We construct a CREATE VIRTUAL TABLE statement */
    const char *zCreateStmt = "CREATE VIRTUAL TABLE test_vtab USING dummy_vtab";
    sqlite3_stmt *stmt = NULL;
    
    rc = sqlite3_prepare_v2(db, zCreateStmt, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        rc = sqlite3_step(stmt);
        sqlite3_finalize(stmt);
    }

    /* 4. Fuzz sqlite3_vtab_nochange */
    /* This requires a sqlite3_context. We can't easily create one outside of a function call.
       However, we can trigger the xColumn method via a SELECT if the table exists.
    */
    if (Size > 0) {
        /* Try to execute a query to trigger xColumn if the table was created */
        const char *zQuery = "SELECT * FROM test_vtab LIMIT 1";
        sqlite3_stmt *qStmt = NULL;
        rc = sqlite3_prepare_v2(db, zQuery, -1, &qStmt, NULL);
        if (rc == SQLITE_OK) {
            rc = sqlite3_step(qStmt);
            sqlite3_finalize(qStmt);
        }
    }

    /* 5. Cleanup */
    sqlite3_close(db);

    return 0;
}