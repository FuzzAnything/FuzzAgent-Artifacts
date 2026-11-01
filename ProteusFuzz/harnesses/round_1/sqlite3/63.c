// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_int at sqlite3.c:92475:16 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_null at sqlite3.c:94039:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_vtab_in_next at sqlite3.c:93350:16 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/* 
 * Fuzz driver for sqlite3_value_* and sqlite3_vtab_in_* functions.
 * Fixed to ensure sqlite3_value objects are "protected" (mutex held) 
 * before calling accessors like sqlite3_value_int.
 */

/* Forward declarations for vtab methods */
static int xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                   sqlite3_vtab **ppVtab, char **pzErr);
static int xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                    sqlite3_vtab **ppVtab, char **pzErr);
static int xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo);
static int xDisconnect(sqlite3_vtab *pVtab);
static int xDestroy(sqlite3_vtab *pVtab);
static int xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor);
static int xClose(sqlite3_vtab_cursor *pCursor);
static int xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                   int argc, sqlite3_value **argv);
static int xNext(sqlite3_vtab_cursor *pCursor);
static int xEof(sqlite3_vtab_cursor *pCursor);
static int xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i);
static int xRowid(sqlite3_vtab_cursor *pCursor, sqlite_int64 *pRowid);
static int xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite_int64 *pRowid);
static int xBegin(sqlite3_vtab *pVtab);
static int xSync(sqlite3_vtab *pVtab);
static int xCommit(sqlite3_vtab *pVtab);
static int xRollback(sqlite3_vtab *pVtab);

/* Vtab method implementations */
static int xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                   sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)pzErr;
    *ppVtab = (sqlite3_vtab *)malloc(sizeof(sqlite3_vtab));
    if (!*ppVtab) return SQLITE_NOMEM;
    memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    (*ppVtab)->pModule = (sqlite3_module *)pAux;
    return SQLITE_OK;
}

static int xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                    sqlite3_vtab **ppVtab, char **pzErr) {
    return xCreate(db, pAux, argc, argv, ppVtab, pzErr);
}

static int xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab; (void)pIdxInfo;
    return SQLITE_OK;
}

static int xDisconnect(sqlite3_vtab *pVtab) {
    (void)pVtab;
    free(pVtab);
    return SQLITE_OK;
}

static int xDestroy(sqlite3_vtab *pVtab) {
    return xDisconnect(pVtab);
}

static int xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    *ppCursor = (sqlite3_vtab_cursor *)malloc(sizeof(sqlite3_vtab_cursor));
    if (!*ppCursor) return SQLITE_NOMEM;
    memset(*ppCursor, 0, sizeof(sqlite3_vtab_cursor));
    return SQLITE_OK;
}

static int xClose(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    free(pCursor);
    return SQLITE_OK;
}

static int xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                   int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr;
    
    /* Try to call sqlite3_vtab_in_first on each argument */
    for (int i = 0; i < argc; i++) {
        sqlite3_value *out;
        int rc = sqlite3_vtab_in_first(argv[i], &out);
        (void)rc;
        (void)out;
        
        /* Try next as well */
        while (rc == SQLITE_OK && out) {
            rc = sqlite3_vtab_in_next(argv[i], &out);
            (void)out;
        }
    }
    return SQLITE_OK;
}

static int xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_DONE;
}

static int xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor; (void)i;
    if (ctx) {
        sqlite3_result_null(ctx);
    }
    return SQLITE_OK;
}

static int xRowid(sqlite3_vtab_cursor *pCursor, sqlite_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 1;
    return SQLITE_OK;
}

static int xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv, sqlite_int64 *pRowid) {
    (void)pVtab; (void)argc; (void)argv; (void)pRowid;
    return SQLITE_OK;
}

static int xBegin(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int xSync(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int xCommit(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int xRollback(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static sqlite3_module vtabModule = {
    0, xCreate, xConnect, xBestIndex, xDisconnect, xDestroy,
    xOpen, xClose, xFilter, xNext, xEof, xColumn, xRowid,
    xUpdate, xBegin, xSync, xCommit, xRollback,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/* Helper function to safely test value accessors within a callback context */
static void test_value_accessors(sqlite3_context *ctx, int col_idx) {
    sqlite3_value *val = sqlite3_column_value(ctx, col_idx);
    if (!val) return;

    /* 
     * Note: sqlite3_column_value returns an UNPROTECTED value.
     * Accessors like sqlite3_value_int require a PROTECTED value (mutex held).
     * To safely fuzz these, we must call them from within a callback 
     * (where values are protected) or use sqlite3_value_dup.
     * Here we use a dummy callback approach or rely on the fact that 
     * in some internal states, the mutex might be held, but the safest 
     * way for fuzzing specific APIs is to use a custom function callback.
     * However, since we are in a row retrieval context, we can't easily 
     * create a callback for column values.
     * 
     * Correction: The error occurred because we called sqlite3_value_int 
     * on an unprotected value from sqlite3_column_value.
     * We must NOT call these functions on sqlite3_column_value directly 
     * outside of a callback.
     * 
     * Strategy Change: We will create a custom SQL function that receives 
     * the values as arguments (which are protected) and test the APIs there.
     */
}

/* Custom function to test value APIs on protected values */
static void test_value_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    for (int i = 0; i < argc; i++) {
        sqlite3_value *val = argv[i];
        if (!val) continue;

        /* These calls are safe here because argv in a function callback 
           contains protected sqlite3_value objects. */
        
        int intVal = sqlite3_value_int(val);
        (void)intVal;

        int type = sqlite3_value_type(val);
        (void)type;

        int isFromBind = sqlite3_value_frombind(val);
        (void)isFromBind;

        int isNoChange = sqlite3_value_nochange(val);
        (void)isNoChange;

        int bytes16 = sqlite3_value_bytes16(val);
        (void)bytes16;
    }
    sqlite3_result_null(ctx);
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    
    /* Create an in-memory database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Create a simple table to test with */
    const char *createSQL = "CREATE TABLE test_table (id INTEGER, data TEXT, blob_data BLOB);";
    rc = sqlite3_exec(db, createSQL, NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register the custom test function */
    rc = sqlite3_create_function(db, "fuzz_test_func", -1, SQLITE_UTF8, NULL, test_value_func, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare an insert statement with bound parameters */
    const char *insertSQL = "INSERT INTO test_table (id, data, blob_data) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, insertSQL, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Use input data to bind values */
    if (Size >= 4) {
        int idVal = *(const int32_t *)(Data);
        sqlite3_bind_int(stmt, 1, idVal);
    }

    if (Size >= 8) {
        int len = *(const int32_t *)(Data + 4);
        if (len < 0 || len > (int)(Size - 8)) len = (int)(Size - 8);
        if (len > 0) {
            sqlite3_bind_text(stmt, 2, (const char *)(Data + 8), len, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 2);
        }
    }

    if (Size >= 12) {
        int blobLen = *(const int32_t *)(Data + 8);
        if (blobLen < 0 || blobLen > (int)(Size - 12)) blobLen = (int)(Size - 12);
        if (blobLen > 0) {
            sqlite3_bind_blob(stmt, 3, (const void *)(Data + 12), blobLen, SQLITE_TRANSIENT);
        } else {
            sqlite3_bind_null(stmt, 3);
        }
    }

    /* Execute the insert */
    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare a select statement that calls our custom function to trigger the tests */
    const char *selectSQL = "SELECT fuzz_test_func(id, data, blob_data) FROM test_table LIMIT 1;";
    rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Execute to trigger the callback where we test the APIs */
    rc = sqlite3_step(stmt);
    (void)rc;

    sqlite3_finalize(stmt);

    /* Attempt to test sqlite3_vtab_in_first with a dummy vtab setup */
    static const char *vtabName = "fuzz_vtab";
    
    /* Register the vtab module */
    rc = sqlite3_create_module(db, vtabName, &vtabModule, NULL);
    if (rc == SQLITE_OK) {
        /* Create the vtab */
        const char *createVtabSQL = "CREATE VIRTUAL TABLE fuzz_vtab USING fuzz_vtab;";
        rc = sqlite3_exec(db, createVtabSQL, NULL, NULL, NULL);
        if (rc == SQLITE_OK) {
            /* Prepare a query that might use IN constraint */
            const char *querySQL = "SELECT * FROM fuzz_vtab WHERE id IN (1, 2, 3);";
            sqlite3_stmt *vtabStmt;
            rc = sqlite3_prepare_v2(db, querySQL, -1, &vtabStmt, NULL);
            if (rc == SQLITE_OK) {
                /* Step to trigger xFilter */
                sqlite3_step(vtabStmt);
                sqlite3_finalize(vtabStmt);
            }
        }
    }

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}