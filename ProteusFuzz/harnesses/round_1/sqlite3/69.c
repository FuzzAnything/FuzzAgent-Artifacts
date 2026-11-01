// This fuzz driver is generated for library sqlite3, aiming to fuzz the following functions:
// sqlite3_vtab_in_first at sqlite3.c:93342:16 in sqlite3.h
// sqlite3_result_text at sqlite3.c:92829:17 in sqlite3.h
// sqlite3_open at sqlite3.c:189310:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_create_function at sqlite3.c:187742:16 in sqlite3.h
// sqlite3_create_module at sqlite3.c:160445:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_exec at sqlite3.c:140725:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_prepare_v2 at sqlite3.c:147216:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_text at sqlite3.c:94068:16 in sqlite3.h
// sqlite3_bind_int at sqlite3.c:94025:16 in sqlite3.h
// sqlite3_bind_double at sqlite3.c:94014:16 in sqlite3.h
// sqlite3_bind_blob at sqlite3.c:93992:16 in sqlite3.h
// sqlite3_step at sqlite3.c:93168:16 in sqlite3.h
// sqlite3_column_count at sqlite3.c:93521:16 in sqlite3.h
// sqlite3_column_value at sqlite3.c:93676:27 in sqlite3.h
// sqlite3_value_type at sqlite3.c:92516:16 in sqlite3.h
// sqlite3_value_encoding at sqlite3.c:92600:16 in sqlite3.h
// sqlite3_value_bytes16 at sqlite3.c:92469:16 in sqlite3.h
// sqlite3_value_frombind at sqlite3.c:92610:16 in sqlite3.h
// sqlite3_value_nochange at sqlite3.c:92605:16 in sqlite3.h
// sqlite3_finalize at sqlite3.c:92370:16 in sqlite3.h
// sqlite3_close at sqlite3.c:186976:16 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
// sqlite3_malloc at sqlite3.c:31402:18 in sqlite3.h
// sqlite3_free at sqlite3.c:31477:17 in sqlite3.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

/*
 * Fuzz driver for sqlite3_value_encoding, sqlite3_value_bytes16,
 * sqlite3_vtab_in_first, sqlite3_value_frombind, sqlite3_value_type,
 * and sqlite3_value_nochange.
 */

static int callback(void *data, int argc, char **argv, char **col_name) {
    (void)data;
    (void)argc;
    (void)argv;
    (void)col_name;
    return 0;
}

/* Fixed: Return type changed to void to match sqlite3_create_function signature */
static void custom_func(sqlite3_context *ctx, int argc, sqlite3_value **argv) {
    (void)ctx;
    (void)argc;
    (void)argv;
    /* Do nothing */
}

/* Virtual Table Implementation */
typedef struct MyVtab {
    sqlite3_vtab base;
} MyVtab;

typedef struct MyVtabCursor {
    sqlite3_vtab_cursor base;
    int rowid;
} MyVtabCursor;

static int xCreate(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                   sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db;
    (void)pAux;
    (void)argc;
    (void)argv;
    (void)pzErr;
    MyVtab *pNew = (MyVtab *)sqlite3_malloc(sizeof(MyVtab));
    if (pNew == 0) return SQLITE_NOMEM;
    pNew->base.pModule = (sqlite3_module *)pAux;
    *ppVtab = &pNew->base;
    return SQLITE_OK;
}

static int xConnect(sqlite3 *db, void *pAux, int argc, const char *const *argv,
                    sqlite3_vtab **ppVtab, char **pzErr) {
    (void)db;
    (void)argc;
    (void)argv;
    (void)pzErr;
    MyVtab *pNew = (MyVtab *)sqlite3_malloc(sizeof(MyVtab));
    if (pNew == 0) return SQLITE_NOMEM;
    pNew->base.pModule = (sqlite3_module *)pAux;
    *ppVtab = &pNew->base;
    return SQLITE_OK;
}

static int xBestIndex(sqlite3_vtab *pVtab, sqlite3_index_info *pIdxInfo) {
    (void)pVtab;
    (void)pIdxInfo;
    return SQLITE_OK;
}

static int xDisconnect(sqlite3_vtab *pVtab) {
    if (pVtab) {
        sqlite3_free(pVtab);
    }
    return SQLITE_OK;
}

static int xDestroy(sqlite3_vtab *pVtab) {
    if (pVtab) {
        sqlite3_free(pVtab);
    }
    return SQLITE_OK;
}

static int xOpen(sqlite3_vtab *pVtab, sqlite3_vtab_cursor **ppCursor) {
    (void)pVtab;
    MyVtabCursor *pCur = (MyVtabCursor *)sqlite3_malloc(sizeof(MyVtabCursor));
    if (pCur == 0) return SQLITE_NOMEM;
    pCur->rowid = 0;
    *ppCursor = &pCur->base;
    return SQLITE_OK;
}

static int xClose(sqlite3_vtab_cursor *cur) {
    if (cur) {
        sqlite3_free(cur);
    }
    return SQLITE_OK;
}

static int xFilter(sqlite3_vtab_cursor *pVtabCur, int idxNum, const char *idxStr,
                   int argc, sqlite3_value **argv) {
    (void)idxNum;
    (void)idxStr;
    (void)argc;
    (void)argv;
    
    MyVtabCursor *pCur = (MyVtabCursor *)pVtabCur;
    pCur->rowid = 0;

    /*
     * Attempt to call sqlite3_vtab_in_first. This is only valid if
     * the corresponding parameter was marked with sqlite3_vtab_in()
     * in xBestIndex. Since we don't implement a full vtab here,
     * this call is likely to return SQLITE_ERROR or behave unexpectedly.
     * We include it to exercise the code path.
     */
    if (argc > 0) {
        sqlite3_value *out;
        int rc = sqlite3_vtab_in_first(argv[0], &out);
        (void)rc; 
        (void)out;
    }

    return SQLITE_OK;
}

static int xNext(sqlite3_vtab_cursor *pVtabCur) {
    MyVtabCursor *pCur = (MyVtabCursor *)pVtabCur;
    pCur->rowid++;
    return SQLITE_OK;
}

static int xEof(sqlite3_vtab_cursor *pVtabCur) {
    MyVtabCursor *pCur = (MyVtabCursor *)pVtabCur;
    return (pCur->rowid > 100); /* Limit rows for safety */
}

static int xColumn(sqlite3_vtab_cursor *pVtabCur, sqlite3_context *ctx, int i) {
    (void)pVtabCur;
    (void)i;
    sqlite3_result_text(ctx, "fuzz_data", -1, SQLITE_STATIC);
    return SQLITE_OK;
}

static int xRowid(sqlite3_vtab_cursor *pVtabCur, sqlite3_int64 *pRowid) {
    MyVtabCursor *pCur = (MyVtabCursor *)pVtabCur;
    *pRowid = pCur->rowid;
    return SQLITE_OK;
}

static int xUpdate(sqlite3_vtab *pVtab, int argc, sqlite3_value **argv,
                   sqlite3_int64 *pRowid) {
    (void)pVtab;
    (void)argc;
    (void)argv;
    (void)pRowid;
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

static const sqlite3_module vtab_module = {
    0, /* iVersion */
    xCreate,
    xConnect,
    xBestIndex,
    xDisconnect,
    xDestroy,
    xOpen,
    xClose,
    xFilter,
    xNext,
    xEof,
    xColumn,
    xRowid,
    xUpdate,
    xBegin,
    xSync,
    xCommit,
    xRollback,
    0, /* xFindMethod */
    0, /* xRename */
    0, /* xSavepoint */
    0, /* xRelease */
    0, /* xRollbackTo */
    0, /* xShadowName */
};

int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    int rc;
    sqlite3_value *val = NULL;
    int encoding, type, frombind, nochange;
    int bytes16;
    const char *sql = "SELECT ?1, ?2, ?3, ?4, ?5, ?6;";
    const char *text_val = "test_string";
    const char *blob_val = "blob_data";
    int int_val = 42;
    double real_val = 3.14;

    /* Initialize database */
    rc = sqlite3_open(":memory:", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create a dummy table and insert data to ensure valid statements */
    rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS test (id INTEGER PRIMARY KEY, data TEXT);", callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Register custom function to test value APIs in callback context */
    sqlite3_create_function(db, "fuzz_func", 1, SQLITE_UTF8, NULL, custom_func, NULL, NULL);

    /* Register virtual table module */
    rc = sqlite3_create_module(db, "fuzz_vtab", &vtab_module, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Create virtual table */
    rc = sqlite3_exec(db, "CREATE VIRTUAL TABLE IF NOT EXISTS fuzz_vtab USING fuzz_vtab;", callback, NULL, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Prepare statement */
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return 0;
    }

    /* Bind values to parameters */
    if (Size > 0) {
        /* Use input data to influence bound values */
        sqlite3_bind_text(stmt, 1, (const char *)Data, Size, SQLITE_TRANSIENT);
        if (Size > 1) {
            sqlite3_bind_text(stmt, 2, (const char *)Data + 1, Size - 1, SQLITE_TRANSIENT);
        }
        if (Size > 2) {
            sqlite3_bind_text(stmt, 3, (const char *)Data + 2, Size - 2, SQLITE_TRANSIENT);
        }
        sqlite3_bind_int(stmt, 4, int_val);
        sqlite3_bind_double(stmt, 5, real_val);
        sqlite3_bind_blob(stmt, 6, blob_val, strlen(blob_val), SQLITE_TRANSIENT);
    } else {
        /* Default bindings if input is empty */
        sqlite3_bind_text(stmt, 1, text_val, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, text_val, -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, text_val, -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, int_val);
        sqlite3_bind_double(stmt, 5, real_val);
        sqlite3_bind_blob(stmt, 6, blob_val, strlen(blob_val), SQLITE_TRANSIENT);
    }

    /* Step through statement to get column values */
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < sqlite3_column_count(stmt); i++) {
            val = sqlite3_column_value(stmt, i);
            if (val == NULL) continue;

            /* Test sqlite3_value_type */
            type = sqlite3_value_type(val);
            (void)type;

            /* Test sqlite3_value_encoding (only meaningful for TEXT) */
            if (type == SQLITE_TEXT) {
                encoding = sqlite3_value_encoding(val);
                (void)encoding;
            }

            /* Test sqlite3_value_bytes16 */
            bytes16 = sqlite3_value_bytes16(val);
            (void)bytes16;

            /* Test sqlite3_value_frombind */
            frombind = sqlite3_value_frombind(val);
            (void)frombind;

            /* Test sqlite3_value_nochange */
            nochange = sqlite3_value_nochange(val);
            (void)nochange;
        }
    }

    /* Clean up statement */
    sqlite3_finalize(stmt);

    /* Close database */
    sqlite3_close(db);

    return 0;
}