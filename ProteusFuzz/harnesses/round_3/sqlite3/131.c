// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_overload_function at sqlite3.c:187848:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_result_null at sqlite3.c:92781:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Static helper to create a minimal virtual table module for testing.
 * This module implements xConnect, xCreate, and xBestIndex to allow
 * the fuzzing of the target API functions.
 */
static int vtab_xConnect(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                         sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;
    /* Allocate a minimal structure to satisfy the vtab pointer requirement */
    *ppVtab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVtab) {
        memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    }
    return *ppVtab ? SQLITE_OK : SQLITE_NOMEM;
}

static int vtab_xCreate(sqlite3 *db, void *pAux, int argc, const char *const*argv,
                        sqlite3_vtab **ppVtab, char **pzErr) {
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;
    /* Allocate a minimal structure to satisfy the vtab pointer requirement */
    *ppVtab = (sqlite3_vtab *)sqlite3_malloc(sizeof(sqlite3_vtab));
    if (*ppVtab) {
        memset(*ppVtab, 0, sizeof(sqlite3_vtab));
    }
    return *ppVtab ? SQLITE_OK : SQLITE_NOMEM;
}

static int vtab_xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab;
    (void)pIdxInfo;
    /* This function is the entry point to fuzz the target APIs.
     * The fuzzer will trigger this via SQL queries. */
    return SQLITE_OK;
}

static int vtab_xDisconnect(sqlite3_vtab *pVtab) {
    if (pVtab) {
        sqlite3_free(pVtab);
    }
    return SQLITE_OK;
}

static int vtab_xDestroy(sqlite3_vtab *pVtab) {
    if (pVtab) {
        sqlite3_free(pVtab);
    }
    return SQLITE_OK;
}

static int vtab_xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    (void)ppCursor;
    return SQLITE_OK;
}

static int vtab_xClose(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int vtab_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                        int argc, sqlite3_value **argv) {
    (void)pCursor;
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    return SQLITE_OK;
}

static int vtab_xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_DONE;
}

static int vtab_xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int vtab_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int i) {
    (void)pCursor;
    (void)ctx;
    (void)i;
    sqlite3_result_null(ctx);
    return SQLITE_OK;
}

static int vtab_xRowid(sqlite3_vtab_cursor *pCursor, sqlite_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 0;
    return SQLITE_OK;
}

static int vtab_xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv,
                        sqlite_int64 *pRowid) {
    (void)pVtab;
    (void)argc;
    (void)argv;
    (void)pRowid;
    return SQLITE_OK;
}

static int vtab_xBegin(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int vtab_xSync(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int vtab_xCommit(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static int vtab_xRollback(sqlite3_vtab *pVtab) {
    (void)pVtab;
    return SQLITE_OK;
}

static const sqlite3_module vtab_module = {
    0, /* iVersion */
    vtab_xCreate,
    vtab_xConnect,
    vtab_xBestIndex,
    vtab_xDisconnect,
    vtab_xDestroy,
    vtab_xOpen,
    vtab_xClose,
    vtab_xFilter,
    vtab_xNext,
    vtab_xEof,
    vtab_xColumn,
    vtab_xRowid,
    vtab_xUpdate,
    vtab_xBegin,
    vtab_xSync,
    vtab_xCommit,
    vtab_xRollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
};

int LLVMFuzzerTestOneInput_131(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;

    /* 1. Prepare the environment */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        return 0;
    }

    /* Register the dummy virtual table module */
    rc = sqlite3_create_module(db, "fuzz_vtab", &vtab_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy virtual table to enable xBestIndex calls */
    /* We use a fixed string here to ensure a valid table is created before fuzzing SQL */
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE fuzz_test USING fuzz_vtab();", NULL, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /*
     * 3. Explore more program states by invoking the target function diversely.
     * We construct SQL queries from the fuzz input that will trigger
     * the xBestIndex and xConnect/xCreate methods of our virtual table.
     */

    /* Ensure the input is null-terminated for sqlite3_prepare_v2 */
    if (Size == 0) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a safe buffer for the SQL statement */
    char *sql_buf = (char *)malloc(Size + 1);
    if (!sql_buf) {
        sqlite3_close(db);
        return 0;
    }
    memcpy(sql_buf, Data, Size);
    sql_buf[Size] = '\0';

    /*
     * Try to prepare and execute the SQL.
     * This will trigger xBestIndex if the query involves the virtual table.
     */
    rc = sqlite3_prepare_v2(db, sql_buf, -1, &stmt, NULL);
    if (rc == SQLITE_OK) {
        /* Execute the statement to trigger xBestIndex and other methods */
        rc = sqlite3_step(stmt);
        (void)rc; /* Ignore execution errors, we just want to trigger the code paths */
        sqlite3_finalize(stmt);
    }

    /*
     * Additional exploration: Try to trigger xConnect/xCreate via explicit creation
     * if the input suggests a CREATE statement, or just rely on the existing table.
     * We can also try to overload functions to test sqlite3_overload_function.
     */
    if (Size > 10) {
        /* Attempt to overload a function based on input data */
        char func_name[32];
        int nArg = (Data[0] % 10) + 1;
        strncpy(func_name, (char *)&Data[1], sizeof(func_name) - 1);
        func_name[sizeof(func_name) - 1] = '\0';
        
        /* Call sqlite3_overload_function */
        sqlite3_overload_function(db, func_name, nArg);
    }

    /* 2. Conduct cleanup */
    if (stmt) {
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);
    free(sql_buf);

    return 0;
}