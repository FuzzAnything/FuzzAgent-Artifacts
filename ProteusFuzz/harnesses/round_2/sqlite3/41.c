// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_set_errmsg at sqlite3.c:188366:16 in sqlite3.h
// sqlite3_errmsg at sqlite3.c:188336:24 in sqlite3.h
// sqlite3_errcode at sqlite3.c:188442:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_drop_modules at sqlite3.c:160477:16 in sqlite3.h
// sqlite3_load_extension at sqlite3.c:142299:16 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Dummy module implementation for sqlite3_create_module */
static int dummy_xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                         sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
    return SQLITE_OK;
}

static int dummy_xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                          sqlite3_vtab **ppVTab, char **pzErr) {
    (void)db; (void)pAux; (void)argc; (void)argv; (void)ppVTab; (void)pzErr;
    return SQLITE_OK;
}

static int dummy_xDisconnect(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xDestroy(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xOpen(sqlite3_vtab *pVTab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVTab; (void)ppCursor;
    return SQLITE_OK;
}

static int dummy_xClose(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_xFilter(sqlite3_vtab_cursor *pCursor, int idxNum, const char *idxStr,
                         int argc, sqlite3_value **argv) {
    (void)pCursor; (void)idxNum; (void)idxStr; (void)argc; (void)argv;
    return SQLITE_OK;
}

static int dummy_xNext(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return SQLITE_OK;
}

static int dummy_xEof(sqlite3_vtab_cursor *pCursor) {
    (void)pCursor;
    return 1;
}

static int dummy_xColumn(sqlite3_vtab_cursor *pCursor, sqlite3_context *ctx, int iCol) {
    (void)pCursor; (void)ctx; (void)iCol;
    sqlite3_result_text(ctx, "dummy", -1, SQLITE_STATIC);
    return SQLITE_OK;
}

static int dummy_xRowid(sqlite3_vtab_cursor *pCursor, sqlite3_int64 *pRowid) {
    (void)pCursor;
    *pRowid = 1;
    return SQLITE_OK;
}

static int dummy_xUpdate(sqlite3_vtab *pVTab, int nArg, sqlite3_value **argv, sqlite3_int64 *pRowid) {
    (void)pVTab; (void)nArg; (void)argv; (void)pRowid;
    return SQLITE_OK;
}

static int dummy_xBegin(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xSync(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xCommit(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static int dummy_xRollback(sqlite3_vtab *pVTab) {
    (void)pVTab;
    return SQLITE_OK;
}

static const sqlite3_module dummy_module = {
    0,
    dummy_xCreate,
    dummy_xConnect,
    NULL, /* xBestIndex */
    dummy_xDisconnect,
    dummy_xDestroy,
    dummy_xOpen,
    dummy_xClose,
    dummy_xFilter,
    dummy_xNext,
    dummy_xEof,
    dummy_xColumn,
    dummy_xRowid,
    dummy_xUpdate,
    dummy_xBegin,
    dummy_xSync,
    dummy_xCommit,
    dummy_xRollback,
    NULL, /* xFindFunction */
    NULL, /* xRename */
    NULL, /* xSavepoint */
    NULL, /* xRelease */
    NULL, /* xRollbackTo */
    NULL, /* xShadowName */
    NULL  /* xIntegrity */
};

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    int rc;

    /* Initialize database connection */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK || db == NULL) {
        return 0;
    }

    /* Use input data to drive fuzzing logic */
    if (Size > 0) {
        /* Fuzz sqlite3_set_errmsg */
        int errcode = (Data[0] % 100) + 100; /* Generate error code */
        const char *errmsg = (Size > 1) ? (const char *)(Data + 1) : NULL;
        /* Ensure null-termination if we use a string from input */
        if (errmsg != NULL && Size > 1) {
            /* Create a safe buffer for the error message */
            char safe_msg[256];
            size_t msg_len = (Size - 1 < 255) ? Size - 1 : 255;
            memcpy(safe_msg, Data + 1, msg_len);
            safe_msg[msg_len] = '\0';
            sqlite3_set_errmsg(db, errcode, safe_msg);
        } else {
            sqlite3_set_errmsg(db, errcode, NULL);
        }

        /* Fuzz sqlite3_errmsg and sqlite3_errcode */
        const char *err_msg = sqlite3_errmsg(db);
        (void)err_msg; /* Use to avoid unused variable warning */
        int err_code = sqlite3_errcode(db);
        (void)err_code; /* Use to avoid unused variable warning */
    }

    /* Fuzz sqlite3_create_module */
    if (Size > 10) {
        char module_name[64];
        size_t name_len = (Size - 10 < 63) ? Size - 10 : 63;
        memcpy(module_name, Data + 10, name_len);
        module_name[name_len] = '\0';
        rc = sqlite3_create_module(db, module_name, &dummy_module, NULL);
        (void)rc;
    }

    /* Fuzz sqlite3_drop_modules */
    if (Size > 20) {
        const char *keep_list[] = {(const char *)(Data + 20), NULL};
        rc = sqlite3_drop_modules(db, keep_list);
        (void)rc;
    } else {
        rc = sqlite3_drop_modules(db, NULL);
        (void)rc;
    }

    /* Fuzz sqlite3_load_extension (using a dummy file) */
    const char *dummy_file = "./dummy_file";
    FILE *f = fopen(dummy_file, "w");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
    char *pzErrMsg = NULL;
    rc = sqlite3_load_extension(db, dummy_file, NULL, &pzErrMsg);
    if (pzErrMsg) {
        sqlite3_free(pzErrMsg);
    }
    (void)rc;

    /* Cleanup */
    sqlite3_close(db);
    return 0;
}